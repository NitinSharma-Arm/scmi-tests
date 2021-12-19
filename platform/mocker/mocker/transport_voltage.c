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
#include <voltage_common.h>
#include <voltage_protocol.h>
#include <pal_voltage_expected.h>

struct arm_scmi_voltage_protocol voltage_protocol;

void fill_voltage_protocol()
{
    voltage_protocol.protocol_version = VOLTAGE_VERSION;
    voltage_protocol.number_domains = num_voltage_domains;
    voltage_protocol.voltage_domain_name = voltage_domain_names;
}

void voltage_send_message(uint32_t message_id, uint32_t parameter_count,
        const uint32_t *parameters, int32_t *status,
        size_t *return_values_count, uint32_t *return_values)
{

      switch(message_id)
      {
      case VOLTAGE_PROTO_VER_MSG_ID:
          *status = SCMI_STATUS_SUCCESS;
          *return_values_count = 1;
          return_values[0] = voltage_protocol.protocol_version;
          break;
      /* Other Message IDs pending implementation */

      default:
          *status = SCMI_STATUS_NOT_FOUND;
          break;
      }
}
