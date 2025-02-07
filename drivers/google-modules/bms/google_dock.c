/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright 2021 Google, LLC
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_gpio.h>
#include <linux/of_irq.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include "gbms_power_supply.h"
#include "google_bms.h"
#include "google_psy.h"
#include "pmic-voter.h"

#define DOCK_USER_VOTER			"DOCK_USER_VOTER"
#define DOCK_AICL_VOTER			"DOCK_AICL_VOTER"

#define DOCK_DELAY_INIT_MS		500
#define DOCK_NOTIFIER_DELAY_MS		100
#define DOCK_ICL_DEFAULT_UA		500000
#define DOCK_15W_ILIM_UA		1250000
#define DOCK_13_5W_ILIM_UA		1500000
#define DOCK_13_5W_VOUT_UV		9000000
#define DOCK_ICL_RAMP_DELAY_DEFAULT_MS	(4 * 1000)	/* 4 seconds */

struct dock_drv {
	struct device *device;
	struct power_supply *psy;
	const char *dc_psy_name;
	struct power_supply *dc_psy;
	struct mutex dock_lock;
	struct delayed_work init_work;
	struct delayed_work notifier_work;
	struct delayed_work icl_ramp_work;
	struct alarm icl_ramp_alarm;
	struct notifier_block nb;
	struct votable *dc_icl_votable;

	bool init_complete;
	bool check_dc;
	bool icl_ramp;
	u32 icl_ramp_ua;
	u32 icl_ramp_delay_ms;
	int online;
	int pogo_ovp_en;
};

static int dock_has_dc_in(struct dock_drv *dock)
{
	union power_supply_propval prop;
	int ret;

	if (!dock->dc_psy) {
		dock->dc_psy = power_supply_get_by_name("dc");
		if (!dock->dc_psy)
			return -EINVAL;
	}

	ret = power_supply_get_property(dock->dc_psy,
					POWER_SUPPLY_PROP_PRESENT, &prop);
	if (ret < 0) {
		dev_err(dock->device, "Error getting charging status: %d\n", ret);
		return -EINVAL;
	}

	return prop.intval != 0;
}

static bool google_dock_find_votable(struct dock_drv *dock)
{
	if (!dock->dc_icl_votable) {
		dock->dc_icl_votable = find_votable("DC_ICL");
		if (!dock->dc_icl_votable) {
			dev_err(dock->device, "Could not get votable: DC_ICL\n");
			return false;
		}
	}

	return true;
}

static void google_dock_set_icl(struct dock_drv *dock)
{
	int icl;

	if (!google_dock_find_votable(dock))
		return;

	/* Default ICL */
	icl = DOCK_ICL_DEFAULT_UA;

	if (dock->icl_ramp)
		icl = dock->icl_ramp_ua;

	vote(dock->dc_icl_votable, DOCK_AICL_VOTER, true, icl);

	dev_info(dock->device, "Setting ICL %duA ramp=%d\n", icl, dock->icl_ramp);
}

static void google_dock_vote_defaults(struct dock_drv *dock)
{
	if (!google_dock_find_votable(dock))
		return;

	vote(dock->dc_icl_votable, DOCK_AICL_VOTER, false, 0);
}

static enum alarmtimer_restart google_dock_icl_ramp_alarm_cb(struct alarm
							     *alarm,
							     ktime_t now)
{
	struct dock_drv *dock = container_of(alarm, struct dock_drv,
					     icl_ramp_alarm);

	/* Alarm is in atomic context, schedule work to complete the task */
	schedule_delayed_work(&dock->icl_ramp_work, msecs_to_jiffies(100));

	return ALARMTIMER_NORESTART;
}

