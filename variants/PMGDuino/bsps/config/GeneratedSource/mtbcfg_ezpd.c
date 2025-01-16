/*******************************************************************************
* File Name: mtbcfg_ezpd.c
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

#include "mtbcfg_ezpd.h"


/*******************************************************************************
* PD Port Configuration Structure Initialization
*******************************************************************************/
#if defined(PD_PORT0_ENABLED)
static const uint8_t mtb_usbpd_port0_mfg_name[] = {"Infineon"}; /* Manufacturer Name */
static const uint8_t mtb_usbpd_port0_ext_snk_cap[] = {0xB4,0x04,0x03,0xF5,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,0x05,0x05,0x14,0x8C,0x8C,0x8C}; /* SKEDB Configuration */
const cy_stc_pdstack_port_cfg_t mtb_usbpd_port0_pdstack_config = 
{
    .signature = 0x50445343, /* PDSC */
    .version = 0x0100, /* Configuration table version */
    .mfgLenInfo = 0x0D, /* Length of Manufacturer Information */
    .mfgVid = 0x04B4, /* Manufacturer Vendor ID */
    .mfgPid = 0xF503, /* Manufacturer Product ID */
    .mfgName = &mtb_usbpd_port0_mfg_name[0], /* Manufacturer Name */
    .scedbEn = 0, /* SCEDB enable */
    .skedbEn = 1, /* SKEDB enable */
    .extSrcCap = NULL, /* SCEDB Configuration */
    .extSrcCapSize = 0, /* Size of extSrcCap in bytes */
    .extSnkCap = &mtb_usbpd_port0_ext_snk_cap[0], /* SKEDB Configuration */
    .extSnkCapSize = 24, /* Size of extSnkCap in bytes */
    .portRole = 2, /* Port role */
    .defPortRole = 0, /* Default port role */
    .curLevel = 2, /* Current level */
    .cableDiscCount = 0x14, /* Cable Discover count */
    .srcPdoFlags = {0x18,0x02}, /* Source PDO flags */
    .snkPdoFlags = {0x00,0x02}, /* Sink PDO flags */
    .drpToggleEn = 1, /* DRP toggle enable */
    .rpSupported = 7, /* Rp supported */
    .pdOpEn = 1, /* PD operation enable */
    .prefPwrRole = 0, /* Preferred Power role */
    .portDis = 0, /* Port disable */
    .cableDiscEn = 1, /* Cable Discovery Enable */
    .deadBatSupp = 1, /* Dead battery support */
    .errorRecoveryEn = 1, /* Error recovery enable */
    .accessoryEn = 1, /* Accessory mode enable */
    .rpDetachEn = 1, /* Rp detach enable */
    .vconnRetain = 0, /* VCONN retain */
    .frsConfig = 0, /* FRS configuration */
    .srcPdoCount = 0x01, /* Source PDO count */
    .defSrcPdoMask = 0x01, /* Default Source PDO mask */
    .snkPdoCount = 0x01, /* Sink PDO count */
    .defSnkPdoMask = 0x01, /* Default Sink PDO mask */
    .srcPdo = {0x2181912C}, /* Source PDO */
    .snkPdo = {0x2001912C}, /* Sink PDO */
    .snkPdoMinMaxCur = {0x005A}, /* Sink PDO min/max current */
    .eprSrcPdoCount = 0x02, /* EPR Source PDO count */
    .eprSrcPdoMask = 0x03, /* Default EPR Source PDO mask */
    .eprSnkPdoCount = 0x01, /* EPR Sink PDO count */
    .eprSnkPdoMask = 0x01, /* Default EPR Sink PDO mask */
    .pdRevision = 0x30000000, /* Get_Revision message response */
    .srcInfo = 0x00000000, /* Get_Source_Info message response */
    .eprSrcPdo = {0x0008C1F4,0xD2309654}, /* EPR Source PDO */
    .eprSnkPdo = {0x0008C1F4}, /* EPR Sink PDO */
};
#endif
#if defined(PD_PORT1_ENABLED)
static const uint8_t mtb_usbpd_port1_mfg_name[] = {"Infineon"}; /* Manufacturer Name */
static const uint8_t mtb_usbpd_port1_ext_snk_cap[] = {0xB4,0x04,0x00,0xF5,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,0x05,0x05,0x14,0x05,0x05,0x05}; /* SKEDB Configuration */
const cy_stc_pdstack_port_cfg_t mtb_usbpd_port1_pdstack_config = 
{
    .signature = 0x50445343, /* PDSC */
    .version = 0x0100, /* Configuration table version */
    .mfgLenInfo = 0x0D, /* Length of Manufacturer Information */
    .mfgVid = 0x04B4, /* Manufacturer Vendor ID */
    .mfgPid = 0xF500, /* Manufacturer Product ID */
    .mfgName = &mtb_usbpd_port1_mfg_name[0], /* Manufacturer Name */
    .scedbEn = 0, /* SCEDB enable */
    .skedbEn = 1, /* SKEDB enable */
    .extSrcCap = NULL, /* SCEDB Configuration */
    .extSrcCapSize = 0, /* Size of extSrcCap in bytes */
    .extSnkCap = &mtb_usbpd_port1_ext_snk_cap[0], /* SKEDB Configuration */
    .extSnkCapSize = 24, /* Size of extSnkCap in bytes */
    .portRole = 2, /* Port role */
    .defPortRole = 0, /* Default port role */
    .curLevel = 2, /* Current level */
    .cableDiscCount = 0x14, /* Cable Discover count */
    .srcPdoFlags = {0x18,0x02}, /* Source PDO flags */
    .snkPdoFlags = {0x00,0x02}, /* Sink PDO flags */
    .drpToggleEn = 1, /* DRP toggle enable */
    .rpSupported = 7, /* Rp supported */
    .pdOpEn = 1, /* PD operation enable */
    .prefPwrRole = 0, /* Preferred Power role */
    .portDis = 0, /* Port disable */
    .cableDiscEn = 1, /* Cable Discovery Enable */
    .deadBatSupp = 1, /* Dead battery support */
    .errorRecoveryEn = 1, /* Error recovery enable */
    .accessoryEn = 1, /* Accessory mode enable */
    .rpDetachEn = 1, /* Rp detach enable */
    .vconnRetain = 0, /* VCONN retain */
    .frsConfig = 0, /* FRS configuration */
    .srcPdoCount = 0x01, /* Source PDO count */
    .defSrcPdoMask = 0x01, /* Default Source PDO mask */
    .snkPdoCount = 0x01, /* Sink PDO count */
    .defSnkPdoMask = 0x01, /* Default Sink PDO mask */
    .srcPdo = {0x2181912C}, /* Source PDO */
    .snkPdo = {0x2001912C}, /* Sink PDO */
    .snkPdoMinMaxCur = {0x005A}, /* Sink PDO min/max current */
    .eprSrcPdoCount = 0x02, /* EPR Source PDO count */
    .eprSrcPdoMask = 0x03, /* Default EPR Source PDO mask */
    .eprSnkPdoCount = 0x01, /* EPR Sink PDO count */
    .eprSnkPdoMask = 0x01, /* Default EPR Sink PDO mask */
    .pdRevision = 0x30000000, /* Get_Revision message response */
    .srcInfo = 0x00000000, /* Get_Source_Info message response */
    .eprSrcPdo = {0x0008C1F4, 0xD2309654}, /* EPR Source PDO */
    .eprSnkPdo = {0x0008C1F4}, /* EPR Sink PDO */
};
#endif


