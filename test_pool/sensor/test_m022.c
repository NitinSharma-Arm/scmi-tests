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

#define TEST_NUM  (SCMI_SENSOR_TEST_NUM_BASE + 22)
#define TEST_DESC "Sensor configuration setting check  "

#define MAX_PARAMETER_SIZE 2

/************ TEST ALGO ******************
 * Get current sensor State
 * Choose a different state for sensor
 * Set new state in sensor
 * Check new state has been set.
 * Restore the default state for sensor.
*****************************************/

uint32_t sensor_set_configuration_check(void)
{
    int32_t  status;
    uint32_t rsp_msg_hdr;
    uint32_t cmd_msg_hdr;
    size_t   param_count;
    size_t   return_value_count;
    uint32_t return_values[MAX_RETURNS_SIZE];
    uint32_t parameters[MAX_PARAMETER_SIZE];
    uint32_t sensor_id, num_sensors;
    uint32_t default_sensor_config, new_sensor_config;
    uint32_t default_sensor_state, sensor_state, new_sensor_state;
    uint32_t update_interval, update_interval_exp;
    uint32_t timestamp_reporting, round_up_down;

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    num_sensors = val_sensor_get_info(NUM_SENSORS);
    if (num_sensors == 0) {
        val_print(VAL_PRINT_ERR, "\n       No Sensor found                             ");
        return VAL_STATUS_SKIP;
    }
    val_print(VAL_PRINT_DEBUG, "\n       NUM SENSORS     : %d", num_sensors);

    for (sensor_id = 0; sensor_id < num_sensors; sensor_id++)
    {
        /* STEP 1: Reading sensor configuration */
        val_print(VAL_PRINT_TEST, "\n    [Step 1] Get Operational sensor state ");
        val_print(VAL_PRINT_TEST, "\n     SENSOR ID: %d", sensor_id);

        VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
        parameters[param_count++] = sensor_id;

        cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_SENSOR, SENSOR_CONFIG_GET, COMMAND_MSG);
        val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                         &return_value_count, return_values);

        if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
             return VAL_STATUS_FAIL;

        if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
             return VAL_STATUS_FAIL;

        default_sensor_config = return_values[SENSOR_CONFIG_OFFSET];

        /* Save the default sensor state */
        update_interval = VAL_EXTRACT_BITS(default_sensor_config, 16, 31);
        update_interval_exp = VAL_EXTRACT_BITS(default_sensor_config, 11, 15);
        round_up_down = VAL_EXTRACT_BITS(default_sensor_config, 9, 10);
        timestamp_reporting = VAL_EXTRACT_BITS(default_sensor_config, 1, 1);

        default_sensor_state = VAL_EXTRACT_BITS(default_sensor_config, 0, 0);
        val_print(VAL_PRINT_DEBUG, "\n       Default Sensor State   : %d", default_sensor_state);

        /* STEP 2: Set a sensor config with modified state. */
        val_print(VAL_PRINT_TEST, "\n    [Step 2] Choose another state for sensor");
        new_sensor_state = default_sensor_state == DISABLED ? ENABLED : DISABLED;

        new_sensor_config = val_sensor_set_config(update_interval, update_interval_exp, round_up_down,
                                            timestamp_reporting, new_sensor_state);

        /* STEP 3 : Set new state for sensor */
        val_print(VAL_PRINT_TEST, "\n    [Step 3] Set new sensor state");
        VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
        parameters[param_count++] = sensor_id;
        parameters[param_count++] = new_sensor_config;

        cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_SENSOR, SENSOR_CONFIG_SET, COMMAND_MSG);
        val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                         &return_value_count, return_values);

        if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
             return VAL_STATUS_FAIL;

        if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
             return VAL_STATUS_FAIL;

        /* STEP 4 : Verify new state has been set */
        val_print(VAL_PRINT_TEST, "\n    [Step 4] Verify newly set sensor state ");
        VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
        parameters[param_count++] = sensor_id;

        cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_SENSOR, SENSOR_CONFIG_GET, COMMAND_MSG);
        val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                         &return_value_count, return_values);

        if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
             return VAL_STATUS_FAIL;

        if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
             return VAL_STATUS_FAIL;

        new_sensor_config = return_values[SENSOR_CONFIG_OFFSET];
        sensor_state = VAL_EXTRACT_BITS(new_sensor_config, 0, 0);
        if (val_compare("Updated sensor state   ", sensor_state, new_sensor_state))
            return VAL_STATUS_FAIL;

        /* STEP 5 : Set default sensor state back */
        val_print(VAL_PRINT_TEST, "\n    [Step 3] Set new sensor state");
        VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
        parameters[param_count++] = sensor_id;
        parameters[param_count++] = default_sensor_config;

        cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_SENSOR, SENSOR_CONFIG_SET, COMMAND_MSG);
        val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                         &return_value_count, return_values);

        if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
             return VAL_STATUS_FAIL;

        if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
             return VAL_STATUS_FAIL;
    }

    return VAL_STATUS_PASS;
}
