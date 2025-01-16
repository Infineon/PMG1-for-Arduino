/*******************************************************************************
* File Name: mtbcfg_ezpd.h
*
* Description:
* PD stack middleware configuration.
* This file should not be modified. It was automatically generated by
* EZ-PD Configurator 1.20.0.1079
*
********************************************************************************
* \copyright
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
*******************************************************************************/

#if !defined(H_MTBCFG_EZPD)
#define H_MTBCFG_EZPD
    
#include "cycfg_peripherals.h"
#include "cy_pdstack_common.h"

#define MTB_EZPD_CFG_TOOL_VERSION	(120)

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(PD_PORT0_ENABLED)
extern const cy_stc_pdstack_port_cfg_t mtb_usbpd_port0_pdstack_config;
#endif
#if defined(PD_PORT1_ENABLED)
extern const cy_stc_pdstack_port_cfg_t mtb_usbpd_port1_pdstack_config;
#endif


#if defined(__cplusplus)
}
#endif

#endif /* H_MTBCFG_EZPD */

/* [] END OF FILE */
