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

#ifdef VOLTAGE_PROTOCOL

#include "pal_interface.h"
#include "pal_voltage_expected.h"

extern void pal_voltage_set_expected(const void *);
static struct arm_scmi_voltage_expected *g_voltage_info;


/**
  @brief   Sets voltage domain protocol info
  @param   platform voltage domain protocol info
  @return  none
**/
void pal_voltage_set_expected(const void *info)
{
    if (info == NULL)
        return;

    g_voltage_info = (struct arm_scmi_voltage_expected *)info;
}

/*-----------  Common PAL API's across platforms ----------*/

/**
  @brief   This API is used for checking num of voltage domain
  @param   none
  @return  num of voltage domain
**/
uint32_t pal_voltage_get_expected_num_domains(void)
{
    if (g_voltage_info == NULL)
        return 0;

    return g_voltage_info->number_domains;
}

/**
  @brief   This API is used for checking voltage domain name
  @param   domain id
  @return  voltage domain name
**/
uint8_t *pal_voltage_get_expected_name(uint32_t domain_id)
{
    if (g_voltage_info == NULL)
        return NULL;

    return (uint8_t *)g_voltage_info->voltage_domain_name[domain_id];
}

#endif
