/** @file
 * Copyright (c) 2019-2020, Arm Limited or its affiliates. All rights reserved.
 * SPDX-License-Identifier : Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
**/

#ifndef SENSOR_COMMON_H_
#define SENSOR_COMMON_H_

#include <protocol_common.h>

struct arm_scmi_sensor_protocol_version {
    struct  {
        uint32_t version;
    } returns;
};

struct arm_scmi_sensor_protocol_attributes {
    struct {
        uint32_t attributes;
        uint32_t sensor_reg_address_low;
        uint32_t sensor_reg_address_high;
        uint32_t sensor_reg_len;
    } returns;
    /* all the enums must be at the end */
    enum {
        SNR_NUMBER_SENSORS_LOW = 0,
        SNR_NUMBER_SENSORS_HIGH = 15,
        SNR_MAX_NUM_PENDING_ASYNC_CMDS_SPRT_LOW = 16,
        SNR_MAX_NUM_PENDING_ASYNC_CMDS_SPRT_HIGH = 23,
        SNR_RESERVED_LOW = 24,
        SNR_RESERVED_HIGH = 31
    }attributes_bits;
};

struct arm_scmi_sensor_protocol_message_attributes {
    struct {
        uint32_t message_id;
    } parameters;
    struct {
        uint32_t attributes;
    } returns;
};

struct arm_scmi_sensor_description_get {
    struct {
        uint32_t desc_index;
    } parameters;
    struct {
        uint32_t num_sensor_flags;
        uint32_t sensor_descriptors;
    } returns;
    /*enums for num_sensor_flags*/
    enum {
        SNR_NUMBER_SNR_DESC_LOW = 0,
        SNR_NUMBER_SNR_DESC_HIGH = 11,
        SNR_NUM_SNR_FLAG_RESV_LOW = 12,
        SNR_NUM_SNR_FLAG_RESV_HIGH = 15,
        SNR_NUMBER_REMAIN_SNR_DESC_LOW = 16,
        SNR_NUMBER_REMAIN_SNR_DESC_HIGH = 31
    }num_sensor_flags_bits;
};

/*sensor descriptor struct and enums*/
struct arm_scmi_sensor_descriptor {
    uint32_t sensor_id;
    uint32_t sensor_attributes_low;
    uint32_t sensor_attributes_high;
    uint8_t sensor_name[SCMI_NAME_STR_SIZE];
    uint32_t sensor_power;
    uint32_t sensor_resolution;
    uint32_t sensor_min_range_low;
    uint32_t sensor_min_range_high;
    uint32_t sensor_max_range_low;
    uint32_t sensor_max_range_high;

    enum {
        SNR_DESC_ATTRLOW_NUM_TRIPPOINT_SPRT_LOW = 0,
        SNR_DESC_ATTRLOW_NUM_TRIPPOINT_SPRT_HIGH = 7,
        SNR_DESC_ATTRLOW_EXT_ATTR_SUPPORT = 8,
        SNR_DESC_ATTRLOW_TIMESTAMP_SUPPORT = 9,
        SNR_DESC_ATTRLOW_TIMESTAMP_EXP_LOW = 10,
        SNR_DESC_ATTRLOW_TIMESTAMP_EXP_HIGH = 14,
        SNR_DESC_ATTRLOW_RESV_LOW = 15,
        SNR_DESC_ATTRLOW_RESV_HIGH = 29,
        SNR_DESC_ATTRLOW_CNT_SNR_UPDATE_NOT = 30,
        SNR_DESC_ATTRLOW_ASYNC_READ_SUPPORT = 31
    } sensor_attributes_low_bits;

    enum {
        SNR_DESC_ATTRHIGH_SENSOR_TYPE_LOW = 0,
        SNR_DESC_ATTRHIGH_SENSOR_TYPE_HIGH = 7,
        SNR_DESC_ATTRHIGH_AXIS_SUPPORT = 8,
        SNR_DESC_ATTRHIGH_RESV_UPPER_LOW = 9,
        SNR_DESC_ATTRHIGH_RESV_UPPER_HIGH = 10,
        SNR_DESC_ATTRHIGH_EXPONENT_LOW = 11,
        SNR_DESC_ATTRHIGH_EXPONENT_HIGH = 15,
        SNR_DESC_ATTRHIGH_NUM_AXES_LOW = 16,
        SNR_DESC_ATTRHIGH_NUM_AXES_HIGH = 21,
        SNR_DESC_ATTRHIGH_RESV_LOW = 22,
        SNR_DESC_ATTRHIGH_RESV_HIGH = 31
    } sensor_attributes_high_bits;

