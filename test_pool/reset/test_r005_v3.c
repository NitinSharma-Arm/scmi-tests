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
#include "val_reset.h"

#define TEST_NUM  (SCMI_RESET_TEST_NUM_BASE + 5)
#define TEST_DESC "Reset domain attributes check                "

uint32_t reset_query_domain_attributes_scmi_v3(void)
{
    int32_t  status;
    uint32_t rsp_msg_hdr;
    uint32_t cmd_msg_hdr;
    size_t   param_count;
    size_t   return_value_count;
    uint32_t return_values[MAX_RETURNS_SIZE];
    uint32_t domain_id, num_domains, attribute;
    uint32_t latency, async_support, notification_support;

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    /* Skip if no domains found*/
    num_domains = val_reset_get_info(NUM_RESET_DOMAINS, 0x0);
    if (num_domains == 0) {
        val_print(VAL_PRINT_ERR, "\n       No reset domains found                      ");
        return VAL_STATUS_SKIP;
    }
    val_print(VAL_PRINT_DEBUG, "\n       NUM DOMAINS                    : %d", num_domains);

    /* Verify the reset IDs are starting from value 0 */
    domain_id = 0;

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    param_count++;
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_RESET, RESET_DOMAIN_ATTRIBUTES, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, &domain_id, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    val_print(VAL_PRINT_TEST, "\n     [Check 2] Verify Domain IDs begin from 0");
    if (val_compare_status(status, SCMI_NOT_FOUND) == VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    /* Check reset domain attributes for valid domain */
    for (domain_id = 0; domain_id < num_domains; domain_id++)
    {
        val_print(VAL_PRINT_TEST, "\n     RESET DOMAIN ID: %d", domain_id);
        val_print(VAL_PRINT_TEST, "\n     [Check 1] Query attributes");

        VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
        param_count++;
        cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_RESET, RESET_DOMAIN_ATTRIBUTES, COMMAND_MSG);
        val_send_message(cmd_msg_hdr, param_count, &domain_id, &rsp_msg_hdr, &status,
                         &return_value_count, return_values);

        if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        val_print_return_values(return_value_count, return_values);

        attribute = return_values[ATTRIBUTE_OFFSET];
        if (val_reserved_bits_check_is_zero(VAL_EXTRACT_BITS(attribute, 0, 29)) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        async_support = VAL_EXTRACT_BITS(attribute, 31, 31);
        notification_support = VAL_EXTRACT_BITS(attribute, 30, 30);
        latency = return_values[LATANCY_OFFSET];

        val_print(VAL_PRINT_DEBUG, "\n       Async reset support            : %d",
                                    async_support);
        val_print(VAL_PRINT_DEBUG, "\n       Reset notifications support    : %d",
                                    notification_support);
        val_print(VAL_PRINT_DEBUG, "\n       Latency                        : %d",
                                    latency);

        /* Save info for further tests*/
        val_reset_save_info(RESET_ASYNC_SUPPORT, domain_id, async_support);
        val_reset_save_info(RESET_NOTIFY_SUPPORT, domain_id, notification_support);

        if (val_compare_str("DOMAIN NAME", (char *)&return_values[NAME_OFFSET],
                            (char *)val_reset_get_expected_name(domain_id), SCMI_NAME_STR_SIZE))
            return VAL_STATUS_FAIL;
    }

    return VAL_STATUS_PASS;
}
