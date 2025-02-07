/*
 * This file is part of the UWB stack for linux.
 *
 * Copyright (c) 2020-2021 Qorvo US, Inc.
 *
 * This software is provided under the GNU General Public License, version 2
 * (GPLv2), as well as under a Qorvo commercial license.
 *
 * You may choose to use this software under the terms of the GPLv2 License,
 * version 2 ("GPLv2"), as published by the Free Software Foundation.
 * You should have received a copy of the GPLv2 along with this program.  If
 * not, see <http://www.gnu.org/licenses/>.
 *
 * This program is distributed under the GPLv2 in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GPLv2 for more
 * details.
 *
 * If you cannot meet the requirements of the GPLv2, you may not use this
 * software for any purpose without first obtaining a commercial license from
 * Qorvo. Please contact Qorvo to inquire about licensing terms.
 */

#ifndef FIRA_REGION_NL_H
#define FIRA_REGION_NL_H

/**
 * enum fira_call - Fira calls identifiers.
 *
 * @FIRA_CALL_GET_CAPABILITIES:
 *	Request Fira capabilities.
 * @FIRA_CALL_SESSION_INIT:
 *	Initialize Fira session.
 * @FIRA_CALL_SESSION_START:
 *	Start Fira session.
 * @FIRA_CALL_SESSION_STOP:
 *	Stop Fira session.
 * @FIRA_CALL_SESSION_DEINIT:
 *	Deinit Fira session.
 * @FIRA_CALL_SESSION_SET_PARAMS:
 *	Set session parameters.
 * @FIRA_CALL_NEW_CONTROLEE:
 *	Add a new controlee to a session.
 * @FIRA_CALL_DEL_CONTROLEE:
 *	Delete controlee from the session.
 * @FIRA_CALL_SESSION_NOTIFICATION:
 *	Notify session reports.
 * @FIRA_CALL_SESSION_GET_PARAMS:
 *	Get session parameters.
 * @FIRA_CALL_SESSION_GET_STATE:
 *	Get session state.
 * @FIRA_CALL_SESSION_GET_COUNT:
 *	Get count of active and inactive sessions.
 * @FIRA_CALL_MAX: Internal use.
 */
enum fira_call {
	FIRA_CALL_GET_CAPABILITIES,
	FIRA_CALL_SESSION_INIT,
	FIRA_CALL_SESSION_START,
	FIRA_CALL_SESSION_STOP,
	FIRA_CALL_SESSION_DEINIT,
	FIRA_CALL_SESSION_SET_PARAMS,
	FIRA_CALL_NEW_CONTROLEE,
	FIRA_CALL_DEL_CONTROLEE,
	FIRA_CALL_SESSION_NOTIFICATION,
	FIRA_CALL_SESSION_GET_PARAMS,
	FIRA_CALL_SESSION_GET_STATE,
	FIRA_CALL_SESSION_GET_COUNT,
	FIRA_CALL_SESSION_STATE_NOTIFICATION,
	FIRA_CALL_MAX,
};

