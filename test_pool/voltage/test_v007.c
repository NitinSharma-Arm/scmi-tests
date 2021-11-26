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

#define TEST_NUM  (SCMI_VOLTAGE_TEST_NUM_BASE + 7)
#define TEST_DESC "Voltage describe levels check                   "

#define PARAMETER_SIZE 2

uint32_t voltage_query_describe_levels(void)
{
    int32_t  status, i;
    uint32_t rsp_msg_hdr;
    uint32_t cmd_msg_hdr;
    size_t   param_count;
    uint32_t parameters[PARAMETER_SIZE];
    size_t   return_value_count;
    uint32_t return_values[MAX_RETURNS_SIZE];
    uint32_t num_domains, domain_id;
    uint32_t levels_flag, levels_return_format;
    uint32_t num_remaining_levels, num_levels_retured, level_index, voltage_index;
    uint32_t *voltage_array, start_voltage, end_voltage, step_size, voltage;

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
        num_remaining_levels = 0;
        level_index = 0;
        voltage_index = 0;

        val_print(VAL_PRINT_TEST, "\n       Voltage Domain : %d", domain_id);
        val_print(VAL_PRINT_TEST, "\n    [Check 1] Query Voltage levels");

        /* Get level of voltage supported by domain until, all voltage level
           details are received */
        do{
            VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
            cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_VOLTAGE, VOLTAGE_DESCRIBE_LEVELS, COMMAND_MSG);

            parameters[param_count++] = domain_id;
            level_index += num_remaining_levels;
            parameters[param_count++] = level_index;

            val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                             &return_value_count, return_values);

            if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
                return VAL_STATUS_FAIL;

            if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
                return VAL_STATUS_FAIL;

            val_print_return_values(return_value_count, return_values);

            levels_flag = return_values[LEVEL_FLAG_OFFSET];

            num_remaining_levels = VAL_EXTRACT_BITS(levels_flag, 16, 31);
            val_print(VAL_PRINT_DEBUG, "\n       Remain Levels  : %d", num_remaining_levels);

            if (val_reserved_bits_check_is_zero(VAL_EXTRACT_BITS(levels_flag, 13, 15)) !=
                                                VAL_STATUS_PASS)
                return VAL_STATUS_FAIL;

            levels_return_format =  VAL_EXTRACT_BITS(levels_flag, 12, 12);
            val_print(VAL_PRINT_DEBUG, "\n       Return format  : %d", levels_return_format);
            val_voltage_save_info(VOLTAGE_LEVEL_FORMAT, domain_id, levels_return_format);

            num_levels_retured = VAL_EXTRACT_BITS(levels_flag, 0, 11);
            val_print(VAL_PRINT_DEBUG, "\n       LEVELS RETURN  : %d", num_levels_retured);

            /* Level array/triplet starts from return_values[1]*/
            voltage_array = &return_values[LEVEL_ARRAY_OFFSET];

            /* Verify only 3 values are returned in case level return format is triplet */
            if (levels_return_format == LEVEL_FORMAT_TRIPLET)
            {
                val_print(VAL_PRINT_TEST, "\n    [Check 2] Verify triplet return format", domain_id);
                if(num_levels_retured != 3 || num_remaining_levels!= 0)
                    return VAL_STATUS_FAIL;

                start_voltage = voltage_array[LEVEL_START_OFFSET];
                val_voltage_save_info(VOLTAGE_LEVEL_START, domain_id, start_voltage);
                val_print(VAL_PRINT_DEBUG, "\n       START VOLTAGE  : 0x%08X", start_voltage);

                end_voltage = voltage_array[LEVEL_END_OFFSET];
                val_voltage_save_info(VOLTAGE_LEVEL_END, domain_id, end_voltage);
                val_print(VAL_PRINT_DEBUG, "\n       END VOLTAGE    : 0x%08X", end_voltage);

                step_size = voltage_array[LEVEL_STEP_OFFSET];
                val_voltage_save_info(VOLTAGE_STEP_SIZE, domain_id, step_size);
                val_print(VAL_PRINT_DEBUG, "\n       STEP SIZE      : 0x%08X                 ", step_size);
            }
            /* Save voltage level arrays */
            else
            {
              for (i = 0; i < num_levels_retured; i++)
              {
                  val_print(VAL_PRINT_DEBUG, "\n       Voltage Index  : %d", voltage_index);

                  voltage = voltage_array[i];
                  voltage_index++;
                  val_voltage_save_level(domain_id, voltage_index, voltage);
                  val_print(VAL_PRINT_DEBUG, "\n       Voltage        : %d                    ", voltage);
              }
            }

        }while(num_remaining_levels > 0);

        /* Check Voltage levels for invalid index values. */

        if (levels_return_format == LEVEL_FORMAT_TRIPLET) {
            continue; /* Ignore index for triplet return values */
        }

        /* Voltage describe level for invalid level index */
        level_index = level_index + num_levels_retured + 1;
        val_print(VAL_PRINT_TEST, "\n     [Check 3] Query with invalid index: %d", level_index);

        VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
        parameters[param_count++] = domain_id;
        parameters[param_count++] = level_index; /*Invalid level index*/
        cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_VOLTAGE, VOLTAGE_DESCRIBE_LEVELS, COMMAND_MSG);
        val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                         &return_value_count, return_values);

        if (val_compare_status(status, SCMI_OUT_OF_RANGE) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;
    }

    return VAL_STATUS_PASS;
}