    enum {
        SNR_RESOLUTION_SENSOR_RESOL_LOW = 0,
        SNR_RESOLUTION_SENSOR_RESOL_HIGH = 26,
        SNR_RESOLUTION_EXPONENT_LOW = 27,
        SNR_RESOLUTION_EXPONENT_HIGH = 31
    } sensor_resolution_bits;
};

struct arm_scmi_sensor_axis_description_get {
  struct {
      uint32_t sensor_id;
      uint32_t axis_desc_index;
  } parameters;
  struct {
      uint32_t num_axis_flags;
      uint32_t sensor_axis_descriptors;
  }returns;
  /*enum for num_axis_flags*/
  enum {
      AXIS_NUM_FLAGS_AXIS_DESC_RET_LOW = 0,
      AXIS_NUM_FLAGS_AXIS_DESC_RET_HIGH = 5,
      AXIS_NUM_FLAGS_RESV_LOW = 6,
      AXIS_NUM_FLAGS_RESV_HIGH = 25,
      AXIS_NUM_FLAGS_REMAIN_AXIS_DESC_LOW = 26,
      AXIS_NUM_FLAGS_REMAIN_AXIS_DESC_HIGH = 31
  }num_axis_flags_bits;
};
/* sensor axis description struct and enums */
struct arm_scmi_sensor_axis_descriptor {
    uint32_t axis_id;
    uint32_t axis_attributes_low;
    uint32_t axis_attributes_high;
    uint8_t name[SCMI_NAME_STR_SIZE];
    uint32_t axis_resolution;
    uint32_t axis_min_range_low;
    uint32_t axis_min_range_high;
    uint32_t axis_max_range_low;
    uint32_t axis_max_range_high;

    enum {
        AXIS_ATTR_LOW_RESV_LOW = 0,
        AXIS_ATTR_LOW_RESV_HIGH = 7,
        AXIS_ATTR_LOW_EXT_ATTR_SUPPORT = 8,
        AXIS_ATTR_LOW_RESV_UPPER_LOW = 9,
        AXIS_ATTR_LOW_RESV_UPPER_HIGH = 31
    } axis_attributes_low_bits;

    enum {
        AXIS_ATTR_HIGH_SENSOR_TYPE_LOW = 0,
        AXIS_ATTR_HIGH_SENSOR_TYPE_HIGH = 7,
        AXIS_ATTR_HIGH_RESV_LOW = 8,
        AXIS_ATTR_HIGH_RESV_HIGH = 10,
        AXIS_ATTR_HIGH_EXP_LOW = 11,
        AXIS_ATTR_HIGH_EXP_HIGH = 15,
        AXIS_ATTR_HIGH_RESV_UPPER_LOW = 16,
        AXIS_ATTR_HIGH_RESV_UPPER_HIGH = 31
    } axis_attributes_high_bits;

    enum {
        AXIS_RESOLUTION_LOW = 0,
        AXIS_RESOLUTION_HIGH = 26,
        AXIS_RESOLUTION_EXP_LOW = 27,
        AXIS_RESOLUTION_EXP_HIGH = 31
    } axis_resolution_bits;
};

struct arm_scmi_sensor_list_update_intervals {
    struct {
        uint32_t sensor_id;
        uint32_t index;
    } parameters;
    struct {
        uint32_t update_intervals_flags;
        uint32_t intervals;
    } returns;

    enum {
        UPDT_INT_NUM_RET_INTERVALS_LOW = 0,
        UPDT_INT_NUM_RET_INTERVALS_HIGH = 11,
        UPDT_INT_RET_FORMAT = 12,
        UPDT_INT_RESV_LOW = 13,
        UPDT_INT_RESV_HIGH = 15,
        UPDT_INT_REMAIN_INTERVALS_LOW = 16,
        UPDT_INT_REMAIN_INTERVALS_HIGH = 31
    } update_intervals_flags_bits;

    enum {
        INTERVAL_EXP_LOW = 0,
        INTERVAL_EXP_HIGH = 4,
        INTERVAL_SECS_LOW = 5,
        INTERVAL_SECS_HIGH = 20,
        INTERVAL_RESV_LOW = 21,
        INTERVAL_RESV_HIGH = 31
    } intervals_bits;
};

