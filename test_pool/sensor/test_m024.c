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

#define TEST_NUM  (SCMI_SENSOR_TEST_NUM_BASE + 24)
#define TEST_DESC "Sensor request sensor update notification check"

#define MAX_PARAMETER_SIZE        1

uint32_t sensor_request_sensor_notification_check(void)
{
    int32_t  status;
    uint32_t rsp_msg_hdr;
    uint32_t cmd_msg_hdr;
    size_t   param_count;
    size_t   return_value_count;
    uint32_t return_values[MAX_RETURNS_SIZE];
    uint32_t parameters[MAX_PARAMETER_SIZE], sensor_id;
    uint32_t notification_support;
    uint32_t notify_enable;
    uint32_t num_sensors, sensor_state;

    num_sensors = val_sensor_get_info(NUM_SENSORS);
    if (num_sensors == 0) {
        val_print(VAL_PRINT_ERR, "\n       No Sensor found                             ");
        return VAL_STATUS_SKIP;
    }
    val_print(VAL_PRINT_DEBUG, "\n       NUM SENSORS     : %d", num_sensors);

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    /* Request sensor notification update if supported */
    for (sensor_id = 0; sensor_id < num_sensors; sensor_id++)
    {
        val_print(VAL_PRINT_DEBUG, "\n       Sensor id              : %d", sensor_id);

        /* Check sensor update notification is supported. */
        notification_support = val_sensor_ext_get_desc_info(SENSOR_CONT_NOTIFY_UPDATE_SUPPORT,
                                                            sensor_id);
        if (val_compare("Sensor notification support enabled", notification_support, ENABLED)
                        == VAL_STATUS_FAIL)
            continue;

        /* Check sensor state is enabled */
        sensor_state = val_sensor_ext_get_desc_info(SENSOR_STATE, sensor_id);
        if (val_compare("Sensor state enabled", sensor_state, ENABLED)
                        == VAL_STATUS_FAIL)
            continue;

        val_print(VAL_PRINT_TEST, "\n     [Check 1] Sensor request update notification");
        VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
        cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_SENSOR, SENSOR_CONTINUOUS_UPDATE_NOTIFY,
                                          COMMAND_MSG);

        parameters[param_count++] = sensor_id;
        notify_enable = val_get_notify_enable_config(ENABLED);
        parameters[param_count++] = notify_enable;

        val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                         &return_value_count, return_values);

        if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        val_print_return_values(return_value_count, return_values);

    }

    return VAL_STATUS_PASS;
}
