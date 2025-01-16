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
#define ioss_0_port_2_pin_0_ANALOG P2_0_PASS0_SARMUX_PADS0
#define ioss_0_port_2_pin_2_ANALOG P2_2_PASS0_SARMUX_PADS2
#define ioss_0_port_2_pin_3_ANALOG P2_3_PASS0_SARMUX_PADS3
#define ioss_0_port_2_pin_4_ANALOG P2_4_PASS0_SARMUX_PADS4
#define ioss_0_port_2_pin_5_ANALOG P2_5_PASS0_SARMUX_PADS5
#define ioss_0_port_2_pin_6_ANALOG P2_6_PASS0_SARMUX_PADS6
#define ioss_0_port_2_pin_7_ANALOG P2_7_PASS0_SARMUX_PADS7
#define ioss_0_port_4_pin_0_HSIOM P4_0_SCB0_UART_RX
#define ioss_0_port_4_pin_1_HSIOM P4_1_SCB0_UART_TX

#define SAR0_VPLUS0_PIN 4
#define SAR0_VPLUS0_PORT 0
#define SAR0_VPLUS1_PIN 7
#define SAR0_VPLUS1_PORT 0
#define SAR0_VPLUS2_PIN 2
#define SAR0_VPLUS2_PORT 0
#define SAR0_VPLUS3_PIN 5
#define SAR0_VPLUS3_PORT 0
#define SAR0_VPLUS4_PIN 3
#define SAR0_VPLUS4_PORT 0
#define SAR0_VPLUS5_PIN 0
#define SAR0_VPLUS5_PORT 0
#define SAR0_VPLUS6_PIN 6
#define SAR0_VPLUS6_PORT 0
#define SAR0_VPLUS16_PIN 0
#define SAR0_VPLUS16_PORT 7

#if defined(__cplusplus)
}
#endif


#endif /* CYCFG_ROUTING_H */