/**
 * enum fira_capability_attrs - Fira capabilities.
 *
 * @FIRA_CAPABILITY_ATTR_FIRA_PHY_VERSION_RANGE:
 *	FiRa PHY version range supported, ex: 0x01010202 -> support from 1.1 to 2.2.
 * @FIRA_CAPABILITY_ATTR_FIRA_MAC_VERSION_RANGE:
 *	FiRa MAC version range supported, ex: @0x01010202 -> support from 1.1 to 2.2.
 * @FIRA_CAPABILITY_ATTR_DEVICE_CLASS:
 *	Class of FiRa device.
 * @FIRA_CAPABILITY_ATTR_DEVICE_TYPE_CONTROLEE_RESPONDER:
 *	Acting as a controlee/responder supported.
 * @FIRA_CAPABILITY_ATTR_DEVICE_TYPE_CONTROLEE_INITIATOR:
 *	Acting as a controlee/initiator supported.
 * @FIRA_CAPABILITY_ATTR_DEVICE_TYPE_CONTROLLER_RESPONDER:
 *	Acting as a controller/responder supported.
 * @FIRA_CAPABILITY_ATTR_DEVICE_TYPE_CONTROLLER_INITIATOR:
 *	Acting as a controller/initiator supported.
 * @FIRA_CAPABILITY_ATTR_MULTI_NODE_MODE_UNICAST:
 *	Unicast supported.
 * @FIRA_CAPABILITY_ATTR_MULTI_NODE_MODE_ONE_TO_MANY:
 *	One to many supported.
 * @FIRA_CAPABILITY_ATTR_RANGING_ROUND_USAGE_SS_TWR:
 *	SS-TWR supported.
 * @FIRA_CAPABILITY_ATTR_RANGING_ROUND_USAGE_DS_TWR:
 *	DS-TWR supported.
 * @FIRA_CAPABILITY_ATTR_NUMBER_OF_CONTROLEES_MAX:
 *	Maximum number of controlee, no limit if not present (but the size of
 *	the control message is a limit).
 * @FIRA_CAPABILITY_ATTR_ROUND_HOPPING:
 *	Round hopping supported.
 * @FIRA_CAPABILITY_ATTR_BLOCK_STRIDING:
 *	Block striding supported.
 * @FIRA_CAPABILITY_ATTR_EMBEDDED_MODE_NON_DEFERRED:
 *	Non-deferred mode supported.
 * @FIRA_CAPABILITY_ATTR_CHANNEL_NUMBER:
 *	Bitmask, with bits 5, 6, 8, 9, 10, 12, 13 and 14 representing
 *	the corresponding channel support.
 * @FIRA_CAPABILITY_ATTR_RFRAME_CONFIG_SP0:
 *	SP0 supported.
 * @FIRA_CAPABILITY_ATTR_RFRAME_CONFIG_SP1:
 *	SP1 supported.
 * @FIRA_CAPABILITY_ATTR_RFRAME_CONFIG_SP3:
 *	SP3 supported.
 * @FIRA_CAPABILITY_ATTR_PRF_MODE_BPRF:
 *	BPRF supported.
 * @FIRA_CAPABILITY_ATTR_PRF_MODE_HPRF:
 *	HPRF supported.
 * @FIRA_CAPABILITY_ATTR_PREAMBLE_DURATION_64:
 *	Preamble duration 64 symbols supported.
 * @FIRA_CAPABILITY_ATTR_PREAMBLE_DURATION_32:
 *	Preamble duration 32 symbols supported.
 * @FIRA_CAPABILITY_ATTR_SFD_ID_0:
 *	SFD 0 supported.
 * @FIRA_CAPABILITY_ATTR_SFD_ID_1:
 *	SFD 1 supported.
 * @FIRA_CAPABILITY_ATTR_SFD_ID_2:
 *	SFD 2 supported.
 * @FIRA_CAPABILITY_ATTR_SFD_ID_3:
 *	SFD 3 supported.
 * @FIRA_CAPABILITY_ATTR_SFD_ID_4:
 *	SFD 4 supported.
 * @FIRA_CAPABILITY_ATTR_NUMBER_OF_STS_SEGMENTS_0:
 *	0 segment for STS supported (SP0).
 * @FIRA_CAPABILITY_ATTR_NUMBER_OF_STS_SEGMENTS_1:
 *	1 segment for STS supported.
 * @FIRA_CAPABILITY_ATTR_NUMBER_OF_STS_SEGMENTS_2:
 *	2 segments for STS supported.
 * @FIRA_CAPABILITY_ATTR_PSDU_DATA_RATE_6M81:
 *	6.81 Mbps support.
 * @FIRA_CAPABILITY_ATTR_PSDU_DATA_RATE_7M80:
 *	7.80 Mbps support.
 * @FIRA_CAPABILITY_ATTR_PSDU_DATA_RATE_27M2:
 *	27.2 Mbps support.
 * @FIRA_CAPABILITY_ATTR_PSDU_DATA_RATE_31M2:
 *	31.2 Mbps support.
 * @FIRA_CAPABILITY_ATTR_BPRF_PHR_DATA_RATE_850K:
 *	850 kbps for PHR in BPRF supported.
 * @FIRA_CAPABILITY_ATTR_BPRF_PHR_DATA_RATE_6M81:
 *	6.81 Mbps for PHR in BPRF supported.
 * @FIRA_CAPABILITY_ATTR_MAC_FCS_TYPE_CRC32:
 *	CRC32 supported.
 * @FIRA_CAPABILITY_ATTR_TX_ADAPTIVE_PAYLOAD_POWER:
 *	Adaptive payload power for TX supported.
 * @FIRA_CAPABILITY_ATTR_RX_ANTENNA_PAIRS:
 *	Number of antenna pairs for RX.
 * @FIRA_CAPABILITY_ATTR_TX_ANTENNAS:
 *	Number of antennas for TX.
 * @FIRA_CAPABILITY_ATTR_STS_CONFIG_STATIC:
 *	Static STS supported.
 * @FIRA_CAPABILITY_ATTR_STS_CONFIG_DYNAMIC:
 *	Dynamic STS supported.
 * @FIRA_CAPABILITY_ATTR_STS_CONFIG_DYNAMIC_INDIVIDUAL:
 *	Dynamic STS for controlee individual keys supported.
 * @FIRA_CAPABILITY_ATTR_AOA_AZIMUTH:
 *	AoA in azimuth supported.
 * @FIRA_CAPABILITY_ATTR_AOA_AZIMUTH_FULL:
 *	AoA in azimuth supported on full circle (front/back difference).
 * @FIRA_CAPABILITY_ATTR_AOA_ELEVATION:
 *	AoA in elevation supported.
 * @FIRA_CAPABILITY_ATTR_AOA_FOM:
 *	AoA FOM supported.
 *
 * @FIRA_CAPABILITY_ATTR_UNSPEC: Invalid command.
 * @__FIRA_CAPABILITY_ATTR_AFTER_LAST: Internal use.
 * @FIRA_CAPABILITY_ATTR_MAX: Internal use.
 */
