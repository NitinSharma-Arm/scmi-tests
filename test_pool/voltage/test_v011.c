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

#define TEST_NUM  (SCMI_VOLTAGE_TEST_NUM_BASE + 11)
#define TEST_DESC "Voltage get operating mode                  "

#define RETURN_VALUE_COUNT    1
#define PARAMETER_SIZE        2

uint32_t voltage_query_config_operating_mode(void)
{
    size_t param_count;
    uint32_t rsp_msg_hdr;
    uint32_t cmd_msg_hdr;
    size_t return_value_count;
    int32_t status;
    uint32_t parameters[PARAMETER_SIZE];
    uint32_t return_values[MAX_RETURNS_SIZE];
    uint32_t attributes, num_domains, domain_id, voltage_config;

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    num_domains = val_voltage_get_info(NUM_VOLTAGE_DOMAINS, 0);
    if (num_domains == 0) {
        val_print(VAL_PRINT_ERR, "\n       No Voltage Domains found                   ");
        return VAL_STATUS_SKIP;
    }
    val_print(VAL_PRINT_DEBUG, "\n       Voltage Domains: %d", num_domains);

    /* Check Voltage config for valid voltage domains */
    for (domain_id = 0; domain_id < num_domains; domain_id++)
    {
        /* Query Configuration of a voltage domain */
        val_print(VAL_PRINT_TEST, "\n       Voltage ID     : %d", domain_id);
        val_print(VAL_PRINT_TEST, "\n   [Check1] Query voltage config set");

        VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
        parameters[param_count++] = domain_id;

        cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_VOLTAGE, VOLTAGE_CONFIG_GET, COMMAND_MSG);
        val_send_message(cmd_msg_hdr, param_count, parameters,&rsp_msg_hdr, &status,
                         &return_value_count, return_values);

        if(val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        if(val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        val_print_return_values(return_value_count, return_values);

        attributes = return_values[ATTRIBUTE_OFFSET];
        if (val_reserved_bits_check_is_zero(VAL_EXTRACT_BITS(attributes, 4, 31) != VAL_STATUS_PASS))
            return VAL_STATUS_FAIL;

        if(val_compare_return_count(return_value_count, RETURN_VALUE_COUNT))
            return VAL_STATUS_FAIL;

        /* Get the voltage configuration set for domain ID */
        voltage_config = VAL_EXTRACT_BITS(attributes, 0, 3);
        val_print(VAL_PRINT_TEST, "\n       Voltage config : %d", voltage_config);

        val_voltage_save_info(VOLTAGE_DOMAIN_CONFIG, domain_id, voltage_config);
    }

    /* Check voltage config for non valid domains */
    domain_id = val_voltage_get_info(NUM_VOLTAGE_DOMAINS, 0) + 2;

    val_print(VAL_PRINT_TEST, "\n     [Check 2] Query Voltage config for invalid domains");

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    parameters[param_count++] = domain_id;

    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_VOLTAGE, VOLTAGE_CONFIG_GET, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    if (val_compare_status(status, SCMI_NOT_FOUND) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if(val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    val_print_return_values(return_value_count, return_values);

    return VAL_STATUS_PASS;
}
