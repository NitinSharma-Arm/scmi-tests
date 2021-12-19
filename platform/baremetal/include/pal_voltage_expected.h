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

#ifndef __PAL_VOLTAGE_EXPECTED_H__
#define __PAL_VOLTAGE_EXPECTED_H__

struct arm_scmi_voltage_expected {
    /*
     * This enum is to specify the flag_mask and have to be ensured that
     *  it is in the same order as the below structure elements after
     *  flags_mask member.
     */
    enum {
        VOLTAGE_DOMAIN_PROTOCOL_VERSION = 1,
		    VOLTAGE_NUMBER_DOMAINS,
		    VOLTAGE_DOMAIN_NAME
    } FLAGS;
    /*
     * The expectation is that there is no more
     * than 32 elements specified via this expected results data structure.
     * This mask is used to indicate which of the structure members hold valid
     * data.
     */
    uint32_t flags_mask;
    uint32_t protocol_version;

    uint32_t number_domains;

    int32_t voltage_level;

    int32_t invalid_voltage_level;
    /*
     * This should have the names of the domains in this platform bounded by the
     * number of domains descriptors.
     */
    char  **voltage_domain_name;
};

#endif /* __PAL_VOLTAGE_EXPECTED_H__ */