enum fira_capability_attrs {
	FIRA_CAPABILITY_ATTR_UNSPEC,
	/* Main session capabilities. */
	FIRA_CAPABILITY_ATTR_FIRA_PHY_VERSION_RANGE,
	FIRA_CAPABILITY_ATTR_FIRA_MAC_VERSION_RANGE,
	FIRA_CAPABILITY_ATTR_DEVICE_CLASS,
	FIRA_CAPABILITY_ATTR_DEVICE_TYPE_CONTROLEE_RESPONDER,
	FIRA_CAPABILITY_ATTR_DEVICE_TYPE_CONTROLEE_INITIATOR,
	FIRA_CAPABILITY_ATTR_DEVICE_TYPE_CONTROLLER_RESPONDER,
	FIRA_CAPABILITY_ATTR_DEVICE_TYPE_CONTROLLER_INITIATOR,
	FIRA_CAPABILITY_ATTR_MULTI_NODE_MODE_UNICAST,
	FIRA_CAPABILITY_ATTR_MULTI_NODE_MODE_ONE_TO_MANY,
	FIRA_CAPABILITY_ATTR_RANGING_ROUND_USAGE_SS_TWR,
	FIRA_CAPABILITY_ATTR_RANGING_ROUND_USAGE_DS_TWR,
	FIRA_CAPABILITY_ATTR_NUMBER_OF_CONTROLEES_MAX,
	/* Behaviour. */
	FIRA_CAPABILITY_ATTR_ROUND_HOPPING,
	FIRA_CAPABILITY_ATTR_BLOCK_STRIDING,
	FIRA_CAPABILITY_ATTR_EMBEDDED_MODE_NON_DEFERRED,
	/* Radio. */
	FIRA_CAPABILITY_ATTR_CHANNEL_NUMBER,
	FIRA_CAPABILITY_ATTR_RFRAME_CONFIG_SP0,
	FIRA_CAPABILITY_ATTR_RFRAME_CONFIG_SP1,
	FIRA_CAPABILITY_ATTR_RFRAME_CONFIG_SP3,
	FIRA_CAPABILITY_ATTR_PRF_MODE_BPRF,
	FIRA_CAPABILITY_ATTR_PRF_MODE_HPRF,
	FIRA_CAPABILITY_ATTR_PREAMBLE_DURATION_64,
	FIRA_CAPABILITY_ATTR_PREAMBLE_DURATION_32,
	FIRA_CAPABILITY_ATTR_SFD_ID_0,
	FIRA_CAPABILITY_ATTR_SFD_ID_1,
	FIRA_CAPABILITY_ATTR_SFD_ID_2,
	FIRA_CAPABILITY_ATTR_SFD_ID_3,
	FIRA_CAPABILITY_ATTR_SFD_ID_4,
	FIRA_CAPABILITY_ATTR_NUMBER_OF_STS_SEGMENTS_0,
	FIRA_CAPABILITY_ATTR_NUMBER_OF_STS_SEGMENTS_1,
	FIRA_CAPABILITY_ATTR_NUMBER_OF_STS_SEGMENTS_2,
	FIRA_CAPABILITY_ATTR_PSDU_DATA_RATE_6M81,
	FIRA_CAPABILITY_ATTR_PSDU_DATA_RATE_7M80,
	FIRA_CAPABILITY_ATTR_PSDU_DATA_RATE_27M2,
	FIRA_CAPABILITY_ATTR_PSDU_DATA_RATE_31M2,
	FIRA_CAPABILITY_ATTR_BPRF_PHR_DATA_RATE_850K,
	FIRA_CAPABILITY_ATTR_BPRF_PHR_DATA_RATE_6M81,
	FIRA_CAPABILITY_ATTR_MAC_FCS_TYPE_CRC32,
	FIRA_CAPABILITY_ATTR_TX_ADAPTIVE_PAYLOAD_POWER,
	/* Antenna. */
	FIRA_CAPABILITY_ATTR_RX_ANTENNA_PAIRS,
	FIRA_CAPABILITY_ATTR_TX_ANTENNAS,
	/* STS and crypto capabilities. */
	FIRA_CAPABILITY_ATTR_STS_CONFIG_STATIC,
	FIRA_CAPABILITY_ATTR_STS_CONFIG_DYNAMIC,
	FIRA_CAPABILITY_ATTR_STS_CONFIG_DYNAMIC_INDIVIDUAL,
	/* Report. */
	FIRA_CAPABILITY_ATTR_AOA_AZIMUTH,
	FIRA_CAPABILITY_ATTR_AOA_AZIMUTH_FULL,
	FIRA_CAPABILITY_ATTR_AOA_ELEVATION,
	FIRA_CAPABILITY_ATTR_AOA_FOM,

	__FIRA_CAPABILITY_ATTR_AFTER_LAST,
	FIRA_CAPABILITY_ATTR_MAX = __FIRA_CAPABILITY_ATTR_AFTER_LAST - 1
};

/**
 * enum fira_call_attrs - Fira call attributes.
 *
 * @FIRA_CALL_ATTR_SESSION_ID:
 *	Session identifier.
 * @FIRA_CALL_ATTR_SESSION_PARAMS:
 *	Session parameters.
 * @FIRA_CALL_ATTR_CONTROLEES:
 *	Controlees information.
 * @FIRA_CALL_ATTR_RANGING_DATA:
 *	Ranging data.
 *
 * @FIRA_CALL_ATTR_UNSPEC: Invalid command.
 * @__FIRA_CALL_ATTR_AFTER_LAST: Internal use.
 * @FIRA_CALL_ATTR_MAX: Internal use.
 */
