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

#define TEST_NUM  (SCMI_SENSOR_TEST_NUM_BASE + 18)
#define TEST_DESC "Sensor Get supported update intervals check "

#define MAX_PARAMETER_SIZE 2

uint32_t sensor_supported_update_intervals_check(void)
{
    int32_t  status;
    uint32_t rsp_msg_hdr;
    uint32_t cmd_msg_hdr;
    size_t   param_count;
    size_t   return_value_count;
    uint32_t return_values[MAX_RETURNS_SIZE];
    uint32_t parameters[MAX_PARAMETER_SIZE];
    uint32_t sensor_id, num_sensors;
    uint32_t update_interval_index = 0;
    uint32_t num_interval_retured = 0, num_remaining_interval;
    uint32_t interval_flag, interval_format;
    uint32_t start_interval, end_interval, step_size;
    uint32_t *intervals_array, interval, i;

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    num_sensors = val_sensor_get_info(NUM_SENSORS);
    if (num_sensors == 0) {
        val_print(VAL_PRINT_ERR, "\n       No Sensor found                             ");
        return VAL_STATUS_SKIP;
    }
    val_print(VAL_PRINT_DEBUG, "\n       NUM SENSORS     : %d", num_sensors);

    /* Reading sensor value with incorrect flags */
    for (sensor_id = 0; sensor_id < num_sensors; sensor_id++)
    {
        val_print(VAL_PRINT_TEST, "\n     SENSOR ID: %d", sensor_id);
        val_print(VAL_PRINT_TEST, "\n     [Check 1] Get Sensor supported update intervals ");

        num_remaining_interval = 0;
        num_interval_retured = 0;

        do {
            update_interval_index += num_interval_retured;

            VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
            parameters[param_count++] = sensor_id;
            parameters[param_count++] = update_interval_index;
            cmd_msg_hdr = val_msg_hdr_create(
                              PROTOCOL_SENSOR, SENSOR_LIST_UPDATE_INTERVALS, COMMAND_MSG);
            val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                             &return_value_count, return_values);

            if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
                 return VAL_STATUS_FAIL;

            if (val_compare_status(status, SCMI_INVALID_PARAMETERS) != VAL_STATUS_PASS)
                 return VAL_STATUS_FAIL;

            /* Check update intervals flags */
            interval_flag = return_values[UPDATE_INTERVAL_FLAG_OFFSET];

            num_remaining_interval = VAL_EXTRACT_BITS(interval_flag, 16, 31);
            val_print(VAL_PRINT_DEBUG, "\n       NUMBER OF INTERVALS    : %d",
                          num_remaining_interval);

            if (val_reserved_bits_check_is_zero(VAL_EXTRACT_BITS(interval_flag, 13, 15)) !=
                    VAL_STATUS_PASS)
                return VAL_STATUS_FAIL;

            interval_format = VAL_EXTRACT_BITS(interval_flag, 12, 12);
            val_print(VAL_PRINT_DEBUG, "\n       Update interval format : %d",
                          interval_format);

            num_interval_retured = VAL_EXTRACT_BITS(interval_flag, 0, 11);
            val_print(VAL_PRINT_DEBUG, "\n       NUM OF INTERVALS RETURN: %d",
                          num_interval_retured);

            intervals_array = &return_values[LEVEL_ARRAY_OFFSET];

            /* Verify only 3 values are returned in case interva; return format is triplet */
            if (interval_format == LEVEL_FORMAT_TRIPLET)
            {
                val_print(VAL_PRINT_TEST, "\n    [Check 2] Verify triplet return format",
                            sensor_id);
                if(num_interval_retured != 3 || num_remaining_interval != 0)
                    return VAL_STATUS_FAIL;

                /* Check Start Interval */
                start_interval = intervals_array[INTERVAL_START_OFFSET];
                if (val_reserved_bits_check_is_zero(VAL_EXTRACT_BITS(start_interval, 21, 31)) !=
                        VAL_STATUS_PASS)
                    return VAL_STATUS_FAIL;

                val_print(VAL_PRINT_DEBUG, "\n       start interval seconds : %d",
                              VAL_EXTRACT_BITS(start_interval, 5, 20));
                val_print(VAL_PRINT_DEBUG, "\n       start interval exponent: %d",
                              VAL_EXTRACT_BITS(start_interval, 0, 4));

                /* Check End interval */
                end_interval = intervals_array[INTERVAL_START_OFFSET];
                if (val_reserved_bits_check_is_zero(VAL_EXTRACT_BITS(end_interval, 21, 31)) !=
                        VAL_STATUS_PASS)
                    return VAL_STATUS_FAIL;

                val_print(VAL_PRINT_DEBUG, "\n       end interval seconds   : %d",
                              VAL_EXTRACT_BITS(end_interval, 5, 20));
                val_print(VAL_PRINT_DEBUG, "\n       end interval exponent  : %d",
                              VAL_EXTRACT_BITS(end_interval, 0, 4));

                /* Step interval Size check */
                step_size = intervals_array[INTERVAL_START_OFFSET];
                if (val_reserved_bits_check_is_zero(VAL_EXTRACT_BITS(step_size, 21, 31)) !=
                        VAL_STATUS_PASS)
                    return VAL_STATUS_FAIL;

                val_print(VAL_PRINT_DEBUG, "\n       step interval seconds  : %d",
                              VAL_EXTRACT_BITS(step_size, 5, 20));
                val_print(VAL_PRINT_DEBUG, "\n       step interval exponent : %d",
                              VAL_EXTRACT_BITS(step_size, 0, 4));
            }
            /* Update interval array check */
            else
            {
              for (i = 0; i < num_interval_retured; i++)
              {
                  val_print(VAL_PRINT_DEBUG, "\n       Interval Index  : %d",
                            (update_interval_index+i));

                  interval = intervals_array[(update_interval_index+i)];

                  if (val_reserved_bits_check_is_zero(VAL_EXTRACT_BITS(interval, 21, 31)) !=
                          VAL_STATUS_PASS)
                      return VAL_STATUS_FAIL;

                  val_print(VAL_PRINT_DEBUG, "\n       interval seconds       : %d",
                                VAL_EXTRACT_BITS(interval, 5, 20));
                  val_print(VAL_PRINT_DEBUG, "\n       interval exponent      : %d",
                                VAL_EXTRACT_BITS(interval, 0, 4));
              }
            }
        } while (num_remaining_interval > 0);

        /* Check Sensor Update interval for invalid index values. */
        if (interval_format == LEVEL_FORMAT_TRIPLET) {
            continue; /* Ignore index for triplet return values */
        }

        update_interval_index = update_interval_index + num_interval_retured + 1;
        val_print(VAL_PRINT_TEST, "\n     [Check 3] Query with invalid index: %d",
                    update_interval_index);

        VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
        parameters[param_count++] = sensor_id;
        parameters[param_count++] = update_interval_index; /*Invalid interval index*/
        cmd_msg_hdr = val_msg_hdr_create(
                              PROTOCOL_SENSOR, SENSOR_LIST_UPDATE_INTERVALS, COMMAND_MSG);
        val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                         &return_value_count, return_values);

        if (val_compare_status(status, SCMI_OUT_OF_RANGE) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;
    }

    return VAL_STATUS_PASS;
}