static void google_dock_icl_ramp_work(struct work_struct *work)
{
	struct dock_drv *dock = container_of(work, struct dock_drv,
					     icl_ramp_work.work);
	int online, voltage;

	online = GPSY_GET_PROP(dock->dc_psy, POWER_SUPPLY_PROP_ONLINE);
	if (!online || online == dock->online)
		goto out;

	voltage = GPSY_GET_PROP(dock->dc_psy, POWER_SUPPLY_PROP_VOLTAGE_NOW);
	if (voltage > DOCK_13_5W_VOUT_UV)
		dock->icl_ramp_ua = DOCK_15W_ILIM_UA;
	else
		dock->icl_ramp_ua = DOCK_13_5W_ILIM_UA;

	dock->icl_ramp = true;

	dev_info(dock->device, "ICL ramp work, ramp=%d icl=%d\n",
		 dock->icl_ramp, dock->icl_ramp_ua);

	google_dock_set_icl(dock);

out:
	pr_info("%s: online: %d->%d\n", __func__, dock->online, online);
	dock->online = online;
}

static void google_dock_icl_ramp_reset(struct dock_drv *dock)
{
	dev_info(dock->device, "ICL ramp reset, ramp=%d\n", dock->icl_ramp);

	dock->icl_ramp = false;

	if (alarm_try_to_cancel(&dock->icl_ramp_alarm) < 0)
		dev_warn(dock->device, "Couldn't cancel icl_ramp_alarm\n");
	cancel_delayed_work(&dock->icl_ramp_work);
}

static void google_dock_icl_ramp_start(struct dock_drv *dock)
{

	google_dock_icl_ramp_reset(dock);

	dev_info(dock->device, "ICL ramp set alarm %dms, %dua, ramp=%d\n",
		 dock->icl_ramp_delay_ms, dock->icl_ramp_ua, dock->icl_ramp);

	alarm_start_relative(&dock->icl_ramp_alarm,
			     ms_to_ktime(dock->icl_ramp_delay_ms));
}

static void google_dock_notifier_check_dc(struct dock_drv *dock)
{
	int dc_in;

	dock->check_dc = false;

	dc_in = dock_has_dc_in(dock);
	if (dc_in < 0)
		return;

	dev_info(dock->device, "dc status is %d\n", dc_in);

	if (dc_in) {
		google_dock_set_icl(dock);
		google_dock_icl_ramp_start(dock);
	} else {
		dock->online = 0;
		google_dock_vote_defaults(dock);
		google_dock_icl_ramp_reset(dock);
	}

	power_supply_changed(dock->psy);
}

static void google_dock_notifier_work(struct work_struct *work)
{
	struct dock_drv *dock = container_of(work, struct dock_drv,
					     notifier_work.work);

	dev_info(dock->device, "notifier_work\n");

	if (dock->check_dc)
		google_dock_notifier_check_dc(dock);
}

static int google_dock_notifier_cb(struct notifier_block *nb,
				   unsigned long event, void *data)
{
	struct power_supply *psy = data;
	struct dock_drv *dock = container_of(nb, struct dock_drv, nb);

	if (event != PSY_EVENT_PROP_CHANGED)
		goto out;

	if (dock->dc_psy_name &&
	    (!strcmp(psy->desc->name, "dc")
	     || !strcmp(psy->desc->name, "main-charger")))
		dock->check_dc = true;

	if (!dock->check_dc)
		goto out;

	schedule_delayed_work(&dock->notifier_work,
			      msecs_to_jiffies(DOCK_NOTIFIER_DELAY_MS));

out:
	return NOTIFY_OK;
}

static int google_dock_parse_dt(struct device *dev,
			  struct dock_drv *dock)
{
	int ret = 0;
	struct device_node *node = dev->of_node;

	/* POGO_OVP_EN */
	ret = of_get_named_gpio(node, "google,pogo_ovp_en", 0);
	dock->pogo_ovp_en = ret;
	if (ret < 0)
		dev_warn(dev, "unable to read google,pogo_ovp_en from dt: %d\n",
			 ret);
	else
		dev_info(dev, "POGO_OVP_EN gpio:%d", dock->pogo_ovp_en);

	return 0;
}

static enum power_supply_property dock_props[] = {
	POWER_SUPPLY_PROP_PRESENT,
	POWER_SUPPLY_PROP_ONLINE,
	POWER_SUPPLY_PROP_VOLTAGE_NOW,
	POWER_SUPPLY_PROP_CURRENT_MAX,
	POWER_SUPPLY_PROP_CURRENT_NOW,
	POWER_SUPPLY_PROP_VOLTAGE_MAX,
};