enum fira_call_attrs {
	FIRA_CALL_ATTR_UNSPEC,
	FIRA_CALL_ATTR_SESSION_ID,
	FIRA_CALL_ATTR_SESSION_PARAMS,
	FIRA_CALL_ATTR_CONTROLEES,
	FIRA_CALL_ATTR_RANGING_DATA,
	FIRA_CALL_ATTR_CAPABILITIES,
	FIRA_CALL_ATTR_SESSION_STATE,
	FIRA_CALL_ATTR_SESSION_COUNT,

	__FIRA_CALL_ATTR_AFTER_LAST,
	FIRA_CALL_ATTR_MAX = __FIRA_CALL_ATTR_AFTER_LAST - 1
};

/**
 * enum fira_session_param_attrs - Fira session parameters attributes.
 *
 * @FIRA_SESSION_PARAM_ATTR_DEVICE_TYPE:
 *	Controlee (0) or controller (1)
 * @FIRA_SESSION_PARAM_ATTR_DEVICE_ROLE:
 *	Responder (0) or initiator (1)
 * @FIRA_SESSION_PARAM_ATTR_RANGING_ROUND_USAGE:
 *	OWR (0, unused, not in FiRa 1.1), SS-TWR (1) or DS-TWR (2, default)
 * @FIRA_SESSION_PARAM_ATTR_MULTI_NODE_MODE:
 *	Unicast (0), one to many (1) or many to many (2, unused)
 * @FIRA_SESSION_PARAM_ATTR_SHORT_ADDR:
 *	Override device address for this session (UCI: DEVICE_MAC_ADDRESS)
 * @FIRA_SESSION_PARAM_ATTR_DESTINATION_SHORT_ADDR:
 *	Controller short addresses (UCI: DST_MAC_ADDRESS) [controlee only]
 * @FIRA_SESSION_PARAM_ATTR_INITIATION_TIME_MS:
 *	Initiation time in unit of 1200 RSTU (same as ms), default 0.
 * @FIRA_SESSION_PARAM_ATTR_SLOT_DURATION_RSTU:
 *	Duration of a slot in RSTU, default 2400. (2 ms)
 * @FIRA_SESSION_PARAM_ATTR_BLOCK_DURATION_MS:
 *	Block size in unit of 1200 RSTU (same as ms), default 200.
 * @FIRA_SESSION_PARAM_ATTR_ROUND_DURATION_SLOTS:
 *	Number of slots per ranging round, default 30 (UCI: SLOTS_PER_RR)
 * @FIRA_SESSION_PARAM_ATTR_BLOCK_STRIDE_LENGTH:
 *	Value of block striding, default 0, can be changed when the
 *	session is active [controller only]
 * @FIRA_SESSION_PARAM_ATTR_MAX_RR_RETRY:
 *	Number of failed ranging round attempts before stopping the session,
 *	or disabled (0, default) [controller only]
 * @FIRA_SESSION_PARAM_ATTR_ROUND_HOPPING:
 *	Disabled (0, default) or enabled (1)
 * @FIRA_SESSION_PARAM_ATTR_PRIORITY:
 *	Priority value, higher has more priority (1-100, default 50)
 * @FIRA_SESSION_PARAM_ATTR_RESULT_REPORT_PHASE:
 *	Disabled (0) or enabled (1, default) report phase [controller only]
 * @FIRA_SESSION_PARAM_ATTR_MAX_NUMBER_OF_MEASUREMENTS:
 *      Number of measurements
 * @FIRA_SESSION_PARAM_ATTR_MR_AT_INITIATOR:
 *	Measurement report available at responder (0, default)
 *	or at initiator (1) [controller only]
 * @FIRA_SESSION_PARAM_ATTR_EMBEDDED_MODE:
 *	Deferred (0, default) or non-deferred (1) (not in UCI)
 * @FIRA_SESSION_PARAM_ATTR_IN_BAND_TERMINATION_ATTEMPT_COUNT:
 *	1-10, default 1 [controller only]
 * @FIRA_SESSION_PARAM_ATTR_CHANNEL_NUMBER:
 *	Override channel for this session: 5, 6, 8, 9, 10, 12, 13 or 14
 * @FIRA_SESSION_PARAM_ATTR_PREAMBLE_CODE_INDEX:
 *	Override preamble code for this session, BPRF (9-24),
 *	HPRF (25-32, not supported)
 * @FIRA_SESSION_PARAM_ATTR_RFRAME_CONFIG:
 *	SP0 (0), SP1 (1), SP2 (2, unused, not in FiRa 1.1) or SP3 (3, default)
 * @FIRA_SESSION_PARAM_ATTR_PRF_MODE:
 *	BPRF (0, default) or HPRF (1, not supported)
 * @FIRA_SESSION_PARAM_ATTR_PREAMBLE_DURATION:
 *	64 (1, default) or 32 (0, only for HPRF)
 * @FIRA_SESSION_PARAM_ATTR_SFD_ID:
 *	BPRF (0 or 2), HPRF (1-4, not supported), default 2
 * @FIRA_SESSION_PARAM_ATTR_NUMBER_OF_STS_SEGMENTS:
 *	0-2, default to 0 for SP0, default to 1 for SP1 & SP3, 2 not supported
 * @FIRA_SESSION_PARAM_ATTR_PSDU_DATA_RATE:
 *	6.81 Mbps (0, default), 7.80 Mbps (1, not supported),
 *	27.2 Mbps (2, not supported), 31.2 Mbps (3, not supported)
 * @FIRA_SESSION_PARAM_ATTR_BPRF_PHR_DATA_RATE:
 *	850 kbps (0, default) or 6.81 Mbps (1)
 * @FIRA_SESSION_PARAM_ATTR_MAC_FCS_TYPE:
 *	CRC16 (0, default) or CRC32 (1, not supported)
 * @FIRA_SESSION_PARAM_ATTR_TX_ADAPTIVE_PAYLOAD_POWER:
 *	Disable adaptive payload power for TX (0, default) or enable (1)
 * @FIRA_SESSION_PARAM_ATTR_RX_ANTENNA_SELECTION:
 *	Reception antenna selection bitmask,
 *	bit 0 selects first antenna pair, etc. 0 for single RX (default)
 * @FIRA_SESSION_PARAM_ATTR_RX_ANTENNA_PAIR_AZIMUTH:
 *	Index of the antenna pair used for azimuth angle measure,
 *	or 0xff if none (default)
 * @FIRA_SESSION_PARAM_ATTR_RX_ANTENNA_PAIR_ELEVATION:
 *	Index of the antenna pair used for elevation angle measure,
 *	or 0xff if none (default)
 * @FIRA_SESSION_PARAM_ATTR_TX_ANTENNA_SELECTION:
 *	Tx antenna selection bitmask, bit 0 selects antenna 0, etc. Default to 1,
 *	switch between ranging rounds
 * @FIRA_SESSION_PARAM_ATTR_RX_ANTENNA_SWITCH:
 *	If several antenna pairs are selected, switch pairs between round (0, default),
 *	use azimuth pair for POLL and elevation pair for FINAL messages when using
 *	DS-TWR as a responder (1),
 *	or, when controller, request two rangings per peer, use azimuth for first
 *	ranging and elevation for second ranging (2)
 * @FIRA_SESSION_PARAM_ATTR_STS_CONFIG:
 *	Static STS (0, default), dynamic STS (1) or dynamic STS for controlee
 *	individual keys (2)
 * @FIRA_SESSION_PARAM_ATTR_SUB_SESSION_ID:
 *	For dynamic STS for controlee individual key, sub session ID [controlee only]
 * @FIRA_SESSION_PARAM_ATTR_VUPPER64:
 *	vUpper64 for static STS (UCI: STATIC_STS_IV | VENDOR_ID)
 * @FIRA_SESSION_PARAM_ATTR_SESSION_KEY:
 *	For dynamic STS, session key (not in UCI)
 * @FIRA_SESSION_PARAM_ATTR_SUB_SESSION_KEY:
 *	For dynamic STS for controlee individual keys, sub session key [controlee only]
 * @FIRA_SESSION_PARAM_ATTR_KEY_ROTATION:
 *	Disable (0, default) or enabled (1)
 * @FIRA_SESSION_PARAM_ATTR_KEY_ROTATION_RATE:
 *	Key rotation rate parameter n, key rotated after 2^n rounds
 * @FIRA_SESSION_PARAM_ATTR_AOA_RESULT_REQ:
 *	No local AoA report (0) or -90 to +90 (1, default)
 * @FIRA_SESSION_PARAM_ATTR_REPORT_TOF:
 *	Report ToF in result message, disabled (0) or enabled (1, default)
 * @FIRA_SESSION_PARAM_ATTR_REPORT_AOA_AZIMUTH:
 *	Report AoA azimuth in result message, disabled (0, default) or enabled (1)
 * @FIRA_SESSION_PARAM_ATTR_REPORT_AOA_ELEVATION:
 *	Report AoA elevation in result message, disabled (0, default) or enabled (1)
 * @FIRA_SESSION_PARAM_ATTR_REPORT_AOA_FOM:
 *	Report AoA FOM in result message, disabled (0, default) or enabled (1)
 * @FIRA_SESSION_PARAM_ATTR_DATA_VENDOR_OUI:
 *	Set the vendor OUI for custom data exchanges
 * @FIRA_SESSION_PARAM_ATTR_DATA_PAYLOAD:
 *	Set the data payload to send in next ranging packet
 *
 * @FIRA_SESSION_PARAM_ATTR_UNSPEC: Invalid command.
 * @__FIRA_SESSION_PARAM_ATTR_AFTER_LAST: Internal use.
 * @FIRA_SESSION_PARAM_ATTR_MAX: Internal use.
 */
