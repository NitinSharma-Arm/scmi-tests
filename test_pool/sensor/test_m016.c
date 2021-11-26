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
#include "val_sensor.h"

#define TEST_NUM  (SCMI_SENSOR_TEST_NUM_BASE + 16)
#define TEST_DESC "Sensor axis description check                "

#define MAX_PARAMETER_SIZE        2
#define SENSOR_AXIS_DESC_LEN      12
#define EXTENDED_ATTR_SUPPORTED   1

uint32_t sensor_axis_description_check(void)
{
    int32_t  status;
    uint32_t rsp_msg_hdr;
    uint32_t cmd_msg_hdr;
    size_t   param_count;
    size_t   return_value_count;
    uint32_t return_values[MAX_RETURNS_SIZE];
    uint32_t parameters[MAX_PARAMETER_SIZE];
    uint32_t sensor_id, num_sensors, num_axis_flag;
    uint32_t axis_desc_index = 0;
    uint32_t num_desc_retured = 0;
    uint32_t *sensor_axis_desc;
    uint32_t axis_id, i;
    uint32_t ext_attr_support;
    uint32_t num_remaining_desc;

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    num_sensors = val_sensor_get_info(NUM_SENSORS);
    if (num_sensors == 0) {
        val_print(VAL_PRINT_ERR, "\n       No Sensor found                             ");
        return VAL_STATUS_SKIP;
    }
    val_print(VAL_PRINT_DEBUG, "\n       NUM SENSORS     : %d", num_sensors);

    /* Reading sensor axis properties for all sensors. */
    for (sensor_id = 0; sensor_id < num_sensors; sensor_id++)
    {
        val_print(VAL_PRINT_TEST, "\n     SENSOR ID: %d", sensor_id);
        val_print(VAL_PRINT_TEST, "\n     [Check 1] Get sensor axis properties");

        axis_desc_index = 0;
        num_desc_retured = 0;

        do{
              axis_desc_index += num_desc_retured;

              VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
              parameters[param_count++] = sensor_id;
              parameters[param_count++] = axis_desc_index;

              cmd_msg_hdr = val_msg_hdr_create(
                                PROTOCOL_SENSOR, SENSOR_AXIS_DESCRIPTION_GET, COMMAND_MSG);
              val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                               &return_value_count, return_values);

              if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
                   return VAL_STATUS_FAIL;

              if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
                   return VAL_STATUS_FAIL;

              val_print_return_values(return_value_count, return_values);

              /* Check num_axis_flag values */
              num_axis_flag = return_values[NUM_AXIS_FLAG_OFFSET];
              num_remaining_desc = VAL_EXTRACT_BITS(num_axis_flag, 26, 31);
              val_print(VAL_PRINT_DEBUG, "\n       NUM OF REMAIN AXIS DESC: %d",
                            num_remaining_desc);

              if (val_reserved_bits_check_is_zero(VAL_EXTRACT_BITS(num_axis_flag, 6, 25)) !=
                      VAL_STATUS_PASS)
                  return VAL_STATUS_FAIL;

              num_desc_retured = VAL_EXTRACT_BITS(num_axis_flag, 0, 5);
              val_print(VAL_PRINT_DEBUG, "\n       NUM OF AXIS DESC RETURN: %d",
                            num_desc_retured);

              /* Get sensor Axis details */
              sensor_axis_desc = &return_values[SENSOR_AXIS_DESC_OFFSET];
              for (i = 0; i < num_desc_retured; i++)
              {
                  axis_id = sensor_axis_desc[i * SENSOR_AXIS_DESC_LEN];
                  val_print(VAL_PRINT_DEBUG, "\n       Axis id                : %d", axis_id);

                  /* Sensor  axis_attributes_low */
                  if (val_reserved_bits_check_is_zero(VAL_EXTRACT_BITS(
                      sensor_axis_desc[1 + (i * SENSOR_AXIS_DESC_LEN)], 9, 31)) != VAL_STATUS_PASS)
                      return VAL_STATUS_FAIL;

                  ext_attr_support = VAL_EXTRACT_BITS(
                      sensor_axis_desc[1 + (i * SENSOR_AXIS_DESC_LEN)], 8, 8);
                  val_print(VAL_PRINT_TEST, "\n     [Check 2] Check extended attr supported");
                  val_print(VAL_PRINT_TEST, "\n       Extended Attr Support  : %d",
                            ext_attr_support);

                  if (val_compare("Extended Attr Support   ",
                          ext_attr_support, EXTENDED_ATTR_SUPPORTED))
                      return VAL_STATUS_FAIL;


                  if (val_reserved_bits_check_is_zero(VAL_EXTRACT_BITS(
                      sensor_axis_desc[1 + (i * SENSOR_AXIS_DESC_LEN)], 0, 7)) != VAL_STATUS_PASS)
                      return VAL_STATUS_FAIL;

                  /* Sensor axis_attributes_high */
                  if (val_reserved_bits_check_is_zero(VAL_EXTRACT_BITS(
                      sensor_axis_desc[2 + (i * SENSOR_AXIS_DESC_LEN)], 16, 31)) != VAL_STATUS_PASS)
                      return VAL_STATUS_FAIL;

                  val_print(VAL_PRINT_DEBUG, "\n       Sensor Unit Exponent   : %d",
                      VAL_EXTRACT_BITS(sensor_axis_desc[2 + (i * SENSOR_AXIS_DESC_LEN)], 11, 15));

                  val_print(VAL_PRINT_DEBUG, "\n       Sensor Type            : %d",
                      VAL_EXTRACT_BITS(sensor_axis_desc[2 + (i * SENSOR_AXIS_DESC_LEN)], 0, 7));

                  if (val_reserved_bits_check_is_zero(VAL_EXTRACT_BITS(
                      sensor_axis_desc[2 + (i * SENSOR_AXIS_DESC_LEN)], 8, 10)) != VAL_STATUS_PASS)
                      return VAL_STATUS_FAIL;

                  /* Sensor Axis name */
                  val_print(VAL_PRINT_DEBUG, "\n       Sensor Axis name       : %s",
                      &sensor_axis_desc[3 + (i * SENSOR_AXIS_DESC_LEN)]);

                  /* Sensor axis_resolution */
                  val_print(VAL_PRINT_DEBUG, "\n       Sensor Resolution exp  : %d",
                      VAL_EXTRACT_BITS(sensor_axis_desc[7 + (i * SENSOR_AXIS_DESC_LEN)], 27, 31));

                  val_print(VAL_PRINT_DEBUG, "\n       Sensor Resolution      : %d",
                      VAL_EXTRACT_BITS(sensor_axis_desc[7 + (i * SENSOR_AXIS_DESC_LEN)], 0, 26));

                  /* Sensor Axis Min Range */
                  val_print(VAL_PRINT_DEBUG, "\n       Sensor Axis Min Range Low   : %d",
                              sensor_axis_desc[8 + (i * SENSOR_AXIS_DESC_LEN)]);
                  val_print(VAL_PRINT_DEBUG, "\n       Sensor Axis Min Range High  : %d",
                              sensor_axis_desc[9 + (i * SENSOR_AXIS_DESC_LEN)]);

                  /* Sensor Axis Max Range */
                  val_print(VAL_PRINT_DEBUG, "\n       Sensor Axis Max Range Low   : %d",
                              sensor_axis_desc[10 + (i * SENSOR_AXIS_DESC_LEN)]);
                  val_print(VAL_PRINT_DEBUG, "\n       Sensor Axis Max Range High  : %d",
                              sensor_axis_desc[11 + (i * SENSOR_AXIS_DESC_LEN)]);
              }

        } while (num_remaining_desc > 0);
    }

    return VAL_STATUS_PASS;
}