static int dock_get_property(struct power_supply *psy,
			     enum power_supply_property psp,
			     union power_supply_propval *val)
{
	struct dock_drv *dock = (struct dock_drv *)
					power_supply_get_drvdata(psy);
	int ret = 0;

	if (!dock->init_complete)
		return -EAGAIN;

	if (!dock->dc_psy && dock->dc_psy_name)
		dock->dc_psy = power_supply_get_by_name(dock->dc_psy_name);

	switch (psp) {
	case POWER_SUPPLY_PROP_PRESENT:
		val->intval = dock_has_dc_in(dock);
		if (val->intval < 0)
			val->intval = 0;
		break;

	case POWER_SUPPLY_PROP_CURRENT_MAX:
		if (!dock->dc_icl_votable)
			return -EAGAIN;

		ret = get_effective_result(dock->dc_icl_votable);
		if (ret < 0)
			break;

		val->intval = ret;

		/* success */
		ret = 0;
		break;

	default:
		ret = power_supply_get_property(dock->dc_psy, psp, val);
		break;
	}

	if (ret)
		dev_dbg(dock->device, "Couldn't get prop %d, ret=%d\n", psp, ret);

	return 0;
}

static int dock_set_property(struct power_supply *psy,
			     enum power_supply_property psp,
			     const union power_supply_propval *val)
{
	struct dock_drv *dock = (struct dock_drv *)
					power_supply_get_drvdata(psy);
	bool changed = false;
	int ret = 0;

	if (!dock->init_complete)
		return -EAGAIN;

	switch (psp) {
	case POWER_SUPPLY_PROP_CURRENT_MAX:
		if (val->intval < 0) {
			ret = -EINVAL;
			break;
		}

		if (!dock->dc_icl_votable) {
			ret = -EAGAIN;
			break;
		}

		ret = vote(dock->dc_icl_votable, DOCK_USER_VOTER, true,
			   val->intval);
		changed = true;
		break;
	default:
		return -EINVAL;
	}

	if (ret)
		dev_dbg(dock->device, "Couldn't set prop %d, ret=%d\n", psp, ret);

	if (changed)
		power_supply_changed(psy);

	return 0;
}

static int dock_property_is_writeable(struct power_supply *psy,
				      enum power_supply_property psp)
{
	switch (psp) {
	case POWER_SUPPLY_PROP_CURRENT_MAX:
		return 1;
	default:
		break;
	}

	return 0;
}

static struct power_supply_desc dock_psy_desc = {
	.name = "dock",
	.type = POWER_SUPPLY_TYPE_BATTERY,
	.get_property = dock_get_property,
	.set_property = dock_set_property,
	.property_is_writeable = dock_property_is_writeable,
	.properties = dock_props,
	.num_properties = ARRAY_SIZE(dock_props),
};

/* ------------------------------------------------------------------------ */

static void google_dock_init_work(struct work_struct *work)
{
	struct dock_drv *dock = container_of(work, struct dock_drv,
					     init_work.work);
	struct power_supply *dc_psy = dock->dc_psy;
	union power_supply_propval val;
	int err = 0;

	if (!dock->dc_psy && dock->dc_psy_name) {
		dc_psy = power_supply_get_by_name(dock->dc_psy_name);
		if (!dc_psy) {
			dev_info(dock->device,
				 "failed to get \"%s\" power supply, retrying...\n",
				 dock->dc_psy_name);
			goto retry_init_work;
		}
		dock->dc_psy = dc_psy;

		/* FIXME */
		err = power_supply_get_property(dc_psy,
						POWER_SUPPLY_PROP_PRESENT,
						&val);
		if (err == -EAGAIN)
			goto retry_init_work;
	}

	dock->init_complete = true;
	dev_info(dock->device, "google_dock_init_work done\n");

	return;

retry_init_work:
	schedule_delayed_work(&dock->init_work,
			      msecs_to_jiffies(DOCK_DELAY_INIT_MS));
}

