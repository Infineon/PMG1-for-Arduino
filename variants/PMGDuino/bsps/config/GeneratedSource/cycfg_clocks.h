/*******************************************************************************
* File Name: cycfg_clocks.h
*
* Description:
* Clock configuration
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

#if !defined(CYCFG_CLOCKS_H)
#define CYCFG_CLOCKS_H

#include "cycfg_notices.h"
#include "cy_sysclk.h"
#if defined (CY_USING_HAL)
    #include "cyhal_hwmgr.h"
#endif //defined (CY_USING_HAL)

#if defined(__cplusplus)
extern "C" {
#endif

#define CLK_PDRX_ENABLED 1U
#define CLK_PDRX_HW CY_SYSCLK_DIV_8_BIT
#define CLK_PDRX_NUM 0U
#define CLK_SYS_TCNT_ENABLED 1U
#define CLK_SYS_TCNT_HW CY_SYSCLK_DIV_8_BIT
#define CLK_SYS_TCNT_NUM 10U
#define CLK_PDTX_ENABLED 1U
#define CLK_PDTX_HW CY_SYSCLK_DIV_8_BIT
#define CLK_PDTX_NUM 1U
#define CLK_PDSAR_ENABLED 1U
#define CLK_PDSAR_HW CY_SYSCLK_DIV_8_BIT
#define CLK_PDSAR_NUM 2U
#define CLK_FILTER1_ENABLED 1U
#define CLK_FILTER1_HW CY_SYSCLK_DIV_8_BIT
#define CLK_FILTER1_NUM 3U
#define CLK_PWM_ENABLED 1U
#define CLK_PWM_HW CY_SYSCLK_DIV_8_BIT
#define CLK_PWM_NUM 4U
#define CLK_PMIC_ENABLED 1U
#define CLK_PMIC_HW CY_SYSCLK_DIV_8_BIT
#define CLK_PMIC_NUM 5U
#define CLK_SPI_ENABLED 1U
#define CLK_SPI_HW CY_SYSCLK_DIV_8_BIT
#define CLK_SPI_NUM 6U
#define CLK_I2C_ENABLED 1U
#define CLK_I2C_HW CY_SYSCLK_DIV_8_BIT
#define CLK_I2C_NUM 7U
#define CLK_ADC_ENABLED 1U
#define CLK_ADC_HW CY_SYSCLK_DIV_8_BIT
#define CLK_ADC_NUM 8U
#define CLK_PMIC_HW CY_SYSCLK_DIV_8_BIT
#define CLK_PMIC_NUM 5U
#define CLK_UART_HW CY_SYSCLK_DIV_16_BIT
#define CLK_UART_NUM 0U

#if defined (CY_USING_HAL)
    extern const cyhal_resource_inst_t CLK_PDRX_obj;
    extern const cyhal_resource_inst_t CLK_SYS_TCNT_obj;
    extern const cyhal_resource_inst_t CLK_PDTX_obj;
    extern const cyhal_resource_inst_t CLK_PDSAR_obj;
    extern const cyhal_resource_inst_t CLK_FILTER1_obj;
    extern const cyhal_resource_inst_t CLK_PWM_obj;
    extern const cyhal_resource_inst_t CLK_PMIC_obj;
    extern const cyhal_resource_inst_t CLK_SPI_obj;
    extern const cyhal_resource_inst_t CLK_I2C_obj;
    extern const cyhal_resource_inst_t CLK_ADC_obj;
#endif //defined (CY_USING_HAL)

void init_cycfg_clocks(void);
void reserve_cycfg_clocks(void);

#if defined(__cplusplus)
}
#endif


#endif /* CYCFG_CLOCKS_H */