enum fira_session_param_attrs {
	FIRA_SESSION_PARAM_ATTR_UNSPEC,
	/* Main session parameters */
	FIRA_SESSION_PARAM_ATTR_DEVICE_TYPE,
	FIRA_SESSION_PARAM_ATTR_DEVICE_ROLE,
	FIRA_SESSION_PARAM_ATTR_RANGING_ROUND_USAGE,
	FIRA_SESSION_PARAM_ATTR_MULTI_NODE_MODE,
	FIRA_SESSION_PARAM_ATTR_SHORT_ADDR,
	FIRA_SESSION_PARAM_ATTR_DESTINATION_SHORT_ADDR,
	/* Timings */
	FIRA_SESSION_PARAM_ATTR_INITIATION_TIME_MS,
	FIRA_SESSION_PARAM_ATTR_SLOT_DURATION_RSTU,
	FIRA_SESSION_PARAM_ATTR_BLOCK_DURATION_MS,
	FIRA_SESSION_PARAM_ATTR_ROUND_DURATION_SLOTS,
	FIRA_SESSION_PARAM_ATTR_BLOCK_STRIDE_LENGTH,
	/* Behaviour */
	FIRA_SESSION_PARAM_ATTR_MAX_RR_RETRY,
	FIRA_SESSION_PARAM_ATTR_ROUND_HOPPING,
	FIRA_SESSION_PARAM_ATTR_PRIORITY,
	FIRA_SESSION_PARAM_ATTR_RESULT_REPORT_PHASE,
	FIRA_SESSION_PARAM_ATTR_MR_AT_INITIATOR,
	FIRA_SESSION_PARAM_ATTR_EMBEDDED_MODE,
	FIRA_SESSION_PARAM_ATTR_IN_BAND_TERMINATION_ATTEMPT_COUNT,
	/* Radio */
	FIRA_SESSION_PARAM_ATTR_CHANNEL_NUMBER,
	FIRA_SESSION_PARAM_ATTR_PREAMBLE_CODE_INDEX,
	FIRA_SESSION_PARAM_ATTR_RFRAME_CONFIG,
	FIRA_SESSION_PARAM_ATTR_PRF_MODE,
	FIRA_SESSION_PARAM_ATTR_PREAMBLE_DURATION,
	FIRA_SESSION_PARAM_ATTR_SFD_ID,
	FIRA_SESSION_PARAM_ATTR_NUMBER_OF_STS_SEGMENTS,
	FIRA_SESSION_PARAM_ATTR_PSDU_DATA_RATE,
	FIRA_SESSION_PARAM_ATTR_BPRF_PHR_DATA_RATE,
	FIRA_SESSION_PARAM_ATTR_MAC_FCS_TYPE,
	FIRA_SESSION_PARAM_ATTR_TX_ADAPTIVE_PAYLOAD_POWER,
	/* Antenna */
	FIRA_SESSION_PARAM_ATTR_RX_ANTENNA_SELECTION,
	FIRA_SESSION_PARAM_ATTR_RX_ANTENNA_PAIR_AZIMUTH,
	FIRA_SESSION_PARAM_ATTR_RX_ANTENNA_PAIR_ELEVATION,
	FIRA_SESSION_PARAM_ATTR_TX_ANTENNA_SELECTION,
	FIRA_SESSION_PARAM_ATTR_RX_ANTENNA_SWITCH,
	/* STS and crypto */
	FIRA_SESSION_PARAM_ATTR_STS_CONFIG,
	FIRA_SESSION_PARAM_ATTR_SUB_SESSION_ID,
	FIRA_SESSION_PARAM_ATTR_VUPPER64,
	FIRA_SESSION_PARAM_ATTR_SESSION_KEY,
	FIRA_SESSION_PARAM_ATTR_SUB_SESSION_KEY,
	FIRA_SESSION_PARAM_ATTR_KEY_ROTATION,
	FIRA_SESSION_PARAM_ATTR_KEY_ROTATION_RATE,
	/* Report */
	FIRA_SESSION_PARAM_ATTR_AOA_RESULT_REQ,
	FIRA_SESSION_PARAM_ATTR_REPORT_TOF,
	FIRA_SESSION_PARAM_ATTR_REPORT_AOA_AZIMUTH,
	FIRA_SESSION_PARAM_ATTR_REPORT_AOA_ELEVATION,
	FIRA_SESSION_PARAM_ATTR_REPORT_AOA_FOM,
	/* Custom Data */
	FIRA_SESSION_PARAM_ATTR_DATA_VENDOR_OUI,
	FIRA_SESSION_PARAM_ATTR_DATA_PAYLOAD,

