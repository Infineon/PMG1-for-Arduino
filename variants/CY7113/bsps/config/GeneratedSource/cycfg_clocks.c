/*******************************************************************************
* File Name: cycfg_clocks.c
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

#include "cycfg_clocks.h"

#if defined (CY_USING_HAL)
    const cyhal_resource_inst_t CLK_PDRX_obj = 
    {
        .type = CYHAL_RSC_CLOCK,
        .block_num = CLK_PDRX_HW,
        .channel_num = CLK_PDRX_NUM,
    };
    const cyhal_resource_inst_t CLK_SYS_TCNT_obj = 
    {
        .type = CYHAL_RSC_CLOCK,
        .block_num = CLK_SYS_TCNT_HW,
        .channel_num = CLK_SYS_TCNT_NUM,
    };
    const cyhal_resource_inst_t CLK_DBG_UART_obj = 
    {
        .type = CYHAL_RSC_CLOCK,
        .block_num = CLK_DBG_UART_HW,
        .channel_num = CLK_DBG_UART_NUM,
    };
    const cyhal_resource_inst_t CLK_PDTX_obj = 
    {
        .type = CYHAL_RSC_CLOCK,
        .block_num = CLK_PDTX_HW,
        .channel_num = CLK_PDTX_NUM,
    };
    const cyhal_resource_inst_t CLK_PDSAR_obj = 
    {
        .type = CYHAL_RSC_CLOCK,
        .block_num = CLK_PDSAR_HW,
        .channel_num = CLK_PDSAR_NUM,
    };
    const cyhal_resource_inst_t CLK_FILTER1_obj = 
    {
        .type = CYHAL_RSC_CLOCK,
        .block_num = CLK_FILTER1_HW,
        .channel_num = CLK_FILTER1_NUM,
    };
    const cyhal_resource_inst_t CLK_PWM_obj = 
    {
        .type = CYHAL_RSC_CLOCK,
        .block_num = CLK_PWM_HW,
        .channel_num = CLK_PWM_NUM,
    };
    const cyhal_resource_inst_t CLK_UART_obj = 
    {
        .type = CYHAL_RSC_CLOCK,
        .block_num = CLK_UART_HW,
        .channel_num = CLK_UART_NUM,
    };
    const cyhal_resource_inst_t CLK_SPI_obj = 
    {
        .type = CYHAL_RSC_CLOCK,
        .block_num = CLK_SPI_HW,
        .channel_num = CLK_SPI_NUM,
    };
    const cyhal_resource_inst_t CLK_I2C_obj = 
    {
        .type = CYHAL_RSC_CLOCK,
        .block_num = CLK_I2C_HW,
        .channel_num = CLK_I2C_NUM,
    };
    const cyhal_resource_inst_t CLK_ADC_obj = 
    {
        .type = CYHAL_RSC_CLOCK,
        .block_num = CLK_ADC_HW,
        .channel_num = CLK_ADC_NUM,
    };
#endif //defined (CY_USING_HAL)


void init_cycfg_clocks(void)
{
    Cy_SysClk_PeriphDisableDivider(CY_SYSCLK_DIV_8_BIT, 0U);
    Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_8_BIT, 0U, 3U);
    Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_8_BIT, 0U);
    Cy_SysClk_PeriphDisableDivider(CY_SYSCLK_DIV_8_BIT, 10U);
    Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_8_BIT, 10U, 47U);
    Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_8_BIT, 10U);
    Cy_SysClk_PeriphDisableDivider(CY_SYSCLK_DIV_8_BIT, 11U);
    Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_8_BIT, 11U, 51U);
    Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_8_BIT, 11U);
    Cy_SysClk_PeriphDisableDivider(CY_SYSCLK_DIV_8_BIT, 1U);
    Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_8_BIT, 1U, 79U);
    Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_8_BIT, 1U);
    Cy_SysClk_PeriphDisableDivider(CY_SYSCLK_DIV_8_BIT, 2U);
    Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_8_BIT, 2U, 47U);
    Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_8_BIT, 2U);
    Cy_SysClk_PeriphDisableDivider(CY_SYSCLK_DIV_8_BIT, 3U);
    Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_8_BIT, 3U, 95U);
    Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_8_BIT, 3U);
    Cy_SysClk_PeriphDisableDivider(CY_SYSCLK_DIV_8_BIT, 4U);
    Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_8_BIT, 4U, 0U);
    Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_8_BIT, 4U);
    Cy_SysClk_PeriphDisableDivider(CY_SYSCLK_DIV_16_BIT, 0U);
    Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_16_BIT, 0U, 25U);
    Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_16_BIT, 0U);
    Cy_SysClk_PeriphDisableDivider(CY_SYSCLK_DIV_8_BIT, 6U);
    Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_8_BIT, 6U, 3U);
    Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_8_BIT, 6U);
    Cy_SysClk_PeriphDisableDivider(CY_SYSCLK_DIV_8_BIT, 7U);
    Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_8_BIT, 7U, 0U);
    Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_8_BIT, 7U);
    Cy_SysClk_PeriphDisableDivider(CY_SYSCLK_DIV_8_BIT, 8U);
    Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_8_BIT, 8U, 47U);
    Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_8_BIT, 8U);
}

void reserve_cycfg_clocks(void)
{
#if defined (CY_USING_HAL)
    cyhal_hwmgr_reserve(&CLK_PDRX_obj);
    cyhal_hwmgr_reserve(&CLK_SYS_TCNT_obj);
    cyhal_hwmgr_reserve(&CLK_DBG_UART_obj);
    cyhal_hwmgr_reserve(&CLK_PDTX_obj);
    cyhal_hwmgr_reserve(&CLK_PDSAR_obj);
    cyhal_hwmgr_reserve(&CLK_FILTER1_obj);
    cyhal_hwmgr_reserve(&CLK_PWM_obj);
    cyhal_hwmgr_reserve(&CLK_UART_obj);
    cyhal_hwmgr_reserve(&CLK_SPI_obj);
    cyhal_hwmgr_reserve(&CLK_I2C_obj);
    cyhal_hwmgr_reserve(&CLK_ADC_obj);
#endif //defined (CY_USING_HAL)
}