struct arm_scmi_sensor_trip_point_notify {
    struct {
        uint32_t sensor_id;
        uint32_t sensor_event_control;
    } parameters;
    enum {
        SNR_EVENT_CTRL_NOTIFUCATION_ENABLE_LOW = 0,
        SNR_EVENT_CTRL_NOTIFUCATION_ENABLE_HIGH = 0,
        SNR_EVENT_CTRL_RESERVED_LOW = 1,
        SNR_EVENT_CTRL_RESERVED_ENABLE_HIGH = 31
    }sensor_event_control_bits;
};

struct arm_scmi_sensor_trip_point_config {
    struct {
        uint32_t sensor_id;
        uint32_t trip_point_ev_ctrl;
        uint32_t trip_point_val_low;
        uint32_t trip_point_val_high;
    } parameters;

    enum {
        SNR_TRIP_POINT_EV_CTRL_BITS_LOW = 0,
        SNR_TRIP_POINT_EV_CTRL_BITS_HIGH = 1,
        SNR_TRIP_POINT_EV_CTRL_LOWER_RESVD_LOW = 2,
        SNR_TRIP_POINT_EV_CTRL_LOWER_RESVD_HIGH = 3,
        SNR_TRIP_POINT_ID_LOW = 4,
        SNR_TRIP_POINT_ID_HIGH = 11,
        SNR_TRIP_POINT_EV_CTRL_UPPER_RESVD_LOW = 12,
        SNR_TRIP_POINT_EV_CTRL_UPPER_RESVD_HIGH = 31
    }trip_point_ev_ctrl_bits;
};

struct arm_scmi_config_get {
    struct {
        uint32_t sensor_id;
    } parameters;
    struct {
        uint32_t sensor_config;
    } returns;

    enum {
        SNR_CFG_SENSOR_STATE = 0,
        SNR_CFG_TIMESTAMP_REPORTING = 1,
        SNR_CFGT_RESV_LOW = 2,
        SNR_CFG_RESV_HIGH = 10,
        SNR_CFG_UPDATE_INT_EXP_LOW = 11,
        SNR_CFG_UPDATE_INT_EXP_HIGH = 15,
        SNR_CFG_UPDATE_INT_SEC_LOW = 16,
        SNR_CFG_UPDATE_INT_SEC_HIGH = 31
    } sensor_config_get_bits;
};

struct arm_scmi_config_set {
    struct {
        uint32_t sensor_id;
        uint32_t sensor_config;
    } parameters;

    enum {
        SNR_CFG_SET_SENSOR_STATE = 0,
        SNR_CFG_SET_TIMESTAMP_REPORTING = 1,
        SNR_CFG_SET_RESV_LOW = 2,
        SNR_CFG_SET_RESV_HIGH = 8,
        SNR_CFG_SET_ROUND_LOW = 9,
        SNR_CFG_SET_ROUND_HIGH = 10,
        SNR_CFG_SET_UPD_INT_LOW = 16,
        SNR_CFG_SET_UPD_INT_HIGH = 31
    } sensor_config_set_bits;
};

struct arm_scmi_sensor_reading_get {
    struct {
        uint32_t sensor_id;
        uint32_t sensor_reading_flags;
    } parameters;
    struct {
        uint32_t sensor_readings;
    } returns;
    enum {
        SNR_READING_FLAG_SYNC_ASYNC = 0,
        SNR_READING_FLAG_RESERVED_LOW = 1,
        SNR_READING_FLAG_RESERVED_HIGH = 31
    }sensor_reading_flags_bits;
};

struct arm_scmi_sensor_reading {
    uint32_t sensor_value_low;
    uint32_t sensor_value_high;
    uint32_t timestamp_low;
    uint32_t timestamp_high;
};

struct arm_scmi_cont_update_notify {
    struct {
        uint32_t sensor_id;
        uint32_t notify_enable;
    } parameters;

    enum {
        SNR_NTF_ENB_NOTIFY_ENABLE = 0,
        SNR_NTF_ENB_RESV_LOW = 1,
        SNR_NTF_ENB_RESV_HIGH = 31
    } sensor_notify_enable_bits;
};

#endif /* SENSOR_COMMON_H_ */