static int google_dock_probe(struct platform_device *pdev)
{
	const char *dc_psy_name;
	struct dock_drv *dock;
	int ret;
	struct power_supply_config psy_cfg = {};

	dock = devm_kzalloc(&pdev->dev, sizeof(*dock), GFP_KERNEL);
	if (!dock)
		return -ENOMEM;

	dock->device = &pdev->dev;

	ret = of_property_read_string(pdev->dev.of_node, "google,dc-psy-name",
				      &dc_psy_name);
	if (ret == 0) {
		pr_info("google,dc-psy-name=%s\n", dc_psy_name);
		dock->dc_psy_name = devm_kstrdup(&pdev->dev,
						 dc_psy_name, GFP_KERNEL);
		if (!dock->dc_psy_name) {
			devm_kfree(&pdev->dev, dock);
			return -ENOMEM;
		}
	}

	google_dock_parse_dt(dock->device, dock);
	mutex_init(&dock->dock_lock);
	INIT_DELAYED_WORK(&dock->init_work, google_dock_init_work);
	INIT_DELAYED_WORK(&dock->icl_ramp_work, google_dock_icl_ramp_work);
	alarm_init(&dock->icl_ramp_alarm, ALARM_BOOTTIME,
		   google_dock_icl_ramp_alarm_cb);

	dock->icl_ramp_delay_ms = DOCK_ICL_RAMP_DELAY_DEFAULT_MS;
	dock->online = 0;

	platform_set_drvdata(pdev, dock);

	psy_cfg.drv_data = dock;
	psy_cfg.of_node = pdev->dev.of_node;

	if (of_property_read_bool(pdev->dev.of_node, "google,psy-type-unknown"))
		dock_psy_desc.type = POWER_SUPPLY_TYPE_UNKNOWN;

	dock->psy = devm_power_supply_register(dock->device,
					       &dock_psy_desc, &psy_cfg);
	if (IS_ERR(dock->psy)) {
		ret = PTR_ERR(dock->psy);
		dev_err(dock->device, "Couldn't register as power supply, ret=%d\n", ret);
		devm_kfree(&pdev->dev, dock);
		return ret;
	}

	/*
	 * Find the DC_ICL votable
	 */
	dock->dc_icl_votable = find_votable("DC_ICL");
	if (!dock->dc_icl_votable)
		dev_warn(dock->device, "Could not find DC_ICL votable\n");

	/*
	 * Register notifier so we can detect changes on DC_IN
	 */
	INIT_DELAYED_WORK(&dock->notifier_work, google_dock_notifier_work);
	dock->nb.notifier_call = google_dock_notifier_cb;
	ret = power_supply_reg_notifier(&dock->nb);
	if (ret) {
		dev_err(dock->device, "Fail to register notifier: %d\n", ret);
		devm_kfree(&pdev->dev, dock);
		return ret;
	}

	if (dock->pogo_ovp_en >= 0)
		gpio_direction_output(dock->pogo_ovp_en, 1);

	schedule_delayed_work(&dock->init_work,
			      msecs_to_jiffies(DOCK_DELAY_INIT_MS));

	pr_info("google_dock_probe done\n");

	return 0;
}

static int google_dock_remove(struct platform_device *pdev)
{
	struct dock_drv *dock = platform_get_drvdata(pdev);

	cancel_delayed_work(&dock->init_work);
	cancel_delayed_work(&dock->notifier_work);
	cancel_delayed_work(&dock->icl_ramp_work);
	alarm_try_to_cancel(&dock->icl_ramp_alarm);

	return 0;
}

static const struct of_device_id google_dock_of_match[] = {
	{.compatible = "google,dock"},
	{},
};
MODULE_DEVICE_TABLE(of, google_dock_of_match);

static struct platform_driver google_dock_driver = {
	.driver = {
		   .name = "google,dock",
		   .owner = THIS_MODULE,
		   .of_match_table = google_dock_of_match,
		   .probe_type = PROBE_PREFER_ASYNCHRONOUS,
		   },
	.probe = google_dock_probe,
	.remove = google_dock_remove,
};

module_platform_driver(google_dock_driver);

MODULE_DESCRIPTION("Google Dock Driver");
MODULE_AUTHOR("Jack Wu <wjack@google.com>");
MODULE_LICENSE("GPL");
