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

#include <pal_interface.h>
#include <pal_voltage_expected.h>

/**
  @brief   This API is used for checking num of voltage  domain
  @param   none
  @return  num of voltage domain
**/
uint32_t pal_voltage_get_expected_num_domains(void)
{
    return num_voltage_domains;
}

/**
  @brief   This API is used for checking voltage domain name
  @param   domain id
  @return  voltage domain name
**/
uint8_t *pal_voltage_get_expected_name(uint32_t domain_id)
{
    return (uint8_t *)voltage_domain_names[domain_id];
}