#if (0)
/*******************************************************************************
* PD Port Configuration Structure Initialization
*******************************************************************************/
#if defined(PD_PORT0_ENABLED)
static const uint8_t mtb_usbpd_port0_mfg_name[] = {"Infineon"}; /* Manufacturer Name */
static const uint8_t mtb_usbpd_port0_ext_snk_cap[] = {0xB4,0x04,0x03,0xF5,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,0x05,0x05,0x14,0x8C,0x8C,0x8C}; /* SKEDB Configuration */
const cy_stc_pdstack_port_cfg_t mtb_usbpd_port0_pdstack_config = 
{
    .signature = 0x50445343, /* PDSC */
    .version = 0x0100, /* Configuration table version */
    .mfgLenInfo = 0x0D, /* Length of Manufacturer Information */
    .mfgVid = 0x04B4, /* Manufacturer Vendor ID */
    .mfgPid = 0xF503, /* Manufacturer Product ID */
    .mfgName = &mtb_usbpd_port0_mfg_name[0], /* Manufacturer Name */
    .scedbEn = 0, /* SCEDB enable */
    .skedbEn = 1, /* SKEDB enable */
    .extSrcCap = NULL, /* SCEDB Configuration */
    .extSrcCapSize = 0, /* Size of extSrcCap in bytes */
    .extSnkCap = &mtb_usbpd_port0_ext_snk_cap[0], /* SKEDB Configuration */
    .extSnkCapSize = 24, /* Size of extSnkCap in bytes */
    .portRole = 0, /* Port role */
    .defPortRole = 0, /* Default port role */
    .curLevel = 2, /* Current level */
    .cableDiscCount = 0x14, /* Cable Discover count */
    .srcPdoFlags = {0}, /* Source PDO flags */
    .snkPdoFlags = {0x00,0x00}, /* Sink PDO flags */
    .drpToggleEn = 1, /* DRP toggle enable */
    .rpSupported = 0, /* Rp supported */
    .pdOpEn = 1, /* PD operation enable */
    .prefPwrRole = 0, /* Preferred Power role */
    .portDis = 0, /* Port disable */
    .cableDiscEn = 0, /* Cable Discovery Enable */
    .deadBatSupp = 1, /* Dead battery support */
    .errorRecoveryEn = 1, /* Error recovery enable */
    .accessoryEn = 1, /* Accessory mode enable */
    .rpDetachEn = 1, /* Rp detach enable */
    .vconnRetain = 0, /* VCONN retain */
    .frsConfig = 0, /* FRS configuration */
    .srcPdoCount = 0x00, /* Source PDO count */
    .defSrcPdoMask = 0x00, /* Default Source PDO mask */
    .snkPdoCount = 0x01, /* Sink PDO count */
    .defSnkPdoMask = 0x1, /* Default Sink PDO mask */
    .srcPdo = {0}, /* Source PDO */
    .snkPdo = {0x0001905A},//0x0002D05A,0x0003C05A,0x0004B05A,0x0006405A,0x9A42305A}, /* Sink PDO */
    .snkPdoMinMaxCur = {0x005A},//,0x005A,0x005A,0x005A,0x005A,0x005A}, /* Sink PDO min/max current */
    .eprSrcPdoCount = 0x00, /* EPR Source PDO count */
    .eprSrcPdoMask = 0x00, /* Default EPR Source PDO mask */
    .eprSnkPdoCount = 0x01, /* EPR Sink PDO count */
    .eprSnkPdoMask = 0x01, /* Default EPR Sink PDO mask */
    .pdRevision = 0x30000000, /* Get_Revision message response */
    .srcInfo = 0x00000000, /* Get_Source_Info message response */
    .eprSrcPdo = {0}, /* EPR Source PDO */
    .eprSnkPdo = {0x0008C1F4}, /* EPR Sink PDO */
};
#endif
#if defined(PD_PORT1_ENABLED)
static const uint8_t mtb_usbpd_port1_mfg_name[] = {"Infineon"}; /* Manufacturer Name */
const cy_stc_pdstack_port_cfg_t mtb_usbpd_port1_pdstack_config = 
{
    .signature = 0x50445343, /* PDSC */
    .version = 0x0100, /* Configuration table version */
    .mfgLenInfo = 0x0D, /* Length of Manufacturer Information */
    .mfgVid = 0x04B4, /* Manufacturer Vendor ID */
    .mfgPid = 0xF500, /* Manufacturer Product ID */
    .mfgName = &mtb_usbpd_port1_mfg_name[0], /* Manufacturer Name */
    .scedbEn = 0, /* SCEDB enable */
    .skedbEn = 1, /* SKEDB enable */
    .extSrcCap = NULL, /* SCEDB Configuration */
    .extSrcCapSize = 0, /* Size of extSrcCap in bytes */
    .extSnkCap = NULL, /* SKEDB Configuration */
    .extSnkCapSize = 0, /* Size of extSnkCap in bytes */
    .portRole = 1, /* Port role */
    .defPortRole = 1, /* Default port role */
    .curLevel = 2, /* Current level */
    .cableDiscCount = 0x14, /* Cable Discover count */
    .srcPdoFlags = {0x70,0x00}, /* Source PDO flags */
    .snkPdoFlags = {0}, /* Sink PDO flags */
    .drpToggleEn = 1, /* DRP toggle enable */
    .rpSupported = 0, /* Rp supported */
    .pdOpEn = 1, /* PD operation enable */
    .prefPwrRole = 0, /* Preferred Power role */
    .portDis = 0, /* Port disable */
    .cableDiscEn = 1, /* Cable Discovery Enable */
    .deadBatSupp = 1, /* Dead battery support */
    .errorRecoveryEn = 1, /* Error recovery enable */
    .accessoryEn = 1, /* Accessory mode enable */
    .rpDetachEn = 1, /* Rp detach enable */
    .vconnRetain = 0, /* VCONN retain */
    .frsConfig = 0, /* FRS configuration */
    .srcPdoCount = 0x04, /* Source PDO count */
    .defSrcPdoMask = 0x01, /* Default Source PDO mask */
    .snkPdoCount = 0x00, /* Sink PDO count */
    .defSnkPdoMask = 0x01, /* Default Sink PDO mask */
    .srcPdo = {0x0701912C},//0x0002D12C,0x0004B12C,0x0006412C}, /* Source PDO */
    .snkPdo = {0x0001905A}, /* Sink PDO */
    .snkPdoMinMaxCur = {0x5A}, /* Sink PDO min/max current */
    .eprSrcPdoCount = 0x00, /* EPR Source PDO count */
    .eprSrcPdoMask = 0x00, /* Default EPR Source PDO mask */
    .eprSnkPdoCount = 0x00, /* EPR Sink PDO count */
    .eprSnkPdoMask = 0x00, /* Default EPR Sink PDO mask */
    .pdRevision = 0x31100000, /* Get_Revision message response */
    .srcInfo = 0x003C3C3C, /* Get_Source_Info message response */
    .eprSrcPdo = {0}, /* EPR Source PDO */
    .eprSnkPdo = {0}, /* EPR Sink PDO */
};
#endif
#endif

/* [] END OF FILE */
