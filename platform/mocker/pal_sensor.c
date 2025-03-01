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

#include <pal_interface.h>
#include <pal_sensor_expected.h>

/*-----------  Common SENSOR API's across platforms ----------*/

uint32_t pal_sensor_get_expected_num_sensors(void)
{
    return num_sensors;
}

uint32_t pal_sensor_get_expected_stats_addr_low(void)
{
    return statistics_address_low_snsr;
}

uint32_t pal_sensor_get_expected_stats_addr_high(void)
{
    return (statistics_address_len_snsr + statistics_address_low_snsr);
}

uint32_t pal_sensor_get_expected_stats_addr_len(void)
{
    return statistics_address_len_snsr;
}

uint8_t pal_sensor_get_expected_async_support(uint32_t sensor_id)
{
    return async_sensor_read_support[sensor_id];
}

uint8_t pal_sensor_get_expected_trip_points(uint32_t sensor_id)
{
    return number_of_trip_points_supported[sensor_id];
}

uint8_t pal_sensor_get_sensor_resolutions(uint32_t sensor_id)
{
    return sensor_resolutions[sensor_id];
}

uint8_t pal_sensor_get_sensor_powers(uint32_t sensor_id)
{
    return sensor_powers[sensor_id];
}

uint8_t pal_sensor_get_number_of_axis_supported(uint32_t sensor_id)
{
    return number_of_axis_supported[sensor_id];
}

uint8_t pal_sensor_get_sensor_axis_supported(uint32_t sensor_id)
{
    return sensor_axis_supported[sensor_id];
}

uint8_t pal_sensor_get_sensor_extended_attributes_support(uint32_t sensor_id)
{
    return sensor_extended_attributes_support[sensor_id];
}

uint8_t pal_sensor_get_sensor_timestamp_support(uint32_t sensor_id)
{
    return sensor_timestamp_support[sensor_id];
}

uint8_t pal_sensor_get_continous_sensor_update_notification_support(uint32_t sensor_id)
{
    return continous_sensor_update_notification_support[sensor_id];
}

uint8_t pal_sensor_get_sensor_update_intervals(uint32_t sensor_id)
{
    return sensor_update_interval_count[sensor_id];
}

uint8_t pal_sensor_get_sensor_update_interval(uint32_t sensor_id)
{
    return sensor_update_intervals[sensor_id];
}
