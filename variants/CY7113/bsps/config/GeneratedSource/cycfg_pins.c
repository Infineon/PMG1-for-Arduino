/*******************************************************************************
* File Name: cycfg_pins.c
*
* Description:
* Pin configuration
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

#include "cycfg_pins.h"

const cy_stc_gpio_pin_config_t APP_SWITCH_config = 
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_PULLUP,
    .hsiom = APP_SWITCH_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};
#if defined (CY_USING_HAL)
    const cyhal_resource_inst_t APP_SWITCH_obj = 
    {
        .type = CYHAL_RSC_GPIO,
        .block_num = APP_SWITCH_PORT_NUM,
        .channel_num = APP_SWITCH_PIN,
    };
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t CYBSP_SWCLK_config = 
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_SWCLK_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};
#if defined (CY_USING_HAL)
    const cyhal_resource_inst_t CYBSP_SWCLK_obj = 
    {
        .type = CYHAL_RSC_GPIO,
        .block_num = CYBSP_SWCLK_PORT_NUM,
        .channel_num = CYBSP_SWCLK_PIN,
    };
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t CYBSP_SWDIO_config = 
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_SWDIO_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};
#if defined (CY_USING_HAL)
    const cyhal_resource_inst_t CYBSP_SWDIO_obj = 
    {
        .type = CYHAL_RSC_GPIO,
        .block_num = CYBSP_SWDIO_PORT_NUM,
        .channel_num = CYBSP_SWDIO_PIN,
    };
#endif //defined (CY_USING_HAL)

const cy_stc_gpio_pin_config_t CYBSP_USER_LED_config = 
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_USER_LED_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};
#if defined (CY_USING_HAL)
    const cyhal_resource_inst_t CYBSP_USER_LED_obj = 
    {
        .type = CYHAL_RSC_GPIO,
        .block_num = CYBSP_USER_LED_PORT_NUM,
        .channel_num = CYBSP_USER_LED_PIN,
    };
#endif //defined (CY_USING_HAL)



void init_cycfg_pins(void)
{
    Cy_GPIO_Pin_Init(APP_SWITCH_PORT, APP_SWITCH_PIN, &APP_SWITCH_config);
    Cy_GPIO_Pin_Init(CYBSP_SWCLK_PORT, CYBSP_SWCLK_PIN, &CYBSP_SWCLK_config);
    Cy_GPIO_Pin_Init(CYBSP_SWDIO_PORT, CYBSP_SWDIO_PIN, &CYBSP_SWDIO_config);
    Cy_GPIO_Pin_Init(CYBSP_USER_LED_PORT, CYBSP_USER_LED_PIN, &CYBSP_USER_LED_config);
}

void reserve_cycfg_pins(void)
{
#if defined (CY_USING_HAL)
    cyhal_hwmgr_reserve(&APP_SWITCH_obj);
    cyhal_hwmgr_reserve(&CYBSP_SWCLK_obj);
    cyhal_hwmgr_reserve(&CYBSP_SWDIO_obj);
    cyhal_hwmgr_reserve(&ioss_0_port_2_pin_1_obj);
    cyhal_hwmgr_reserve(&ioss_0_port_2_pin_2_obj);
    cyhal_hwmgr_reserve(&PFET_SNK_CTRL_P0_obj);
    cyhal_hwmgr_reserve(&PFET_SNK_CTRL_P1_obj);
    cyhal_hwmgr_reserve(&CYBSP_USER_LED_obj);
#endif //defined (CY_USING_HAL)
}
