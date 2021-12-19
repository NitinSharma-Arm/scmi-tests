/** @file
 * Copyright (c) 2020, Arm Limited or its affiliates. All rights reserved.
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

#include <sensor_protocol.h>
#include <sensor_common.h>
#include <pal_sensor_expected.h>

struct arm_scmi_sensor_protocol sensor_protocol;
#define SENSOR_DESC_LEN 13
#define SENSOR_AXIS_DESC_LEN  12

void fill_sensor_protocol()
{
    sensor_protocol.protocol_version = SENSOR_VERSION;
    sensor_protocol.number_sensors = num_sensors;
    sensor_protocol.sensor_trip_point_notify_supported = true;
    sensor_protocol.sensor_trip_point_config_supported = true;
    sensor_protocol.sensor_reg_address_low = statistics_address_low_snsr;
    sensor_protocol.sensor_reg_length = statistics_address_len_snsr;
    sensor_protocol.sensor_reg_address_high =
    sensor_protocol.sensor_reg_address_low + sensor_protocol.sensor_reg_length;
    sensor_protocol.asynchronous_sensor_read_support =
            async_sensor_read_support;
    sensor_protocol.cont_update_notify_support =
            continous_sensor_update_notification_support;
    sensor_protocol.timestamp_support = sensor_timestamp_support;
    sensor_protocol.extended_attributes_support =
            sensor_extended_attributes_support;
    sensor_protocol.number_of_trip_points_supported =
            number_of_trip_points_supported;
    sensor_protocol.axis_supported = sensor_axis_supported;
}

void sensor_send_message(uint32_t message_id, uint32_t parameter_count,
        const uint32_t *parameters, int32_t *status,
        size_t *return_values_count, uint32_t *return_values)
{

    uint32_t parameter_idx, return_idx, sensor_id, sensor_cfg;
    char * str;
    int i;

    switch(message_id)
    {
    case SNSR_PROTO_VER_MSG_ID:
        *status = SCMI_STATUS_SUCCESS;
        *return_values_count = 1;
        return_values[0] = sensor_protocol.protocol_version;
        break;
    case SNSR_PROTO_ATTRIB_MSG_ID:
        *status = SCMI_STATUS_SUCCESS;
        *return_values_count = 4;
        return_idx = OFFSET_RET(struct arm_scmi_sensor_protocol_attributes,
                attributes);
        return_values[return_idx] = (sensor_protocol.number_sensors << SNR_NUMBER_SENSORS_LOW) +
                        (sensor_protocol.max_num_pending_async_cmds_supported <<
                                SNR_MAX_NUM_PENDING_ASYNC_CMDS_SPRT_LOW) +
                                (RESERVED << SNR_RESERVED_LOW);
        return_values[OFFSET_RET(
                struct arm_scmi_sensor_protocol_attributes,
                sensor_reg_address_low)] = sensor_protocol.sensor_reg_address_low;
        return_values[OFFSET_RET(
                struct arm_scmi_sensor_protocol_attributes,
                sensor_reg_address_high)] = sensor_protocol.sensor_reg_address_high;
        return_values[OFFSET_RET(
                struct arm_scmi_sensor_protocol_attributes,
                sensor_reg_len)] = sensor_protocol.sensor_reg_length;
        break;
    case SNSR_PROTO_MSG_ATTRIB_MSG_ID:
        parameter_idx = OFFSET_PARAM(
                struct arm_scmi_sensor_protocol_message_attributes,
                message_id);
        if(parameters[parameter_idx] > SNSR_READING_GET_MSG_ID)
        {
            *status = SCMI_STATUS_NOT_FOUND;
            break;
        }
        *status = SCMI_STATUS_SUCCESS;
        *return_values_count = 1;
        return_values[OFFSET_RET(
                struct arm_scmi_sensor_protocol_message_attributes,
                attributes)] = 0x0;
        break;
    case SNSR_DESC_GET_MSG_ID:
        parameter_idx = parameters[OFFSET_PARAM(
                struct arm_scmi_sensor_description_get,
                desc_index)];
        if(parameter_idx >= sensor_protocol.number_sensors)
        {
            *status = SCMI_STATUS_INVALID_PARAMETERS;
            break;
        }
        return_values[OFFSET_RET(
                struct arm_scmi_sensor_description_get,
                num_sensor_flags)] = sensor_protocol.number_sensors;
        for (i = 0; i < sensor_protocol.number_sensors; i++)
        {
            return_values[OFFSET_RET(
                    struct arm_scmi_sensor_description_get,
                    sensor_descriptors) + SENSOR_DESC_LEN * i] = i;
            return_values[OFFSET_RET(
                    struct arm_scmi_sensor_description_get,
                    sensor_descriptors) + (SENSOR_DESC_LEN * i) + 1] =
                            (sensor_protocol.asynchronous_sensor_read_support[i] <<
                                    SNR_DESC_ATTRLOW_ASYNC_READ_SUPPORT) |
                            (sensor_protocol.cont_update_notify_support[i] <<
                                    SNR_DESC_ATTRLOW_CNT_SNR_UPDATE_NOT) |
                            (sensor_protocol.timestamp_support[i] <<
                                    SNR_DESC_ATTRLOW_TIMESTAMP_SUPPORT) |
                            (sensor_protocol.extended_attributes_support[i] <<
                                    SNR_DESC_ATTRLOW_EXT_ATTR_SUPPORT) |
                            (sensor_protocol.number_of_trip_points_supported[i] <<
                                    SNR_DESC_ATTRLOW_NUM_TRIPPOINT_SPRT_LOW);
            return_values[OFFSET_RET(
                    struct arm_scmi_sensor_description_get,
                    sensor_descriptors) + (SENSOR_DESC_LEN * i) + 2] =
                            (number_of_axis_supported[i] <<
                                    SNR_DESC_ATTRHIGH_NUM_AXES_LOW) |
                            (sensor_protocol.axis_supported[i] <<
                                    SNR_DESC_ATTRHIGH_AXIS_SUPPORT);

            str = (char *)
                                                (&return_values[OFFSET_RET(
                                                        struct arm_scmi_sensor_description_get,
                                                        sensor_descriptors) + (SENSOR_DESC_LEN * i) + 3]);
            sprintf(str, "SENSOR_%d", i);

            return_values[OFFSET_RET(
                    struct arm_scmi_sensor_description_get,
                    sensor_descriptors) + (SENSOR_DESC_LEN * i) + 7] =
                            sensor_powers[i];
            return_values[OFFSET_RET(
                    struct arm_scmi_sensor_description_get,
                    sensor_descriptors) + (SENSOR_DESC_LEN * i) + 8] =
                                    sensor_resolutions[i] <<
                                            SNR_RESOLUTION_SENSOR_RESOL_LOW;
            return_values[OFFSET_RET(
                    struct arm_scmi_sensor_description_get,
                    sensor_descriptors) + (SENSOR_DESC_LEN * i) + 9] = 9;
            return_values[OFFSET_RET(
                    struct arm_scmi_sensor_description_get,
                    sensor_descriptors) + (SENSOR_DESC_LEN * i) + 10] = 8;
            return_values[OFFSET_RET(
                    struct arm_scmi_sensor_description_get,
                    sensor_descriptors) + (SENSOR_DESC_LEN * i) + 11] = 7;
            return_values[OFFSET_RET(
                    struct arm_scmi_sensor_description_get,
                    sensor_descriptors) + (SENSOR_DESC_LEN * i) + 12] = 6;
        }
        *status = SCMI_STATUS_SUCCESS;
        *return_values_count = 1 +
                (sensor_protocol.number_sensors * SENSOR_DESC_LEN);
        break;
    case SNSR_TRIP_POINT_NOTIFY_ID:
        if (parameters[OFFSET_PARAM(
                struct arm_scmi_sensor_trip_point_notify, sensor_id)] >=
                sensor_protocol.number_sensors)
        {
            *status = SCMI_STATUS_NOT_FOUND;
            break;
        }
        if (parameters[OFFSET_PARAM(
                struct arm_scmi_sensor_trip_point_notify, sensor_event_control)] > 1)
        {
            *status = SCMI_STATUS_INVALID_PARAMETERS;
            break;
        }
        *status = SCMI_STATUS_SUCCESS;
        break;
    case SNSR_TRIP_POINT_CONFIG_ID:
        if (parameters[OFFSET_PARAM(
                struct arm_scmi_sensor_trip_point_config, sensor_id)] >=
                sensor_protocol.number_sensors)
        {
            *status = SCMI_STATUS_NOT_FOUND;
            break;
        }
        if (((parameters[OFFSET_PARAM(
                struct arm_scmi_sensor_trip_point_config, trip_point_ev_ctrl)] >>
                SNR_TRIP_POINT_ID_LOW) & 0xFF) >
        sensor_protocol.number_of_trip_points_supported[
                                                        parameters[OFFSET_PARAM(
                                                                struct arm_scmi_sensor_trip_point_config, sensor_id)]])
        {
            *status = SCMI_STATUS_INVALID_PARAMETERS;
            break;
        }
        if ((parameters[OFFSET_PARAM(
                struct arm_scmi_sensor_trip_point_config, trip_point_ev_ctrl)] >>
                SNR_TRIP_POINT_EV_CTRL_LOWER_RESVD_LOW) & 0x3)
        {
            *status = SCMI_STATUS_INVALID_PARAMETERS;
            break;
        }
        *status = SCMI_STATUS_SUCCESS;
        break;
    case SNSR_READING_GET_MSG_ID:
        if (parameters[OFFSET_PARAM(
                struct arm_scmi_sensor_reading_get, sensor_id)] >=
                sensor_protocol.number_sensors)
        {
            *status = SCMI_STATUS_NOT_FOUND;
            break;
        }
        if (parameters[OFFSET_PARAM(
                struct arm_scmi_sensor_reading_get, sensor_reading_flags)] > 1)
        {
            *status = SCMI_STATUS_INVALID_PARAMETERS;
            break;
        }
        if( (parameters[OFFSET_PARAM(
                struct arm_scmi_sensor_reading_get, sensor_reading_flags)] == 1) &&
                ((sensor_protocol.asynchronous_sensor_read_support[
                                                                   parameters[OFFSET_PARAM(
                                                                           struct arm_scmi_sensor_reading_get, sensor_id)]]) == 0))
        {
            *status = SCMI_STATUS_NOT_SUPPORTED;
            break;
        }
        *status = SCMI_STATUS_SUCCESS;
        *return_values_count = 2;
        break;
    case SNSR_AXIS_DESC_GET_MSG_ID:
        sensor_id = parameters[OFFSET_PARAM(struct arm_scmi_sensor_axis_description_get, sensor_id)];
        if (sensor_id >= sensor_protocol.number_sensors) {
            *status = SCMI_STATUS_NOT_FOUND;
            break;
        }
        if (parameters[OFFSET_PARAM(struct arm_scmi_sensor_axis_description_get, axis_desc_index)] >=
                                            number_of_axis_supported[sensor_id])
        {
            *status = SCMI_STATUS_OUT_OF_RANGE;
            break;
        }
        *status = SCMI_STATUS_SUCCESS;

        return_values[OFFSET_RET(struct arm_scmi_sensor_axis_description_get, num_axis_flags)] =
                      (number_of_axis_supported[sensor_id]) <<
                        AXIS_NUM_FLAGS_AXIS_DESC_RET_LOW;

        for (i = 0; i < number_of_axis_supported[sensor_id]; ++i) {
          return_values[OFFSET_RET(
                  struct arm_scmi_sensor_axis_description_get,
                  sensor_axis_descriptors) + SENSOR_AXIS_DESC_LEN * i] = i;

          return_values[OFFSET_RET(
                  struct arm_scmi_sensor_axis_description_get,
                  sensor_axis_descriptors) + (SENSOR_AXIS_DESC_LEN * i) + 1] =
                          1 << AXIS_ATTR_LOW_EXT_ATTR_SUPPORT;

          return_values[OFFSET_RET(
                  struct arm_scmi_sensor_axis_description_get,
                  sensor_axis_descriptors) + (SENSOR_AXIS_DESC_LEN * i) + 2] =
                          (2 << AXIS_ATTR_HIGH_EXP_LOW) |
                          (3 << AXIS_ATTR_HIGH_SENSOR_TYPE_LOW);

          str = (char *)(&return_values[OFFSET_RET(struct arm_scmi_sensor_axis_description_get,
                          sensor_axis_descriptors) + (SENSOR_AXIS_DESC_LEN * i) + 3]);
          sprintf(str, "SENSOR_%d_AXIS%d", sensor_id, i);

          return_values[OFFSET_RET(
                  struct arm_scmi_sensor_axis_description_get,
                  sensor_axis_descriptors) + (SENSOR_AXIS_DESC_LEN * i) + 7] =
                                  4 << AXIS_RESOLUTION_LOW;
          return_values[OFFSET_RET(
                  struct arm_scmi_sensor_axis_description_get,
                  sensor_axis_descriptors) + (SENSOR_AXIS_DESC_LEN * i) + 8] = 9;
          return_values[OFFSET_RET(
                  struct arm_scmi_sensor_axis_description_get,
                  sensor_axis_descriptors) + (SENSOR_AXIS_DESC_LEN * i) + 9] = 8;
          return_values[OFFSET_RET(
                  struct arm_scmi_sensor_axis_description_get,
                  sensor_axis_descriptors) + (SENSOR_AXIS_DESC_LEN * i) + 10] = 7;
          return_values[OFFSET_RET(
                  struct arm_scmi_sensor_axis_description_get,
                  sensor_axis_descriptors) + (SENSOR_AXIS_DESC_LEN * i) + 11] = 6;
        }
        *return_values_count = 1 +
                (number_of_axis_supported[sensor_id] * SENSOR_AXIS_DESC_LEN);
        *status = SCMI_STATUS_SUCCESS;
        break;
    case SNSR_LIST_UPDATE_INTERVALS_MSG_ID:
        sensor_id = parameters[OFFSET_PARAM(struct arm_scmi_sensor_list_update_intervals, sensor_id)];
        if (sensor_id >= sensor_protocol.number_sensors) {
            *status = SCMI_STATUS_NOT_FOUND;
            break;
        }
        int update_int;
        update_int = parameters[OFFSET_PARAM(struct arm_scmi_sensor_list_update_intervals, update_int_index)];
        if ( update_int >=
                                            sensor_update_interval_count[sensor_id])
        {
            *status = SCMI_STATUS_OUT_OF_RANGE;
            break;
        }
        return_values[OFFSET_RET(struct arm_scmi_sensor_list_update_intervals, update_intervals_flags)] =
                                            sensor_update_interval_count[sensor_id] <<
                                                    UPDT_INT_NUM_RET_INTERVALS_LOW;
        for (i = 0; i < sensor_update_interval_count[sensor_id] ; i++)
        {
          return_values[OFFSET_RET(struct arm_scmi_sensor_list_update_intervals, intervals) + i] =
                                              (1 << INTERVAL_EXP_LOW) |
                                              (1 << INTERVAL_SECS_LOW);
        }
        *status = SCMI_STATUS_SUCCESS;
        *return_values_count = 1 + (sensor_update_interval_count[sensor_id]);
        break;
    case SNSR_CONFIG_GET_MSG_ID:
        sensor_id = parameters[OFFSET_PARAM(struct arm_scmi_config_get, sensor_id)];
        if (sensor_id >= sensor_protocol.number_sensors) {
            *status = SCMI_STATUS_NOT_FOUND;
            break;
        }
        return_values[OFFSET_RET(struct arm_scmi_config_get, sensor_config)] =
                                            (sensor_update_intervals[sensor_id] << SNR_CFG_UPDATE_INT_SEC_LOW) |
                                            (1 << SNR_CFG_UPDATE_INT_EXP_LOW) |
                                            (RESERVED << SNR_CFG_SET_RESV_LOW) |
                                            (1 << SNR_CFG_SET_TIMESTAMP_REPORTING) |
                                            (1 << SNR_CFG_SET_SENSOR_STATE);
        *status = SCMI_STATUS_SUCCESS;
        *return_values_count = 1;
        break;
    case SNSR_CONFIG_SET_MSG_ID:
        sensor_id = parameters[OFFSET_PARAM(struct arm_scmi_config_set, sensor_id)];
        if (sensor_id >= sensor_protocol.number_sensors) {
            *status = SCMI_STATUS_NOT_FOUND;
            break;
        }
        sensor_cfg = parameters[OFFSET_PARAM(struct arm_scmi_config_set, sensor_config)];

        *status = SCMI_STATUS_SUCCESS;
        break;
    case SNSR_CNT_UPDATE_NOTIFY_MSG_ID:
        sensor_id = parameters[OFFSET_PARAM(struct arm_scmi_cont_update_notify, sensor_id)];
        if (sensor_id >= sensor_protocol.number_sensors) {
            *status = SCMI_STATUS_NOT_FOUND;
            break;
        }
        if ( sensor_protocol.cont_update_notify_support[sensor_id] ==
          SENSOR_CONT_NOTI_UPD_NOT_SUPPORTED )
        {
            *status = SCMI_STATUS_NOT_SUPPORTED;
            break;
        }
        sensor_cfg = parameters[OFFSET_PARAM(struct arm_scmi_cont_update_notify, notify_enable)];
        if ( sensor_cfg >> SNR_NTF_ENB_RESV_LOW )
        {
          *status = SCMI_STATUS_INVALID_PARAMETERS;
          break;
        }
        *status = SCMI_STATUS_SUCCESS;
        break;
    default:
        *status = SCMI_STATUS_NOT_FOUND;
    }
}
