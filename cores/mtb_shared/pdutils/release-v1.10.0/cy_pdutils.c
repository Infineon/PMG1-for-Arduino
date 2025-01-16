/***************************************************************************//**
* \file cy_pdutils.c
* \version 1.10
*
* Provides general utility macros and definitions for the PDUtils middleware.
*
********************************************************************************
* \copyright
* Copyright 2022-2023, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include <stddef.h>
#include "cy_syslib.h"
#include "cy_pdutils.h"

/* Constant used for CRC-16 calculation. */
#define CY_PDUTILS_CRC16_XOR_VALUE       (0xA001u)

/** Maximum possible number of events index. */
#define CY_PDUTILS_MAX_EVENT_INDEX                 (31U)

UTILS_ATTRIBUTES uint8_t Cy_PdUtils_MemCalculateByteChecksum(uint8_t *ptr, uint32_t size)
{
    uint8_t checksum = 0;
    uint32_t index;

    if ((ptr == NULL) || (size == 0U))
    {
        return checksum;
    }

    /* Calculate the binary sum of all the data. */
    for (index = 0; index < size; index++)
    {
        checksum += ptr[index];
    }

    /* Return the 2's complement of the binary sum. */
    return ((uint8_t)(1u) + (uint8_t)(~checksum));
}

UTILS_ATTRIBUTES uint16_t Cy_PdUtils_MemCalculateWordChecksum(uint16_t *ptr, uint32_t size)
{
    uint16_t checksum = 0;
    uint32_t index;

    if ((ptr == NULL) || (size == 0U))
    {
        return checksum;
    }

    /* Calculate the binary sum of all the data. */
    for (index = 0; index < size; index++)
    {
        checksum += ptr[index];
    }

    /* Return the 2's complement of the binary sum. */
    return ((uint16_t)(1u) + (uint16_t)(~checksum));
}

UTILS_ATTRIBUTES uint32_t Cy_PdUtils_MemCalculateDwordChecksum(uint32_t *ptr, uint32_t size)
{
    uint32_t index;
    uint32_t checksum = 0;

    if ((ptr == NULL) || (size == 0U))
    {
        return checksum;
    }

    /* Calculate the binary sum of all the data. */
    for (index = 0; index < size; index++)
    {
        checksum += ptr[index];
    }

    /* Return the 2's complement of the binary sum. */
    return ((uint32_t)(1u) + (uint32_t)(~checksum));
}

UTILS_ATTRIBUTES uint16_t Cy_PdUtils_Crc16(uint16_t crc, uint8_t data)
{
    int i = 8;

    crc ^= data;
    do
    {
        if ((crc & 1U) != 0U)
        {
            crc = (crc >> 1) ^ CY_PDUTILS_CRC16_XOR_VALUE;
        }
        else
        {
            crc = (crc >> 1);
        }
        i = i - 1;
    } while(i != 0);

    return crc;
}

UTILS_ATTRIBUTES void Cy_PdUtils_MemCopyWord(uint32_t* dest, const uint32_t* source, uint32_t size)
{
    uint32_t i;
    for(i = 0 ; i < size; i++)
    {
        dest[i] = source[i];
    }
}

UTILS_ATTRIBUTES void Cy_PdUtils_MemCopy (uint8_t* dest, const uint8_t* source, uint32_t size)
{
    uint32_t i;
    for(i = 0 ; i < size; i++)
    {
        dest[i] = source[i];
    }
}

UTILS_ATTRIBUTES void Cy_PdUtils_MemSet (uint8_t* dest, uint8_t c, uint32_t size)
{
    uint32_t i;
    for(i = 0 ; i < size; i++)
    {
        dest[i] = c;
    }
}

UTILS_ATTRIBUTES uint32_t Cy_PdUtils_DivRoundUp(uint32_t x, uint32_t y)
{
    volatile uint32_t z = 0;
    volatile uint32_t count = 0;

    /*
     * Using repeated subtraction so that the compiler does not insert any C or math
     * library code to divide.
     */
    if (y != 0U)
    {
        z = (x) + (y) - 1U;

        while (z >= (y))
        {
            z = z - (y);
            count ++;
        }
    }

    return count;
}

UTILS_ATTRIBUTES int32_t Cy_PdUtils_ApplyThreshold(int32_t val, int8_t percentage)
{
    return val + ((val * percentage) / 100);
}

UTILS_ATTRIBUTES void Cy_PdUtils_EventGroup_SetEvent(uint32_t *event_map, uint8_t event_sel)
{
    uint32_t int_status = SYS_CALL_MAP(Cy_SysLib_EnterCriticalSection)();

    if ((event_map != NULL) && (event_sel <= CY_PDUTILS_MAX_EVENT_INDEX))
    {
        *event_map |= (uint32_t)(1UL << event_sel);
    }

    SYS_CALL_MAP(Cy_SysLib_ExitCriticalSection)(int_status);
}

UTILS_ATTRIBUTES void Cy_PdUtils_EventGroup_SetEventsByVal(uint32_t *event_map, uint32_t event_sel)
{
    uint32_t int_status = SYS_CALL_MAP(Cy_SysLib_EnterCriticalSection)();

    if (event_map != NULL)
    {
        *event_map |= event_sel;
    }

    SYS_CALL_MAP(Cy_SysLib_ExitCriticalSection)(int_status);
}

UTILS_ATTRIBUTES void Cy_PdUtils_EventGroup_ClearEvent(uint32_t *event_map, uint8_t event_sel)
{
    uint32_t int_status = SYS_CALL_MAP(Cy_SysLib_EnterCriticalSection)();

    if ((event_map != NULL) && (event_sel <= CY_PDUTILS_MAX_EVENT_INDEX))
    {
        *event_map &= (uint32_t)(~(1UL << event_sel));
    }

    SYS_CALL_MAP(Cy_SysLib_ExitCriticalSection)(int_status);
}

UTILS_ATTRIBUTES void Cy_PdUtils_EventGroup_ClearEventsByVal(uint32_t *event_map, uint32_t event_sel)
{
    uint32_t int_status = SYS_CALL_MAP(Cy_SysLib_EnterCriticalSection)();

    if (event_map != NULL)
    {
        *event_map &= ~event_sel;
    }

    SYS_CALL_MAP(Cy_SysLib_ExitCriticalSection)(int_status);
}

UTILS_ATTRIBUTES uint8_t Cy_PdUtils_EventGroup_GetEvent(volatile uint32_t *event_map, bool clr_stat)
{
    uint32_t evt_stat;
    uint32_t  int_status = SYS_CALL_MAP(Cy_SysLib_EnterCriticalSection)();
    uint8_t  ret = CY_PDUTILS_MAX_EVENT_INDEX + 1U;

    if (event_map != NULL)
    {
        if (*event_map != 0U)
        {
            evt_stat = *event_map;
            for (ret = 0; ret <= CY_PDUTILS_MAX_EVENT_INDEX; ret++)
            {
                if ((evt_stat & (uint32_t)(1UL << ret)) != 0U)
                {
                    if (clr_stat)
                    {
                        *event_map &= (uint32_t)(~(1UL << ret));
                    }

                    break;
                }
            }
        }
    }

    SYS_CALL_MAP(Cy_SysLib_ExitCriticalSection)(int_status);
    return (ret);
}

/* [] END OF FILE */