	FIRA_SESSION_PARAM_ATTR_MAX_NUMBER_OF_MEASUREMENTS,

	__FIRA_SESSION_PARAM_ATTR_AFTER_LAST,
	FIRA_SESSION_PARAM_ATTR_MAX = __FIRA_SESSION_PARAM_ATTR_AFTER_LAST - 1
};

/**
 * enum fira_call_controlee_attrs - Fira session parameters attributes.
 *
 * @FIRA_CALL_CONTROLEE_ATTR_SHORT_ADDR:
 *	Controlee short address.
 * @FIRA_CALL_CONTROLEE_ATTR_SUB_SESSION_ID:
 *	Controlee sub session identifier
 * @FIRA_CALL_CONTROLEE_ATTR_SUB_SESSION_KEY:
 *	Controlee sub session key
 *
 * @FIRA_CALL_CONTROLEE_ATTR_UNSPEC: Invalid command.
 * @__FIRA_CALL_CONTROLEE_ATTR_AFTER_LAST: Internal use.
 * @FIRA_CALL_CONTROLEE_ATTR_MAX: Internal use.
 */
enum fira_call_controlee_attrs {
	FIRA_CALL_CONTROLEE_ATTR_UNSPEC,
	FIRA_CALL_CONTROLEE_ATTR_SHORT_ADDR,
	FIRA_CALL_CONTROLEE_ATTR_SUB_SESSION_ID,
	FIRA_CALL_CONTROLEE_ATTR_SUB_SESSION_KEY,

	__FIRA_CALL_CONTROLEE_ATTR_AFTER_LAST,
	FIRA_CALL_CONTROLEE_ATTR_MAX = __FIRA_CALL_CONTROLEE_ATTR_AFTER_LAST - 1
};

