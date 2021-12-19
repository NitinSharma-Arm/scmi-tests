/** @file
 * Copyright (c) 2021, Arm Limited or its affiliates. All rights reserved.
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

#define PARAMETER_SIZE     3

#define TEST_NUM  (SCMI_VOLTAGE_TEST_NUM_BASE + 10)
#define TEST_DESC "Voltage set operating mode invalid id               "

uint32_t voltage_invalid_domain_set_operating_mode(void)
{
    size_t param_count;
    uint32_t rsp_msg_hdr;
    uint32_t cmd_msg_hdr;
    size_t return_value_count;
    int32_t status;
    uint32_t parameters[PARAMETER_SIZE];
    uint32_t return_values[MAX_RETURNS_SIZE];
    uint32_t domain_id;

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    domain_id = val_voltage_get_info(NUM_VOLTAGE_DOMAINS, 0) + 1;

    /* Set Architectural operating mode for valid voltage domain */
    val_print(VAL_PRINT_TEST, "\n     [Check 1] Set architectural operating mode");

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    parameters[param_count++] = domain_id;
    parameters[param_count++] = val_voltage_operating_mode_create(ARCHITECTURAL_MODE, 0);

    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_VOLTAGE, VOLTAGE_CONFIG_SET, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    if (val_compare_status(status, SCMI_NOT_FOUND) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    val_print_return_values(return_value_count, return_values);

    /* Set Implementation defined operating mode for valid voltage domain */
    val_print(VAL_PRINT_TEST, "\n     [Check 2] Set implementation operating mode");

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    parameters[param_count++] = domain_id;
    parameters[param_count++] = val_voltage_operating_mode_create(IMPLEMENTATION_DEFINED, 0);

    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_VOLTAGE, VOLTAGE_CONFIG_SET, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    if (val_compare_status(status, SCMI_NOT_FOUND) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    val_print_return_values(return_value_count, return_values);

    return VAL_STATUS_PASS;
}
