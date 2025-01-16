/***************************************************************************//**
* \file cy_pdutils_rom.h
* \version 1.10
*
* Provides user ROM access.
*
********************************************************************************
* \copyright
* Copyright 2022-2023, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef _CY_PDUTILS_ROM_H_
#define _CY_PDUTILS_ROM_H_


#if defined PMG1S3_SROM_CODE

#define UTILS_ATTRIBUTES __attribute__ ((section (".sromCode"))) __attribute__((used))  __attribute__((noinline))

#elif (PAG2S_SROM_CODE && CCG_SROM_CODE_ENABLE)

/* To generate the SROM code, then include the corresponding device SROM header file. */
#include "srom_vars_pag2s.h"

#endif /* CCGX_ROM_CODE */

#ifndef TIMER_ATTRIBUTES
#define TIMER_ATTRIBUTES
#endif /* TIMER_ATTRIBUTES*/

#ifndef SYS_CALL_MAP
#define SYS_CALL_MAP
#endif /* SYS_CALL_MAP */

#ifndef TIMER_CALL_MAP
#define TIMER_CALL_MAP
#endif /* TIMER_CALL_MAP */

#ifndef UTILS_ATTRIBUTES
#define UTILS_ATTRIBUTES
#endif /* UTILS_ATTRIBUTES */

#ifndef GENERATE_SROM_CODE
#define GENERATE_SROM_CODE                (0u)
#endif /**/

#ifndef PAG2S_SROM_CODE
#define PAG2S_SROM_CODE                   (0u)
#endif /* PAG2S_SROM_CODE */

#endif /* _CY_PDUTILS_ROM_H_ */

/* [] END OF FILE */
