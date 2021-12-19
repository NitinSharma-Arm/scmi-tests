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

#include "val_interface.h"
#include "val_sensor.h"

#define TEST_NUM  (SCMI_SENSOR_TEST_NUM_BASE + 5)
#define TEST_DESC "Sensor description get check                 "

#define SENSOR_DESC_LEN 13
#define START_SENSOR_ID 0
#define EXTENDED_ATTR_SUPPORTED 1

uint32_t sensor_query_description_get_scmi_v3(void)
{
    int32_t  status;
    uint32_t rsp_msg_hdr;
    uint32_t cmd_msg_hdr;
    size_t   param_count;
    size_t   return_value_count;
    uint32_t return_values[MAX_RETURNS_SIZE];
    uint32_t num_remaining_desc, num_desc_retured = 0;
    uint32_t total_sensors = 0;
    uint32_t i, desc_index = 0, sensor_id;
    uint32_t async_support, num_trip_points, cont_update_notify_support;
    uint32_t timestamp_support, ext_attr_support;
    uint32_t *sensor_desc, num_sensor_flag;
    uint32_t num_axis, unit_exponent, axis_support;
    uint32_t sensor_type;

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    val_print(VAL_PRINT_TEST, "\n     [Check 1] Query sensor description ");

    /* Reading sensor description repeatedly until all sensor details received*/
    do
    {
        VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
        desc_index += num_desc_retured;
        param_count++;
        cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_SENSOR, SENSOR_DESCRIPTION_GET, COMMAND_MSG);
        val_send_message(cmd_msg_hdr, param_count, &desc_index, &rsp_msg_hdr, &status,
                         &return_value_count, return_values);

        if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        val_print_return_values(return_value_count, return_values);

        /* Check flag values */
        num_sensor_flag = return_values[NUM_SENSOR_FLAG_OFFSET];
        num_remaining_desc = VAL_EXTRACT_BITS(num_sensor_flag, 16, 31);
        val_print(VAL_PRINT_DEBUG, "\n       NUM OF REMAINING DESC  : %d", num_remaining_desc);

        if (val_reserved_bits_check_is_zero(VAL_EXTRACT_BITS(num_sensor_flag, 12, 15)) !=
                VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        num_desc_retured = VAL_EXTRACT_BITS(num_sensor_flag, 0, 11);
        val_print(VAL_PRINT_DEBUG, "\n       NUM OF DESC RETURNED   : %d", num_desc_retured);

        /* Sensor desc start from return_values[1] */
        sensor_desc = &return_values[DESC_OFFSET];
        for (i = 0; i < num_desc_retured; i++)
        {
            val_print(VAL_PRINT_TEST, "\n       Sensor description ");

            sensor_id = sensor_desc[i * SENSOR_DESC_LEN];
            val_print(VAL_PRINT_DEBUG, "\n       Sensor id              : %d", sensor_id);

            /* sensor_attributes_low */
            async_support = VAL_EXTRACT_BITS(sensor_desc[1 + (i * SENSOR_DESC_LEN)], 31, 31);
            val_print(VAL_PRINT_DEBUG, "\n       Async read support     : %d", async_support);

            cont_update_notify_support = VAL_EXTRACT_BITS(sensor_desc[1 + (i * SENSOR_DESC_LEN)], 30, 30);
            val_print(VAL_PRINT_DEBUG, "\n       Cont Update Notify Sup : %d", cont_update_notify_support);

            if (val_reserved_bits_check_is_zero(VAL_EXTRACT_BITS(
                    sensor_desc[1 + (i * SENSOR_DESC_LEN)], 15, 29)) != VAL_STATUS_PASS)
                return VAL_STATUS_FAIL;

            val_print(VAL_PRINT_DEBUG, "\n       Timestamp exponent     : %d",
                        VAL_EXTRACT_BITS(sensor_desc[1 + (i * SENSOR_DESC_LEN)], 10, 14));

            timestamp_support = VAL_EXTRACT_BITS(sensor_desc[1 + (i * SENSOR_DESC_LEN)], 9, 9);
            val_print(VAL_PRINT_DEBUG, "\n       Time stamp support     : %d", timestamp_support);

            ext_attr_support = VAL_EXTRACT_BITS(sensor_desc[1 + (i * SENSOR_DESC_LEN)], 8, 8);
            val_print(VAL_PRINT_DEBUG, "\n       Extended Attr Support  : %d", ext_attr_support);

            num_trip_points = VAL_EXTRACT_BITS(sensor_desc[1 + (i * SENSOR_DESC_LEN)], 0, 7);
            val_print(VAL_PRINT_DEBUG, "\n       Number of trip points  : %d", num_trip_points);

            val_print(VAL_PRINT_TEST, "\n     [Check 2] Check Extended attributes from Sensors");
            if (val_compare("Extended Attr Support   ", ext_attr_support, EXTENDED_ATTR_SUPPORTED))
                return VAL_STATUS_FAIL;

            /* sensor_attributes_high */
            if (val_reserved_bits_check_is_zero(VAL_EXTRACT_BITS(
                    sensor_desc[2 + (i * SENSOR_DESC_LEN)], 22, 31)) != VAL_STATUS_PASS)
                return VAL_STATUS_FAIL;

            num_axis = VAL_EXTRACT_BITS(sensor_desc[2 + (i * SENSOR_DESC_LEN)], 16, 21);
            val_print(VAL_PRINT_DEBUG, "\n       Number of Axis         : %d", num_axis);

            unit_exponent = VAL_EXTRACT_BITS(sensor_desc[2 + (i * SENSOR_DESC_LEN)], 11, 15);
            val_print(VAL_PRINT_DEBUG, "\n       Sensor Unit Exponent   : %d", unit_exponent);

            if (val_reserved_bits_check_is_zero(VAL_EXTRACT_BITS(
                    sensor_desc[2 + (i * SENSOR_DESC_LEN)], 9, 10)) != VAL_STATUS_PASS)
                return VAL_STATUS_FAIL;

            axis_support = VAL_EXTRACT_BITS(sensor_desc[2 + (i * SENSOR_DESC_LEN)], 8, 8);
            val_print(VAL_PRINT_DEBUG, "\n       Sensor Axis Support    : %d", axis_support);

            sensor_type = VAL_EXTRACT_BITS(sensor_desc[2 + (i * SENSOR_DESC_LEN)], 0, 7);
            val_print(VAL_PRINT_DEBUG, "\n       Sensor Type            : %d", sensor_type);

            /* Sensor Name */
            val_print(VAL_PRINT_DEBUG, "\n       Sensor name            : %s",
                    &sensor_desc[3 + (i * SENSOR_DESC_LEN)]);

            /* Sensor Power */
            val_print(VAL_PRINT_DEBUG, "\n       Sensor Power           : %d",
                    sensor_desc[7 + (i * SENSOR_DESC_LEN)]);

            /* sensor resolution */
            val_print(VAL_PRINT_DEBUG, "\n       Sensor Resolution exp  : %d",
                        VAL_EXTRACT_BITS(sensor_desc[8 + (i * SENSOR_DESC_LEN)], 27, 31));

            val_print(VAL_PRINT_DEBUG, "\n       Sensor Resolution      : %d",
                        VAL_EXTRACT_BITS(sensor_desc[8 + (i * SENSOR_DESC_LEN)], 0, 26));

            /* Sensor Min Range */
            val_print(VAL_PRINT_DEBUG, "\n       Sensor Min Range Low   : %d",
                        sensor_desc[9 + (i * SENSOR_DESC_LEN)]);
            val_print(VAL_PRINT_DEBUG, "\n       Sensor Min Range High  : %d",
                        sensor_desc[10 + (i * SENSOR_DESC_LEN)]);

            /* Sensor Max Range */
            val_print(VAL_PRINT_DEBUG, "\n       Sensor Max Range Low   : %d",
                        sensor_desc[11 + (i * SENSOR_DESC_LEN)]);
            val_print(VAL_PRINT_DEBUG, "\n       Sensor Max Range High  : %d",
                        sensor_desc[12 + (i * SENSOR_DESC_LEN)]);

            /* Save sensor information */
            val_sensor_save_desc_info(SENSOR_NUM_OF_TRIP_POINTS, sensor_id, num_trip_points);
            val_sensor_save_desc_info(SENSOR_ASYNC_READ_SUPPORT, sensor_id, async_support);

            /* Save extended sensor information */
            val_sensor_ext_save_desc_info(SENSOR_CONT_NOTIFY_UPDATE_SUPPORT,
                                          sensor_id, cont_update_notify_support);
            val_sensor_ext_save_desc_info(SENSOR_TIMESTAMP_SUPPORT,
                                          sensor_id, timestamp_support);
            val_sensor_ext_save_desc_info(SENSOR_NUM_OF_AXIS, sensor_id, num_axis);
            val_sensor_ext_save_desc_info(SENSOR_AXIS_SUPPORT, sensor_id, axis_support);

            /* Ensure sensor description indicates sensor identifiers start from 0 and are sequential*/
            val_print(VAL_PRINT_TEST, "\n     [Check 3] Ensure sensor IDs start from 0");

            /* Get sensor ID for first returned sensor.*/
            if (total_sensors == 0 && i == 0)
            {
              if (val_compare("START SENSOR ID   ", sensor_id, START_SENSOR_ID))
                  return VAL_STATUS_FAIL;

              val_print(VAL_PRINT_DEBUG, "\n       Start Sensor      : %s, has ID 0",
                      &sensor_desc[3 + (i * SENSOR_DESC_LEN)]);
            }
            /* Check sensor IDs are sequential. */
            if (val_compare("SENSOR ID SEQUENCE", sensor_id, i+total_sensors))
                return VAL_STATUS_FAIL;
        }
        total_sensors += num_desc_retured;
    } while (num_remaining_desc > 0);

    if (val_compare("NUM OF DESC  ", total_sensors, val_sensor_get_expected_num_sensors()))
        return VAL_STATUS_FAIL;

    /* Sensor get desc for invalid descriptor index */
    desc_index = total_sensors + 1;
    val_print(VAL_PRINT_TEST, "\n     [Check 4] Query with invalid desc_index : %d", desc_index);

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    param_count++;
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_SENSOR, SENSOR_DESCRIPTION_GET, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, &desc_index, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    if (val_compare_status(status, SCMI_INVALID_PARAMETERS) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    return VAL_STATUS_PASS;
}
