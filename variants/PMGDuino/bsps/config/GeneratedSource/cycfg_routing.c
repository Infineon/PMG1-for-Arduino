/*******************************************************************************
* File Name: cycfg_routing.c
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

#include "cycfg_routing.h"

#include "cy_device_headers.h"

void init_cycfg_routing(void)
{
    SAR0->MUX_SWITCH_HW_CTRL = SAR_MUX_SWITCH_HW_CTRL_MUX_HW_CTRL_P0_Msk |
        SAR_MUX_SWITCH_HW_CTRL_MUX_HW_CTRL_P2_Msk |
        SAR_MUX_SWITCH_HW_CTRL_MUX_HW_CTRL_P3_Msk |
        SAR_MUX_SWITCH_HW_CTRL_MUX_HW_CTRL_P4_Msk |
        SAR_MUX_SWITCH_HW_CTRL_MUX_HW_CTRL_P5_Msk |
        SAR_MUX_SWITCH_HW_CTRL_MUX_HW_CTRL_P6_Msk |
        SAR_MUX_SWITCH_HW_CTRL_MUX_HW_CTRL_P7_Msk |
        SAR_MUX_SWITCH_HW_CTRL_MUX_HW_CTRL_TEMP_Msk |
        SAR_MUX_SWITCH_HW_CTRL_MUX_HW_CTRL_VSSA_Msk;
    SAR0->MUX_SWITCH0 = SAR_MUX_SWITCH0_MUX_FW_P0_VPLUS_Msk |
        SAR_MUX_SWITCH0_MUX_FW_P2_VPLUS_Msk |
        SAR_MUX_SWITCH0_MUX_FW_P3_VPLUS_Msk |
        SAR_MUX_SWITCH0_MUX_FW_P4_VPLUS_Msk |
        SAR_MUX_SWITCH0_MUX_FW_P5_VPLUS_Msk |
        SAR_MUX_SWITCH0_MUX_FW_P6_VPLUS_Msk |
        SAR_MUX_SWITCH0_MUX_FW_P7_VPLUS_Msk |
        SAR_MUX_SWITCH0_MUX_FW_TEMP_VPLUS_Msk |
        SAR_MUX_SWITCH0_MUX_FW_VSSA_VMINUS_Msk;
    SAR0->CTRL |= SAR_CTRL_ENABLED_Msk;
}
