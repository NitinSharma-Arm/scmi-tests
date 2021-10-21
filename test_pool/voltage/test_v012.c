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
#include "val_voltage.h"

#define TEST_NUM  (SCMI_VOLTAGE_TEST_NUM_BASE + 12)
#define TEST_DESC "Voltage set new voltage level                  "

#define RETURN_VALUE_COUNT 1
#define PARAMETER_SIZE     3

/********* TEST ALGO ********************
 * Get the current voltage level
 * Find a new level from previously saved voltage levels for domain
 * Set new level in voltage domain
 * Get the voltge level and compare with set voltage value
 * Restore the default voltage level
*****************************************/

uint32_t voltage_set_voltage_level(void)
{
    int32_t  status;
    uint32_t rsp_msg_hdr;
    uint32_t cmd_msg_hdr;
    size_t   param_count;
    size_t   return_value_count;
    uint32_t return_values[MAX_RETURNS_SIZE];
    uint32_t parameters[PARAMETER_SIZE];
    uint32_t num_domains, domain_id;
    uint32_t level_index=0, flags=0;
    uint32_t *voltage_array, start_voltage, default_voltage, voltage;

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    num_domains = val_voltage_get_info(NUM_VOLTAGE_DOMAINS, 0);
    if (num_domains == 0) {
        val_print(VAL_PRINT_ERR, "\n       No Voltage Domains found                   ");
        return VAL_STATUS_SKIP;
    }
    val_print(VAL_PRINT_DEBUG, "\n       Voltage Domains: %d", num_domains);

    /* Check Voltage describe level for valid voltage domains */
    for (domain_id = 0; domain_id < num_domains; domain_id++)
    {
        /* STEP 1 : Get currently operational voltage. */
        val_print(VAL_PRINT_TEST, "\n       Voltage ID     : %d", domain_id);
        val_print(VAL_PRINT_TEST, "\n    [Step 1] Get Operational Voltage ");

        VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
        parameters[param_count++] = domain_id;
        cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_VOLTAGE, VOLTAGE_LEVEL_GET, COMMAND_MSG);
        val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                         &return_value_count, return_values);

        if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        val_print_return_values(return_value_count, return_values);

        default_voltage = return_values[ATTRIBUTE_OFFSET];
        val_print(VAL_PRINT_TEST, "\n       Voltage        : 0x%08X", default_voltage);

        /* STEP 2 : Get first voltage level for domain. */
        val_print(VAL_PRINT_TEST, "\n    [Step 2] Get allowed Voltage levels");
        VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
        cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_VOLTAGE, VOLTAGE_DESCRIBE_LEVELS, COMMAND_MSG);

        parameters[param_count++] = domain_id;
        parameters[param_count++] = level_index;

        val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                         &return_value_count, return_values);

        if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        val_print_return_values(return_value_count, return_values);
        voltage_array = &return_values[LEVEL_ARRAY_OFFSET];
        start_voltage = voltage_array[LEVEL_START_OFFSET];
        val_print(VAL_PRINT_DEBUG, "\n       START VOLTAGE  : 0x%08X", start_voltage);

        /* STEP 3 : Set new voltage level for domain. */
        val_print(VAL_PRINT_TEST, "\n    [Step 3] Set allowed Voltage levels");
        val_print(VAL_PRINT_TEST, "\n    [Check 1] Set new voltage level in domain");
        VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
        cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_VOLTAGE, VOLTAGE_LEVEL_SET, COMMAND_MSG);

        parameters[param_count++] = domain_id;
        parameters[param_count++] = flags;
        parameters[param_count++] = start_voltage;

        val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                         &return_value_count, return_values);

        if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        val_print_return_values(return_value_count, return_values);

        /* STEP 4 : query newly set voltage level for domain */
        val_print(VAL_PRINT_TEST, "\n    [Step 4] Get newly set Voltage ");

        VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
        parameters[param_count++] = domain_id;
        cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_VOLTAGE, VOLTAGE_LEVEL_GET, COMMAND_MSG);
        val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                         &return_value_count, return_values);

        if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        val_print_return_values(return_value_count, return_values);

        voltage = return_values[ATTRIBUTE_OFFSET];
        val_print(VAL_PRINT_TEST, "\n       Voltage        : 0x%08X", voltage);

        /* Check this is voltage we set in step 3. */
        if(val_compare("Voltage Level", voltage, start_voltage))
            return VAL_STATUS_FAIL;

        /* STEP 5 : Set default voltage level for domain */
        val_print(VAL_PRINT_TEST, "\n    [Step 5] Set default Voltage levels");
        VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
        cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_VOLTAGE, VOLTAGE_LEVEL_SET, COMMAND_MSG);

        parameters[param_count++] = domain_id;
        parameters[param_count++] = flags;
        parameters[param_count++] = default_voltage;

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
