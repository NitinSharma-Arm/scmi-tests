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

#ifndef __PAL_POWER_DOMAIN_EXPECTED_H__
#define __PAL_POWER_DOMAIN_EXPECTED_H__

/* Expected POWER DOMAIN parameters */
#ifdef POWER_DOMAIN_PROTOCOL
static uint32_t num_power_domains = 0x09;
uint32_t statistics_address_low_pow = 0;
uint32_t statistics_address_len_pow = 0;

#endif

#endif /* __PAL_POWER_DOMAIN_EXPECTED_H__ */