/**
 * enum fira_ranging_data_attrs_stopped_values - Values for
 * FIRA_RANGING_DATA_ATTR_STOPPED attribute.
 *
 * @FIRA_RANGING_DATA_ATTR_STOPPED_REQUEST:
 *	Stopped due to stop request.
 * @FIRA_RANGING_DATA_ATTR_STOPPED_IN_BAND:
 *	Stopped using in band signaling from the controller [controlee only].
 * @FIRA_RANGING_DATA_ATTR_STOPPED_NO_RESPONSE:
 *	Stopped due to maximum attempts reached with no response [controller
 *	only].
 */
enum fira_ranging_data_attrs_stopped_values {
	FIRA_RANGING_DATA_ATTR_STOPPED_REQUEST,
	FIRA_RANGING_DATA_ATTR_STOPPED_IN_BAND,
	FIRA_RANGING_DATA_ATTR_STOPPED_NO_RESPONSE,
};

/**
 * enum fira_ranging_data_attrs - Fira ranging data attributes.
 *
 * @FIRA_RANGING_DATA_ATTR_STOPPED:
 *	If present, session was stopped, see
 *	&enum fira_ranging_data_attrs_stopped_values.
 * @FIRA_RANGING_DATA_ATTR_BLOCK_INDEX:
 *	Current block index.
 * @FIRA_RANGING_DATA_ATTR_TIMESTAMP_NS:
 *	Timestamp in nanoseconds in the CLOCK_MONOTONIC time reference.
 * @FIRA_RANGING_DATA_ATTR_RANGING_INTERVAL_MS:
 *	Current ranging interval (block size * (stride + 1)) in unit of
 *	1200 RSTU (same as ms).
 * @FIRA_RANGING_DATA_ATTR_MEASUREMENTS:
 *	Measurements, see fira_ranging_data_measurements.
 *
 * @FIRA_RANGING_DATA_ATTR_UNSPEC: Invalid command.
 * @__FIRA_RANGING_DATA_ATTR_AFTER_LAST: Internal use.
 * @FIRA_RANGING_DATA_ATTR_MAX: Internal use.
 */
enum fira_ranging_data_attrs {
	FIRA_RANGING_DATA_ATTR_UNSPEC,
	FIRA_RANGING_DATA_ATTR_STOPPED,
	FIRA_RANGING_DATA_ATTR_BLOCK_INDEX,
	FIRA_RANGING_DATA_ATTR_TIMESTAMP_NS,
	FIRA_RANGING_DATA_ATTR_RANGING_INTERVAL_MS,
	FIRA_RANGING_DATA_ATTR_MEASUREMENTS,

	__FIRA_RANGING_DATA_ATTR_AFTER_LAST,
	FIRA_RANGING_DATA_ATTR_MAX = __FIRA_RANGING_DATA_ATTR_AFTER_LAST - 1
};

/**
 * enum fira_ranging_data_measurements_attrs - Fira ranging data measurements
 * attributes.
 *
 * @FIRA_RANGING_DATA_MEASUREMENTS_ATTR_SHORT_ADDR:
 *	Address of the participing device.
 * @FIRA_RANGING_DATA_MEASUREMENTS_ATTR_STOPPED:
 *	If present, ranging was stopped as requested [controller only].
 * @FIRA_RANGING_DATA_MEASUREMENTS_ATTR_STATUS:
 *	Status of the measurement (0: success, 1: tx failed, 2: rx timeout,
 *	3: rx phy dec error, 4: rx phy toa error, 5: rx phy sts error,
 *	6: rx mac dec error, 7: rx mac ie dec error, 8: rx mac ie missing)
 * @FIRA_RANGING_DATA_MEASUREMENTS_ATTR_SLOT_INDEX:
 *	In case of failure slot index where the error was detected.
 * @FIRA_RANGING_DATA_MEASUREMENTS_ATTR_NLOS:
 *	Set if in non line of sight.
 * @FIRA_RANGING_DATA_MEASUREMENTS_ATTR_LOS:
 *	Set if in line of sight.
 * @FIRA_RANGING_DATA_MEASUREMENTS_ATTR_DISTANCE_MM:
 *	Distance in mm.
 * @FIRA_RANGING_DATA_MEASUREMENTS_ATTR_LOCAL_AOA:
 *	Local AoA measurement,
 *	cf. fira_ranging_data_measurements_aoa_attrs.
 * @FIRA_RANGING_DATA_MEASUREMENTS_ATTR_LOCAL_AOA_AZIMUTH:
 *	Local AoA measurement for azimuth,
 *	cf. fira_ranging_data_measurements_aoa_attrs.
 * @FIRA_RANGING_DATA_MEASUREMENTS_ATTR_LOCAL_AOA_ELEVATION:
 *	Local AoA measurement for elevation,
 *	cf. fira_ranging_data_measurements_aoa_attrs.
 * @FIRA_RANGING_DATA_MEASUREMENTS_ATTR_REMOTE_AOA_AZIMUTH_2PI:
 *	Estimation of reception angle in the azimuth of the participing device.
 * @FIRA_RANGING_DATA_MEASUREMENTS_ATTR_REMOTE_AOA_ELEVATION_PI:
 *	Estimation of reception angle in elevation of the participing device.
 * @FIRA_RANGING_DATA_MEASUREMENTS_ATTR_REMOTE_AOA_AZIMUTH_FOM:
 *	Estimation of azimuth reliability of the participing device.
 * @FIRA_RANGING_DATA_MEASUREMENTS_ATTR_REMOTE_AOA_ELEVATION_FOM:
 *	Estimation of elevation reliability of the participing device.
 * @FIRA_RANGING_DATA_MEASUREMENTS_ATTR_DATA_PAYLOAD_SEQ_SENT:
 *	Sequence number of last data sent
 * @FIRA_RANGING_DATA_MEASUREMENTS_ATTR_DATA_PAYLOAD_RECV:
 *	Received Data payload in the SP1 RFRAME
 *
 * @FIRA_RANGING_DATA_MEASUREMENTS_ATTR_UNSPEC: Invalid command.
 * @__FIRA_RANGING_DATA_MEASUREMENTS_ATTR_AFTER_LAST: Internal use.
 * @FIRA_RANGING_DATA_MEASUREMENTS_ATTR_MAX: Internal use.
 */
