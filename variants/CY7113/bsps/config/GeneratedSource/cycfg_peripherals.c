/*******************************************************************************
* File Name: cycfg_peripherals.c
*
* Description:
* Peripheral Hardware Block configuration
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

#include "cycfg_peripherals.h"

#define CYBSP_SYS_TCNT_INPUT_DISABLED 0x7U

const cy_stc_sar_channel_config_t pass_0_sar_0_channel_0_config = 
{
    .addr = (cy_en_sar_chan_config_port_pin_addr_t)(SAR0_VPLUS0_PIN | (SAR0_VPLUS0_PORT << SAR_CHAN_CONFIG_PORT_ADDR_Pos)),
    .differential = false,
    .resolution = CY_SAR_MAX_RES,
    .avgEn = true,
    .sampleTimeSel = CY_SAR_SAMPLE_TIME_0,
    .rangeIntrEn = false,
    .satIntrEn = false,
};
const cy_stc_sar_channel_config_t pass_0_sar_0_channel_1_config = 
{
    .addr = (cy_en_sar_chan_config_port_pin_addr_t)(SAR0_VPLUS1_PIN | (SAR0_VPLUS1_PORT << SAR_CHAN_CONFIG_PORT_ADDR_Pos)),
    .differential = false,
    .resolution = CY_SAR_MAX_RES,
    .avgEn = true,
    .sampleTimeSel = CY_SAR_SAMPLE_TIME_1,
    .rangeIntrEn = false,
    .satIntrEn = false,
};
const cy_stc_sar_channel_config_t pass_0_sar_0_inj_channel_config = 
{
    .addr = (cy_en_sar_chan_config_port_pin_addr_t)(SAR0_VPLUS16_PIN | (SAR0_VPLUS16_PORT << SAR_CHAN_CONFIG_PORT_ADDR_Pos)),
    .differential = false,
    .resolution = CY_SAR_MAX_RES,
    .avgEn = true,
    .sampleTimeSel = CY_SAR_SAMPLE_TIME_0,
    .rangeIntrEn = false,
    .satIntrEn = false,
};

const cy_stc_sar_config_t pass_0_sar_0_config = 
{
    .vrefSel = CY_SAR_VREF_SEL_BGR,
    .vrefBypCapEn = true,
    .negSel = CY_SAR_NEG_SEL_VREF,
    .negVref = CY_SAR_NEGVREF_HW,
    .boostPump = false,
    .power = CY_SAR_QUARTER_PWR,
    .sarMuxDsEn = false,
    .switchDisable = false,
    .subResolution = CY_SAR_SUB_RESOLUTION_10B,
    .leftAlign = false,
    .singleEndedSigned = false,
    .differentialSigned = false,
    .avgCnt = CY_SAR_AVG_CNT_8,
    .avgShift = false,
    .trigMode = CY_SAR_TRIGGER_MODE_FW_ONLY,
    .eosEn = false,
    .sampleTime0 = 2,
    .sampleTime1 = 2,
    .sampleTime2 = 2,
    .sampleTime3 = 2,
    .rangeThresLow = 0UL,
    .rangeThresHigh = 0UL,
    .rangeCond = CY_SAR_RANGE_COND_BELOW,
    .chanEn = 65536UL,
    .channelConfig = {&pass_0_sar_0_channel_0_config, &pass_0_sar_0_channel_1_config, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &pass_0_sar_0_inj_channel_config},
    .routingConfig = NULL,
    .vrefMvValue = pass_0_sar_0_VREF_MV,
};
#if defined (CY_USING_HAL)
    const cyhal_resource_inst_t pass_0_sar_0_obj = 
    {
        .type = CYHAL_RSC_ADC,
        .block_num = 0,
        .channel_num = 0,
    };
#endif //defined (CY_USING_HAL)

const cy_stc_tcpwm_counter_config_t CYBSP_SYS_TCNT_config = 
{
    .period = 1000,
    .clockPrescaler = CY_TCPWM_COUNTER_PRESCALER_DIVBY_1,
    .runMode = CY_TCPWM_COUNTER_CONTINUOUS,
    .countDirection = CY_TCPWM_COUNTER_COUNT_UP,
    .compareOrCapture = CY_TCPWM_COUNTER_MODE_CAPTURE,
    .compare0 = 1000,
    .compare1 = 16384,
    .enableCompareSwap = false,
    .interruptSources = CY_TCPWM_INT_ON_TC,
    .captureInputMode = CYBSP_SYS_TCNT_INPUT_DISABLED & 0x3U,
    .captureInput = CY_TCPWM_INPUT_0,
    .reloadInputMode = CYBSP_SYS_TCNT_INPUT_DISABLED & 0x3U,
    .reloadInput = CY_TCPWM_INPUT_0,
    .startInputMode = CYBSP_SYS_TCNT_INPUT_DISABLED & 0x3U,
    .startInput = CY_TCPWM_INPUT_0,
    .stopInputMode = CYBSP_SYS_TCNT_INPUT_DISABLED & 0x3U,
    .stopInput = CY_TCPWM_INPUT_0,
    .countInputMode = CYBSP_SYS_TCNT_INPUT_DISABLED & 0x3U,
    .countInput = CY_TCPWM_INPUT_1,
};
#if defined (CY_USING_HAL)
    const cyhal_resource_inst_t CYBSP_SYS_TCNT_obj = 
    {
        .type = CYHAL_RSC_TCPWM,
        .block_num = 0U,
        .channel_num = 6U,
    };
#endif //defined (CY_USING_HAL)

const cy_stc_fault_vbus_ovp_cfg_t mtb_usbpd_port0_ovp_config = 
{
    .enable = true,
    .mode = 2,
    .threshold = 20,
    .debounce = 10,
    .retryCount = 2,
};
const cy_stc_fault_vbus_ocp_cfg_t mtb_usbpd_port0_ocp_config = 
{
    .enable = true,
    .mode = 1,
    .threshold = 20,
    .debounce = 10,
    .retryCount = 2,
    .senseRes = 5,
};
const cy_stc_fault_vbus_scp_cfg_t mtb_usbpd_port0_scp_config = 
{
    .enable = true,
    .mode = 2,
    .senseRes = 5,
    .threshold = 6,
    .debounce = 10,
    .retryCount = 2,
};
const cy_stc_fault_vbus_rcp_cfg_t mtb_usbpd_port0_rcp_config = 
{
    .enable = true,
    .retryCount = 2,
};
const cy_stc_fault_vconn_ocp_cfg_t mtb_usbpd_port0_vconn_ocp_config = 
{
    .enable = true,
    .debounce = 1,
    .retryCount = 2,
};
const cy_stc_usbpd_config_t mtb_usbpd_port0_config = 
{
    .vbusOvpConfig = &mtb_usbpd_port0_ovp_config,
    .vbusUvpConfig = NULL,
    .vbusOcpConfig = &mtb_usbpd_port0_ocp_config,
    .vbusScpConfig = &mtb_usbpd_port0_scp_config,
    .vbusRcpConfig = &mtb_usbpd_port0_rcp_config,
    .vconnOcpConfig = &mtb_usbpd_port0_vconn_ocp_config,
    .ccOvpConfig = NULL,
    .sbuOvpConfig = NULL,
    .legacyChargingConfig = NULL,
    .buckBoostConfig = NULL,
};

void init_cycfg_peripherals(void)
{
    Cy_CTB_SetPumpClkSource(CTBM0, CY_CTB_CLK_PUMP_HF_DIV_2);
    Cy_SysClk_PeriphAssignDivider(PCLK_PASS0_CLOCK_SAR, CY_SYSCLK_DIV_8_BIT, 8U);
    Cy_SysClk_PeriphAssignDivider(PCLK_SCB0_CLOCK, CY_SYSCLK_DIV_8_BIT, 11U);
    Cy_SysClk_PeriphAssignDivider(PCLK_SCB4_CLOCK, CY_SYSCLK_DIV_16_BIT, 0U);
    Cy_SysClk_PeriphAssignDivider(PCLK_TCPWM_CLOCKS0, CY_SYSCLK_DIV_8_BIT, 4U);
    Cy_SysClk_PeriphAssignDivider(PCLK_TCPWM_CLOCKS1, CY_SYSCLK_DIV_8_BIT, 4U);
    Cy_SysClk_PeriphAssignDivider(PCLK_TCPWM_CLOCKS2, CY_SYSCLK_DIV_8_BIT, 4U);
    Cy_SysClk_PeriphAssignDivider(PCLK_TCPWM_CLOCKS3, CY_SYSCLK_DIV_8_BIT, 4U);
    Cy_SysClk_PeriphAssignDivider(PCLK_TCPWM_CLOCKS4, CY_SYSCLK_DIV_8_BIT, 4U);
    Cy_SysClk_PeriphAssignDivider(PCLK_TCPWM_CLOCKS5, CY_SYSCLK_DIV_8_BIT, 4U);
    Cy_SysClk_PeriphAssignDivider(PCLK_TCPWM_CLOCKS6, CY_SYSCLK_DIV_8_BIT, 10U);
    Cy_SysClk_PeriphAssignDivider(PCLK_TCPWM_CLOCKS7, CY_SYSCLK_DIV_8_BIT, 4U);
    Cy_SysClk_PeriphAssignDivider(PCLK_USBPD0_CLOCK_RX, CY_SYSCLK_DIV_8_BIT, 0U);
    Cy_SysClk_PeriphAssignDivider(PCLK_USBPD0_CLOCK_TX, CY_SYSCLK_DIV_8_BIT, 1U);
    Cy_SysClk_PeriphAssignDivider(PCLK_USBPD0_CLOCK_SAR, CY_SYSCLK_DIV_8_BIT, 2U);
    Cy_SysClk_PeriphAssignDivider(PCLK_USBPD0_CLOCK_REFGEN, CY_SYSCLK_DIV_8_BIT, 0U);
    Cy_SysClk_PeriphAssignDivider(PCLK_USBPD0_CLOCK_FILTER1, CY_SYSCLK_DIV_8_BIT, 3U);
    Cy_SysClk_PeriphAssignDivider(PCLK_USBPD0_CLOCK_FILTER2, CY_SYSCLK_DIV_8_BIT, 3U);
    Cy_SysClk_PeriphAssignDivider(PCLK_USBPD1_CLOCK_RX, CY_SYSCLK_DIV_8_BIT, 0U);
    Cy_SysClk_PeriphAssignDivider(PCLK_USBPD1_CLOCK_TX, CY_SYSCLK_DIV_8_BIT, 1U);
    Cy_SysClk_PeriphAssignDivider(PCLK_USBPD1_CLOCK_SAR, CY_SYSCLK_DIV_8_BIT, 2U);
    Cy_SysClk_PeriphAssignDivider(PCLK_USBPD1_CLOCK_FILTER1, CY_SYSCLK_DIV_8_BIT, 3U);
    Cy_SysClk_PeriphAssignDivider(PCLK_USBPD1_CLOCK_FILTER2, CY_SYSCLK_DIV_8_BIT, 3U);
}

void reserve_cycfg_peripherals(void)
{
#if defined (CY_USING_HAL)
    cyhal_hwmgr_reserve(&pass_0_sar_0_obj);
    cyhal_hwmgr_reserve(&CYBSP_DBG_UART_obj);
    cyhal_hwmgr_reserve(&pwm0_obj);
    cyhal_hwmgr_reserve(&pwm1_obj);
    cyhal_hwmgr_reserve(&pwm2_obj);
    cyhal_hwmgr_reserve(&pwm3_obj);
    cyhal_hwmgr_reserve(&pwm4_obj);
    cyhal_hwmgr_reserve(&pwm5_obj);
    cyhal_hwmgr_reserve(&CYBSP_SYS_TCNT_obj);
    cyhal_hwmgr_reserve(&pwm6_obj);
#endif //defined (CY_USING_HAL)
}
