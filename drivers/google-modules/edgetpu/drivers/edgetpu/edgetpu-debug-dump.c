// SPDX-License-Identifier: GPL-2.0
/*
 * Module that defines structures and functions to retrieve debug dump segments
 * from edgetpu firmware.
 *
 * Copyright (C) 2020 Google, Inc.
 */
#include <linux/workqueue.h>

#include "edgetpu-config.h"
#include "edgetpu-debug-dump.h"
#include "edgetpu-device-group.h"
#include "edgetpu-iremap-pool.h"
#include "edgetpu-kci.h"

int edgetpu_get_debug_dump(struct edgetpu_dev *etdev, u64 type)
{
	int ret;
	struct edgetpu_debug_dump_setup *dump_setup;

	if (!etdev->debug_dump_mem.vaddr) {
		etdev_err(etdev, "Debug dump not allocated");
		return -EINVAL;
	}

	dump_setup =
		(struct edgetpu_debug_dump_setup *)etdev->debug_dump_mem.vaddr;
	dump_setup->type = type;
	/* Signal the type of dump and buffer address to firmware */
	ret = edgetpu_kci_get_debug_dump(etdev->kci,
					 etdev->debug_dump_mem.tpu_addr,
					 etdev->debug_dump_mem.size);
	etdev_dbg(etdev, "Sent debug dump request, tpu addr: %llx",
		  (u64)etdev->debug_dump_mem.tpu_addr);
	if (ret)
		etdev_err(etdev, "KCI dump info req failed: %d", ret);

	return ret;
}

static void edgetpu_reset_debug_dump(struct edgetpu_dev *etdev)
{
	memset(etdev->debug_dump_mem.vaddr, 0, etdev->debug_dump_mem.size);
}

static void edgetpu_debug_dump_work(struct work_struct *work)
{
	struct edgetpu_dev *etdev;
	struct edgetpu_debug_dump_setup *dump_setup;
	struct edgetpu_debug_dump *debug_dump;
	int ret;
	u64 dump_reason;

	etdev = container_of(work, struct edgetpu_dev, debug_dump_work);
	dump_setup =
		(struct edgetpu_debug_dump_setup *)etdev->debug_dump_mem.vaddr;
	debug_dump = (struct edgetpu_debug_dump *)(dump_setup + 1);

	if (!etdev->debug_dump_handlers) {
		etdev_err(etdev, "Failed to generate coredump as handler is NULL");
		edgetpu_reset_debug_dump(etdev);
		return;
	}

	dump_reason = debug_dump->dump_reason;
	if (dump_reason >= DUMP_REASON_NUM ||
	    !etdev->debug_dump_handlers[dump_reason]) {
		etdev_err(etdev,
			  "Failed to generate coredump as handler is NULL for dump request reason: %#llx",
			  dump_reason);
		edgetpu_reset_debug_dump(etdev);
		return;
	}

	ret = etdev->debug_dump_handlers[dump_reason]((void *)etdev, (void *)dump_setup);
	if (ret)
		etdev_err(etdev, "Failed to generate coredump: %d\n", ret);
	edgetpu_reset_debug_dump(etdev);
}

void edgetpu_debug_dump_resp_handler(struct edgetpu_dev *etdev)
{
	struct edgetpu_debug_dump_setup *dump_setup;
	struct edgetpu_debug_dump *debug_dump;

	if (!etdev->debug_dump_mem.vaddr) {
		etdev_err(etdev, "Debug dump memory not allocated");
		return;
	}
	dump_setup =
		(struct edgetpu_debug_dump_setup *)etdev->debug_dump_mem.vaddr;
	debug_dump = (struct edgetpu_debug_dump *)(dump_setup + 1);
	if (!debug_dump->host_dump_available_to_read)
		return;

	debug_dump->host_dump_available_to_read = false;

	if (!etdev->debug_dump_work.func)
		INIT_WORK(&etdev->debug_dump_work, edgetpu_debug_dump_work);

	schedule_work(&etdev->debug_dump_work);
}