enum fira_ranging_data_measurements_attrs {
	FIRA_RANGING_DATA_MEASUREMENTS_ATTR_UNSPEC,
	FIRA_RANGING_DATA_MEASUREMENTS_ATTR_SHORT_ADDR,
	FIRA_RANGING_DATA_MEASUREMENTS_ATTR_STOPPED,
	FIRA_RANGING_DATA_MEASUREMENTS_ATTR_STATUS,
	FIRA_RANGING_DATA_MEASUREMENTS_ATTR_SLOT_INDEX,
	FIRA_RANGING_DATA_MEASUREMENTS_ATTR_NLOS,
	FIRA_RANGING_DATA_MEASUREMENTS_ATTR_LOS,
	FIRA_RANGING_DATA_MEASUREMENTS_ATTR_DISTANCE_MM,
	FIRA_RANGING_DATA_MEASUREMENTS_ATTR_LOCAL_AOA,
	FIRA_RANGING_DATA_MEASUREMENTS_ATTR_LOCAL_AOA_AZIMUTH,
	FIRA_RANGING_DATA_MEASUREMENTS_ATTR_LOCAL_AOA_ELEVATION,
	FIRA_RANGING_DATA_MEASUREMENTS_ATTR_REMOTE_AOA_AZIMUTH_2PI,
	FIRA_RANGING_DATA_MEASUREMENTS_ATTR_REMOTE_AOA_ELEVATION_PI,
	FIRA_RANGING_DATA_MEASUREMENTS_ATTR_REMOTE_AOA_AZIMUTH_FOM,
	FIRA_RANGING_DATA_MEASUREMENTS_ATTR_REMOTE_AOA_ELEVATION_FOM,
	FIRA_RANGING_DATA_MEASUREMENTS_ATTR_DATA_PAYLOAD_SEQ_SENT,
	FIRA_RANGING_DATA_MEASUREMENTS_ATTR_DATA_PAYLOAD_RECV,

	__FIRA_RANGING_DATA_MEASUREMENTS_ATTR_AFTER_LAST,
	FIRA_RANGING_DATA_MEASUREMENTS_ATTR_MAX =
		__FIRA_RANGING_DATA_MEASUREMENTS_ATTR_AFTER_LAST - 1
};

/**
 * enum fira_ranging_data_measurements_aoa_attrs - Fira ranging AoA measurements
 * attributes.
 *
 * @FIRA_RANGING_DATA_MEASUREMENTS_AOA_ATTR_RX_ANTENNA_PAIR:
 *	Antenna pair index.
 * @FIRA_RANGING_DATA_MEASUREMENTS_AOA_ATTR_AOA_2PI:
 *	Estimation of reception angle.
 * @FIRA_RANGING_DATA_MEASUREMENTS_AOA_ATTR_AOA_FOM:
 *	Estimation of local AoA reliability.
 * @FIRA_RANGING_DATA_MEASUREMENTS_AOA_ATTR_PDOA_2PI:
 *	Estimation of reception phase difference.
 *
 * @FIRA_RANGING_DATA_MEASUREMENTS_AOA_ATTR_UNSPEC: Invalid command.
 * @__FIRA_RANGING_DATA_MEASUREMENTS_AOA_ATTR_AFTER_LAST: Internal use.
 * @FIRA_RANGING_DATA_MEASUREMENTS_AOA_ATTR_MAX: Internal use.
 */
enum fira_ranging_data_measurements_aoa_attrs {
	FIRA_RANGING_DATA_MEASUREMENTS_AOA_ATTR_UNSPEC,
	FIRA_RANGING_DATA_MEASUREMENTS_AOA_ATTR_RX_ANTENNA_PAIR,
	FIRA_RANGING_DATA_MEASUREMENTS_AOA_ATTR_AOA_2PI,
	FIRA_RANGING_DATA_MEASUREMENTS_AOA_ATTR_AOA_FOM,
	FIRA_RANGING_DATA_MEASUREMENTS_AOA_ATTR_PDOA_2PI,

	__FIRA_RANGING_DATA_MEASUREMENTS_AOA_ATTR_AFTER_LAST,
	FIRA_RANGING_DATA_MEASUREMENTS_AOA_ATTR_MAX =
		__FIRA_RANGING_DATA_MEASUREMENTS_AOA_ATTR_AFTER_LAST - 1
};

#endif /* FIRA_REGION_NL_H */
