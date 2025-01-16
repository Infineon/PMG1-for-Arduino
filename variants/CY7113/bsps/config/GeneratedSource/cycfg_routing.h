/*******************************************************************************
* File Name: cycfg_routing.h
*
* Description:
* Establishes all necessary connections between hardware elements.
* This file was automatically generated and should not be modified.
* Configurator Backend 3.0.0
* device-db 4.7.0.4251
* mtb-pdl-cat2 2.5.0.9691
*
********************************************************************************
* Copyright 2023 Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation.
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
********************************************************************************/

#if !defined(CYCFG_ROUTING_H)
#define CYCFG_ROUTING_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "cycfg_notices.h"
void init_cycfg_routing(void);

#define ioss_0_port_1_pin_1_HSIOM P1_1_CPUSS_SWD_CLK
#define ioss_0_port_1_pin_2_HSIOM P1_2_CPUSS_SWD_DATA
#define ioss_0_port_2_pin_1_ANALOG P2_1_PASS0_SARMUX_PADS1
#define ioss_0_port_2_pin_2_ANALOG P2_2_PASS0_SARMUX_PADS2

#define SAR0_VPLUS0_PIN 2
#define SAR0_VPLUS0_PORT 0
#define SAR0_VPLUS1_PIN 1
#define SAR0_VPLUS1_PORT 0
#define SAR0_VPLUS16_PIN 0
#define SAR0_VPLUS16_PORT 7

#if defined(__cplusplus)
}
#endif


#endif /* CYCFG_ROUTING_H */
