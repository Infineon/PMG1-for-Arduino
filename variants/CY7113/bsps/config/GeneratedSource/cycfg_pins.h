/*******************************************************************************
* File Name: cycfg_pins.h
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

#if !defined(CYCFG_PINS_H)
#define CYCFG_PINS_H

#include "cycfg_notices.h"
#include "cy_gpio.h"
#if defined (CY_USING_HAL)
    #include "cyhal_hwmgr.h"
#endif //defined (CY_USING_HAL)
#include "cycfg_routing.h"

#if defined(__cplusplus)
extern "C" {
#endif

#define APP_SWITCH_ENABLED 1U
#define APP_SWITCH_PORT GPIO_PRT3
#define APP_SWITCH_PORT_NUM 3U
#define APP_SWITCH_PIN 3U
#define APP_SWITCH_NUM 3U
#define APP_SWITCH_DRIVEMODE CY_GPIO_DM_PULLUP
#define APP_SWITCH_INIT_DRIVESTATE 1
#ifndef ioss_0_port_3_pin_3_HSIOM
    #define ioss_0_port_3_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define APP_SWITCH_HSIOM ioss_0_port_3_pin_3_HSIOM
#define APP_SWITCH_IRQ ioss_interrupt_gpio_IRQn
#if defined (CY_USING_HAL)
    #define APP_SWITCH_HAL_PORT_PIN P3_3
    #define APP_SWITCH P3_3
    #define APP_SWITCH_HAL_IRQ CYHAL_GPIO_IRQ_NONE
    #define APP_SWITCH_HAL_DIR CYHAL_GPIO_DIR_BIDIRECTIONAL 
    #define APP_SWITCH_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_PULLUP
#endif //defined (CY_USING_HAL)
#define CYBSP_SWCLK_ENABLED 1U
#define CYBSP_SWCLK_PORT GPIO_PRT1
#define CYBSP_SWCLK_PORT_NUM 1U
#define CYBSP_SWCLK_PIN 1U
#define CYBSP_SWCLK_NUM 1U
#define CYBSP_SWCLK_DRIVEMODE CY_GPIO_DM_STRONG
#define CYBSP_SWCLK_INIT_DRIVESTATE 1
#ifndef ioss_0_port_1_pin_1_HSIOM
    #define ioss_0_port_1_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_SWCLK_HSIOM ioss_0_port_1_pin_1_HSIOM
#define CYBSP_SWCLK_IRQ ioss_interrupt_gpio_IRQn
#if defined (CY_USING_HAL)
    #define CYBSP_SWCLK_HAL_PORT_PIN P1_1
    #define CYBSP_SWCLK P1_1
    #define CYBSP_SWCLK_HAL_IRQ CYHAL_GPIO_IRQ_NONE
    #define CYBSP_SWCLK_HAL_DIR CYHAL_GPIO_DIR_BIDIRECTIONAL 
    #define CYBSP_SWCLK_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)
#define CYBSP_SWDIO_ENABLED 1U
#define CYBSP_SWDIO_PORT GPIO_PRT1
#define CYBSP_SWDIO_PORT_NUM 1U
#define CYBSP_SWDIO_PIN 2U
#define CYBSP_SWDIO_NUM 2U
#define CYBSP_SWDIO_DRIVEMODE CY_GPIO_DM_STRONG
#define CYBSP_SWDIO_INIT_DRIVESTATE 1
#ifndef ioss_0_port_1_pin_2_HSIOM
    #define ioss_0_port_1_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_SWDIO_HSIOM ioss_0_port_1_pin_2_HSIOM
#define CYBSP_SWDIO_IRQ ioss_interrupt_gpio_IRQn
#if defined (CY_USING_HAL)
    #define CYBSP_SWDIO_HAL_PORT_PIN P1_2
    #define CYBSP_SWDIO P1_2
    #define CYBSP_SWDIO_HAL_IRQ CYHAL_GPIO_IRQ_NONE
    #define CYBSP_SWDIO_HAL_DIR CYHAL_GPIO_DIR_BIDIRECTIONAL 
    #define CYBSP_SWDIO_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)
#define CYBSP_USER_LED_ENABLED 1U
#define CYBSP_USER_LED_PORT GPIO_PRT5
#define CYBSP_USER_LED_PORT_NUM 5U
#define CYBSP_USER_LED_PIN 5U
#define CYBSP_USER_LED_NUM 5U
#define CYBSP_USER_LED_DRIVEMODE CY_GPIO_DM_STRONG
#define CYBSP_USER_LED_INIT_DRIVESTATE 1
#ifndef ioss_0_port_5_pin_5_HSIOM
    #define ioss_0_port_5_pin_5_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_USER_LED_HSIOM ioss_0_port_5_pin_5_HSIOM
#define CYBSP_USER_LED_IRQ ioss_interrupt_gpio_IRQn
#if defined (CY_USING_HAL)
    #define CYBSP_USER_LED_HAL_PORT_PIN P7_5
    #define CYBSP_USER_LED P7_5
    #define CYBSP_USER_LED_HAL_IRQ CYHAL_GPIO_IRQ_NONE
    #define CYBSP_USER_LED_HAL_DIR CYHAL_GPIO_DIR_BIDIRECTIONAL 
    #define CYBSP_USER_LED_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)

extern const cy_stc_gpio_pin_config_t APP_SWITCH_config;
#if defined (CY_USING_HAL)
    extern const cyhal_resource_inst_t APP_SWITCH_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_SWCLK_config;
#if defined (CY_USING_HAL)
    extern const cyhal_resource_inst_t CYBSP_SWCLK_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_SWDIO_config;
#if defined (CY_USING_HAL)
    extern const cyhal_resource_inst_t CYBSP_SWDIO_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_USER_LED_config;
#if defined (CY_USING_HAL)
    extern const cyhal_resource_inst_t CYBSP_USER_LED_obj;
#endif //defined (CY_USING_HAL)

void init_cycfg_pins(void);
void reserve_cycfg_pins(void);

#if defined(__cplusplus)
}
#endif


#endif /* CYCFG_PINS_H */
