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

#ifndef __VAL_VOLTAGE_H__
#define __VAL_VOLTAGE_H__

#define ATTRIBUTE_OFFSET              0
#define LEVEL_FLAG_OFFSET             0
#define LEVEL_ARRAY_OFFSET            1
#define LEVEL_START_OFFSET            0
#define LEVEL_END_OFFSET              1
#define LEVEL_STEP_OFFSET             2
#define START_LEVEL_INDEX             0
#define SECOND_LEVEL_INDEX            1

#define MAX_NUM_OF_VOLTAGE_DOMAINS   32
#define MAX_NUMBER_VOLTAGE_LEVELS    32

typedef enum {
    ARCHITECTURAL_MODE,
    IMPLEMENTATION_DEFINED
} VOLTAGE_OPERATING_MODE_TYPE;

typedef enum{
    LEVEL_FORMAT_ARRAY,
    LEVEL_FORMAT_TRIPLET
} VOLTAGE_LEVEL_RETURN_FORMAT;

typedef struct {
    uint32_t config;
    uint32_t voltage_level_format;
    uint32_t lowest_voltage_level;
    uint32_t highest_voltage_level;
    uint32_t step_voltage_level;
    uint32_t voltage_array[MAX_NUMBER_VOLTAGE_LEVELS];
    uint32_t operational_voltage;
} VOLTAGE_DOMAIN_INFO_s;

typedef struct {
    uint32_t num_domains;
    VOLTAGE_DOMAIN_INFO_s domain_info[MAX_NUM_OF_VOLTAGE_DOMAINS];
} VOLTAGE_INFO_s;

/* Common Tests */
uint32_t voltage_query_protocol_version(uint32_t *version);
uint32_t voltage_query_protocol_attributes(void);
uint32_t voltage_query_mandatory_command_support(void);
uint32_t voltage_invalid_messageid_call(void);
uint32_t voltage_query_domain_attributes(void);
uint32_t voltage_query_domain_attributes_invalid_id(void);
uint32_t voltage_query_describe_levels(void);
uint32_t voltage_query_describe_level_invalid_voltage_id(void);
uint32_t voltage_set_operating_mode(void);
uint32_t voltage_invalid_domain_set_operating_mode(void);
uint32_t voltage_query_config_operating_mode(void);
uint32_t voltage_set_voltage_level(void);
uint32_t voltage_set_voltage_invalid_domain(void);
uint32_t voltage_set_invalid_voltage_level(void);
uint32_t voltage_query_current_level(void);

uint32_t val_voltage_get_expected_num_domains(void);
uint8_t *val_voltage_get_expected_name(uint32_t domain_id);
#endif
