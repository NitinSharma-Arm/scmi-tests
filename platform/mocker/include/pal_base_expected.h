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

#ifndef __PAL_BASE_EXPECTED_H__
#define __PAL_BASE_EXPECTED_H__

#include <pal_platform.h>

static uint8_t supported_protocols[] = {
    POWER_DOMAIN_PROTOCOL_ID,
    SYSTEM_POWER_PROTOCOL_ID,
    PERFORMANCE_PROTOCOL_ID,
    CLOCK_PROTOCOL_ID,
    SENSOR_PROTOCOL_ID,
    RESET_PROTOCOL_ID,
    VOLTAGE_PROTOCOL_ID,
};

/* Expected BASE parameters */

static char *agents[] = {
    "OSPM", "PSCI"
};

static char *vendor_name = "mocker";
static char *subvendor_name = "mocker";
static uint32_t implementation_version = 1;

#endif /* __PAL_BASE_EXPECTED_H__ */
