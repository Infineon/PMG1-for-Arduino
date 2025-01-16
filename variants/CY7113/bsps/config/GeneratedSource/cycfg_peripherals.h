/*******************************************************************************
* File Name: cycfg_peripherals.h
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

#if !defined(CYCFG_PERIPHERALS_H)
#define CYCFG_PERIPHERALS_H

#include "cycfg_notices.h"
#include "cy_ctb.h"
#include "cy_sar.h"
#include "cycfg_routing.h"
#include "cy_sysclk.h"
#if defined (CY_USING_HAL)
    #include "cyhal_hwmgr.h"
#endif //defined (CY_USING_HAL)
#include "cy_scb_uart.h"
#include "cy_tcpwm_pwm.h"
#include "cy_tcpwm_counter.h"
#include "cy_usbpd_common.h"

#if defined(__cplusplus)
extern "C" {
#endif

#define pass_0_ENABLED 1U
#define pass_0_sar_0_ENABLED 1U
#define pass_0_sar_0_HW SAR0
#define pass_0_sar_0_IRQ pass_0_interrupt_sar_IRQn
#define pass_0_sar_0_VREF_MV 1650UL
#define pass_0_sarmux_0_tempsensor_0_ENABLED 1U
#define CYBSP_SYS_TCNT_HW TCPWM
#define CYBSP_SYS_TCNT_NUM 6UL
#define CYBSP_SYS_TCNT_MASK (1UL << 6)
#define CYBSP_SYS_TCNT_IRQ tcpwm_interrupts_6_IRQn
#define CYBSP_SYS_TCNT_INPUT_DISABLED 0x7U
#define PD_PORT0_ENABLED 1U
#define mtb_usbpd_port0_HW PDSS0
#define mtb_usbpd_port0_HW_TRIM PDSS_TRIMS0
#define mtb_usbpd_port0_IRQ usbpd_0_interrupt_IRQn
#define mtb_usbpd_port0_DS_IRQ usbpd_0_interrupt_wakeup_IRQn

extern const cy_stc_sar_channel_config_t pass_0_sar_0_channel_0_config;
extern const cy_stc_sar_channel_config_t pass_0_sar_0_channel_1_config;
extern const cy_stc_sar_channel_config_t pass_0_sar_0_channel_2_config;
extern const cy_stc_sar_channel_config_t pass_0_sar_0_inj_channel_config;
extern const cy_stc_sar_config_t pass_0_sar_0_config;
#if defined (CY_USING_HAL)
    extern const cyhal_resource_inst_t pass_0_sar_0_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_tcpwm_counter_config_t CYBSP_SYS_TCNT_config;
#if defined (CY_USING_HAL)
    extern const cyhal_resource_inst_t CYBSP_SYS_TCNT_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_fault_vbus_ovp_cfg_t mtb_usbpd_port0_ovp_config;
extern const cy_stc_fault_vbus_ocp_cfg_t mtb_usbpd_port0_ocp_config;
extern const cy_stc_fault_vbus_scp_cfg_t mtb_usbpd_port0_scp_config;
extern const cy_stc_fault_vbus_rcp_cfg_t mtb_usbpd_port0_rcp_config;
extern const cy_stc_fault_vconn_ocp_cfg_t mtb_usbpd_port0_vconn_ocp_config;
extern const cy_stc_usbpd_config_t mtb_usbpd_port0_config;

void init_cycfg_peripherals(void);
void reserve_cycfg_peripherals(void);

#if defined(__cplusplus)
}
#endif


#endif /* CYCFG_PERIPHERALS_H */
