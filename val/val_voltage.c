/** @file
 * Copyright (c) 2019-2020, Arm Limited or its affiliates. All rights reserved.
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

#include "val_interface.h"
#include "val_voltage.h"

static VOLTAGE_INFO_s g_voltage_info_table;

/**
  @brief   This API is called from app layer to execute voltage tests
  @param   none
  @return  test execution result
**/
uint32_t val_voltage_execute_tests(void)
{
    uint32_t version = 0;

    if (val_agent_check_protocol_support(PROTOCOL_VOLTAGE)) {
        if (RUN_TEST(voltage_query_protocol_version(&version)))
            return VAL_STATUS_FAIL;

        RUN_TEST(voltage_query_protocol_attributes());
        RUN_TEST(voltage_query_mandatory_command_support());
        RUN_TEST(voltage_invalid_messageid_call());
        RUN_TEST(voltage_query_domain_attributes());
        RUN_TEST(voltage_query_domain_attributes_invalid_id());
        RUN_TEST(voltage_query_describe_levels());
        RUN_TEST(voltage_query_describe_level_invalid_voltage_id());
        RUN_TEST(voltage_set_operating_mode());
        RUN_TEST(voltage_invalid_domain_set_operating_mode());
        RUN_TEST(voltage_query_config_operating_mode());
        RUN_TEST(voltage_set_voltage_level());
        RUN_TEST(voltage_set_voltage_invalid_domain());
        RUN_TEST(voltage_set_invalid_voltage_level());
        RUN_TEST(voltage_query_current_level());

    }
    else
        val_print(VAL_PRINT_ERR, "\n Calling agent have no access to Voltage protocol");

    return VAL_STATUS_PASS;
}


/**
  @brief   This API is used for checking num of voltage domain
  @param   none
  @return  num of voltage domain
**/
uint32_t val_voltage_get_expected_num_domains(void)
{
    return pal_voltage_get_expected_num_domains();
}

/**
  @brief   This API is used to set voltage protocol info
           1. Caller       -  Test Suite.
           2. Prerequisite -  Voltage protocol info table.
  @param   param_identifier  id of parameter which will be set
  @param   domain_id         voltage domain ID.
  @param   param_value       value of parameter
  @return  none
**/
void val_voltage_save_info(uint32_t param_identifier, uint32_t domain_id, uint32_t param_value)
{
    switch (param_identifier)
    {
    case NUM_VOLTAGE_DOMAINS:
        g_voltage_info_table.num_domains = param_value;
        break;
    case VOLTAGE_DOMAIN_CONFIG:
        g_voltage_info_table.domain_info[domain_id].config = param_value;
        break;
    case VOLTAGE_LEVEL_FORMAT:
        g_voltage_info_table.domain_info[domain_id].voltage_level_format = param_value;
        break;
    case VOLTAGE_LEVEL_START:
        g_voltage_info_table.domain_info[domain_id].lowest_voltage_level = param_value;
        break;
    case VOLTAGE_LEVEL_END:
        g_voltage_info_table.domain_info[domain_id].highest_voltage_level = param_value;
        break;
    case VOLTAGE_STEP_SIZE:
        g_voltage_info_table.domain_info[domain_id].highest_voltage_level = param_value;
        break;
    case OPERATIONAL_VOLTAGE:
        g_voltage_info_table.domain_info[domain_id].operational_voltage = param_value;
        break;
    default:
        val_print(VAL_PRINT_ERR, "\nUnidentified parameter %d", param_identifier);
    }
}

/**
  @brief   This API is used to save voltage levels for
            particular domain voltage ID.
  @param    domain_id          voltage domain ID
  @param    level_index        voltage level Index
  @return   none
**/
void val_voltage_save_level(uint32_t domain_id, uint32_t level_index, uint32_t voltage)
{
    g_voltage_info_table.domain_info[domain_id].voltage_array[level_index] = voltage;
}


/**
  @brief   This API is used to get voltage levels for
            particular domain voltage ID.
  @param    domain_id          voltage domain ID
  @param    level_index        voltage level Index
  @return   none
**/
uint32_t val_voltage_get_level(uint32_t domain_id, uint32_t level_index)
{
    return g_voltage_info_table.domain_info[domain_id].voltage_array[level_index];
}


/**
  @brief   This API is used to get voltage protocol info
           1. Caller       -  Test Suite.
           2. Prerequisite -  Voltage protocol info table.
  @param   param_identifier  id of parameter which will be set
  @return  param_value       value of the parameter
**/
uint32_t val_voltage_get_info(uint32_t param_identifier, uint32_t domain_id)
{
    uint32_t param_value = 0;

    switch (param_identifier)
    {
    case NUM_VOLTAGE_DOMAINS:
        param_value = g_voltage_info_table.num_domains;
        break;
    case VOLTAGE_DOMAIN_CONFIG:
        param_value = g_voltage_info_table.domain_info[domain_id].config;
        break;
    case VOLTAGE_LEVEL_FORMAT:
        param_value = g_voltage_info_table.domain_info[domain_id].voltage_level_format;
        break;
    case VOLTAGE_LEVEL_START:
        param_value = g_voltage_info_table.domain_info[domain_id].lowest_voltage_level;
        break;
    case VOLTAGE_LEVEL_END:
        param_value = g_voltage_info_table.domain_info[domain_id].highest_voltage_level;
        break;
    case VOLTAGE_STEP_SIZE:
        param_value = g_voltage_info_table.domain_info[domain_id].highest_voltage_level;
        break;
    default:
        val_print(VAL_PRINT_ERR, "\nUnidentified parameter %d", param_identifier);
    }

    return param_value;
}

/**
  @brief   This API is used for checking voltage domain name
  @param   domain id
  @return  voltage domain name
**/
uint8_t *val_voltage_get_expected_name(uint32_t domain_id)
{
    return pal_voltage_get_expected_name(domain_id);
}

/**
  @brief   This function forms Voltage Operating mode config
  @param   mode_type    voltage domain mode type
  @param   mode_id      voltage domain mode ID
  @return  32 bit voltage operating mode
**/
uint32_t val_voltage_operating_mode_create(uint32_t mode_type, uint32_t mode_id)
{
    uint32_t config = 0;

    config = config | (mode_type << 3);
    config = config | mode_id;

    return config;
}
#endif
