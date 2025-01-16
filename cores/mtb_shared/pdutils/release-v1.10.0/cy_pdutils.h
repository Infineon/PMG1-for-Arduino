/***************************************************************************//**
* \file cy_pdutils.h
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

#if !defined(CY_PDUTILS_H)
#define CY_PDUTILS_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "cy_pdutils_rom.h"

/**
********************************************************************************
* \mainpage PDUtils middleware library
*
* The PDUtils middleware implements the software timer and utility functions
* required by EZ-PD(TM) PMG1/CCGx/WLC devices.
*
* <br><b>Features:</b>
* 1. Provides APIs for software timer management
* 2. Provides APIs for performing utility functions such as Checksum, CRC check,
* and event management.

********************************************************************************
* \section section_pdutils_general_description General description
********************************************************************************
*
* Include cy_pdutils.h and cy_pdutils_sw_timer.h to get access to all functions
* and other declarations in this library. See the \ref
* section_pdutils_quick_start to start using PDUtils.
*
* See the \ref section_pdutils_toolchain section for compatibility
* information.
*
* See the \ref section_pdutils_changelog section for change history.
*
* The PDUtils middleware implements the software timer and utility functions
* that are needed for the EZ-PD(TM) PMG1/CCGx/WLC solutions.
*
* The software timer module provides a WDT/SYS_TICK-based soft timer
* implementation. This allows different modules to create millisecond-level
* timers without additional hardware support. The pdutils middleware defines
* ranges of timer IDs for different usage as shown in the following table.
*
| Section name                    | Section index | Timer ID range  |
| :---                            | :----         | :----:          |
| Device-specific                 | 0             | 0x0000 - 0x00FF |
| PD and Type-C Stack (Port 0)    | 1             | 0x0100 - 0x01FF |
| PD and Type-C Stack (Port 1)    | 2             | 0x0200 - 0x02FF |
| Base application stack (Port 0) | 3             | 0x0300 - 0x03FF |
| Base application stack (Port 1) | 4             | 0x0400 - 0x04FF |
| Alternate mode stack (Port 0)   | 5             | 0x0500 - 0x05FF |
| Alternate mode stack (Port 1)   | 6             | 0x0600 - 0x06FF |
| Wireless charging               | 7             | 0x0700 - 0x07FF |
| Solution layer                  | 8             | 0x0800 - 0x08FF |
| Reserved for future usage       | 9 - 15        | 0x0900 - 0x0FFF |
| User space                      | 16 - 255      | 0x1000 - 0x1FFF |

* The actual timer ID allocation and management within the available range is
* done by the respective assets/modules. The device-specific timer IDs are
* defined and managed by the PDUtils middleware.
********************************************************************************
* \section section_pdutils_quick_start Quick start guide
********************************************************************************
* These steps describe the simplest way of enabling the PDUtils software
* timers in the application.
*
* -# Includes the cy_pdutils.h and cy_pdutils_sw_timer.h to get access to all
* functions and other declarations of this middleware.
*    \snippet snippet/main.c snippet_configuration_include
*
* -# Defines the data structures required by the PDUtils software timer.
*    <br>
*    \snippet snippet/main.c snippet_configuration_data
*
* -# Initializes the PDUtils software timer module after the start. If the
* timer type is watchdog(WDT) timer, then register the interrupt handler for the
* watchdog timer. The watchdog timer is used to implement the software timers.
*    \snippet snippet/main.c snippet_configuration_wdt_interrupt_cfg
*
*    \snippet snippet/main.c snippet_configuration_wdt_interrupt
*
*    Shows a reference wdt_interrupt_handler
*    implementation.
*    \snippet snippet/main.c snippet_configuration_wdt_interrupt_cbk
*
*   Invokes the Cy_PDUtils_SwTimer_Init by passing required parameters.
*   \snippet snippet/main.c snippet_configuration_timer_init
*
* -# Starting timers
*   <br> 
*   Starts a specific software timer by passing the timer ID and desired timer
*   period (ms). All software timers are one-shot timers that will run until
*   the specified period has elapsed. The timer expiration callback will be
*   called at the end of the period if registered.
*
*   Defines the timer ID and timer period.
*   \snippet snippet/main.c snippet_configuration_timer_user_id
*
*   References the callback for an expiry notification handling.
*   \snippet snippet/main.c snippet_configuration_timer_start_cbk
*   
*   Starts a timer by passing the caller context, timer ID, desired period in ms,
*   and a callback for an expiry notification.
*   \snippet snippet/main.c snippet_configuration_timer_start
*
*   Uses when a callback for an expiry notification is not required.
*   \snippet snippet/main.c snippet_configuration_timer_start_wocb
*
* -# Checks the timer running status.
*   \snippet snippet/main.c snippet_configuration_timer_running
*
* -# Stopping timers
*    <br>
*    Stops a timer that is currently running:
*   \snippet snippet/main.c snippet_configuration_timer_stop
*
*   Stops all active software timers:
*   \snippet snippet/main.c snippet_configuration_timer_stop_all
*
*   Stops software timers with timer IDs in the specified range:
*   \snippet snippet/main.c snippet_configuration_timer_stop_range
* .
* <br>
* In addition to the software timer functionality, the PDUtils middleware
* provides APIs for event management and other utility functions. The following
* steps describe the usage of these functions in the application.
* <br>
* -# Event management functions
* <br>
*   An event group facilitates the management of a set of related tasks or status
*   bits. The following APIs can be used to set, get, and clear events for a
*   event group. Event groups are DWORD(uint32_t) variables that hold a set of
*   request bits that can be set/cleared independently.
*
*   Event group variable.
*   \snippet snippet/main.c snippet_configuration_utils_evt_data
*
*   Setting a bit in an event group variable.
*   \snippet snippet/main.c snippet_configuration_utils_evt_set
*
*   Setting one or more values in an event group variable.
*   \snippet snippet/main.c snippet_configuration_utils_evt_setval
*   
*   Get the position of the event detected that needs to be processed.
*   \snippet snippet/main.c snippet_configuration_utils_evt_get
*
*   Clears a bit in an event group variable.
*   \snippet snippet/main.c snippet_configuration_utils_evt_clr
*
*   Clears one or more values in an event group variable.
*   \snippet snippet/main.c snippet_configuration_utils_evt_clrval
* -# Other Utility functions
* <br>
*   Computes the checksum (2's complement of the binary sum
*   of all bytes) for the specified byte array.
*   \snippet snippet/main.c snippet_configuration_utils_byte_chksum
* <br>
*   Computes the checksum (2's complement of the binary sum
*   of all words) for the specified WORD array.
*   \snippet snippet/main.c snippet_configuration_utils_word_chksum
* <br>
*   Computes the checksum (2's complement of the binary sum
*   of all words) for the specified DWORD array.
*   \snippet snippet/main.c snippet_configuration_utils_dword_chksum
* <br>
*   Implements the CRC-16 with polynomial x^16 + x^15 +
*   x^2 + 1 (0xA001).
*   \snippet snippet/main.c snippet_configuration_utils_crc
* <br>
*   Returns the quotient rounded up to the nearest integer
*   for the given two unsigned integers.
*   \snippet snippet/main.c snippet_configuration_utils_div
*
********************************************************************************
* \section section_pdutils_configuration_considerations Configuration Considerations
********************************************************************************
*
* This section consists of instructions on how to configure and use
* the PDUtils middleware in a design.
* 1. After including the PDUtils files, select the timer type by setting:
*    * CY_PDUTILS_TIMER_TYPE to CY_PDUTILS_TIMER_TYPE_SYSTICK for using SYSTICK
*    timer based software timers.
*    * CY_PDUTILS_TIMER_TYPE to CY_PDUTILS_TIMER_TYPE_WDT for using Watchdog
*    timer based software timers.
*    * CY_PDUTILS_TIMER_TYPE to CY_PDUTILS_TIMER_TYPE_PD_ILO for using LF timer
*    based software timers.
* 2. Set the macro CY_PDUTILS_TIMER_CALIB_SAMPLE_TIME_1MS to '1' to enable timer
* calibration sample time of 1 ms. By default, this macro is set to '0' for a
* sample time of 10 ms.
*
* <br>
* See the \ref section_pdutils_miscellaneous section for the existing
* restrictions.
*
********************************************************************************
* \section section_pdutils_miscellaneous Limitations and restrictions
********************************************************************************
* 1. The timer source used by the software timer should not be used elsewhere
* in the application.
*
********************************************************************************
* \section section_pdutils_toolchain Supported software and tools
********************************************************************************
*
* This version of the PDUtils middleware is validated for compatibility
* with the following software and tools:
*
* <table class="doxtable">
*   <tr>
*     <th>Software and tools</th>
*     <th>Version</th>
*   </tr>
*   <tr>
*     <td>ModusToolbox(TM) software environment</td>
*     <td>3.0</td>
*   </tr>
*   <tr>
*     <td>mtb-pdl-cat2</td>
*     <td>2.4.0</td>
*   </tr>
*   <tr>
*     <td>GCC compiler</td>
*     <td>10.3.1</td>
*   </tr>
*   <tr>
*     <td>IAR compiler</td>
*     <td>8.42.2</td>
*   </tr>
*   <tr>
*     <td>Arm(R) compiler</td>
*     <td>6.13</td>
*   </tr>
* </table>
*
********************************************************************************
* \section section_pdutils_changelog Changelog
********************************************************************************
*
* <table class="doxtable">
*   <tr><th>Version</th><th>Changes</th><th>Reason for change</th></tr>
*   <tr>
*     <td rowspan="2">1.10</td>
*     <td>Updates to support LF timer functionality</td>
*     <td>New feature</td>
*   </tr>
*   <tr>
*     <td>Fixes to SYSTICK timer functionality</td>
*     <td>Bug fix</td>
*   </tr>
*   <tr>
*     <td>1.0</td>
*     <td>Initial version</td>
*     <td></td>
*   </tr>
* </table>
*
********************************************************************************
* \section section_pdutils_more_information More information
********************************************************************************
*
* For more information, see the following documents:
*
* * <a href="https://www.infineon.com/modustoolbox">
*      <b>ModusToolbox(TM) software, quick start guide, documentation,
*         and videos</b>
*   </a>
*
* \note
* Links to other software components' documentation (middleware and PDL)
* point to GitHub for the latest available version of the software.
* To get the documentation of the specified version, download from GitHub and unzip
* the component archive. The documentation is available in
* the <i>docs</i> folder.
*
********************************************************************************
*
* \defgroup group_pdutils_macros Macros
* \brief
* Describes the PDUtils macros.
*
* \defgroup group_pdutils_functions Functions
* \brief
* Describes the PDUtils function prototypes.
*
* \defgroup group_pdutils_data_structures Data structures
* \brief
* Describes the data structures defined by PDUtils.
*
* \defgroup group_pdutils_enums Enumerated Types
* \brief
* Describes the enumeration types defined by PDUtils.
*
*/

/*****************************************************************************
* Macro definition
*****************************************************************************/

/**
* \addtogroup group_pdutils_macros
* \{
*/

/** The PDUtils middleware major version */
#define CY_PDUTILS_MW_VERSION_MAJOR               (1)

/** The PDUtils middleware minor version */
#define CY_PDUTILS_MW_VERSION_MINOR               (10)

/**  Get the maximum from among two numbers. */
#define CY_PDUTILS_GET_MAX(a,b)    (((a) > (b)) ? (a) : (b))

/**  Get the minimum from among two numbers. */
#define CY_PDUTILS_GET_MIN(a,b)    (((a) > (b)) ? (b) : (a))

/** Integer division - round up to the next integer. */
#define CY_PDUTILS_DIV_ROUND_UP(x, y) (((x) + ((y) - 1)) / (y))

/** Integer division - round to the nearest integer. */
#define CY_PDUTILS_DIV_ROUND_NEAREST(x, y) (((x) + ((y) / 2)) / (y))

/** Combine two bytes to create one 16-bit WORD. */
#define CY_PDUTILS_MAKE_WORD(hi,lo)                        (((uint16_t)(hi) << 8) | ((uint16_t)(lo)))

/**
 * Combine four bytes to create one 32-bit DWORD.
 */
#define CY_PDUTILS_MAKE_DWORD(b3,b2,b1,b0)                         \
    (((uint32_t)(b3) << 24) | ((uint32_t)(b2) << 16) |  \
     ((uint32_t)(b1) << 8) | ((uint32_t)(b0)))

/** Combine two WORDs to create one DWORD. */
#define CY_PDUTILS_MAKE_DWORD_FROM_WORD(hi,lo)             (((uint32_t)(hi) << 16) | ((uint32_t)(lo)))

/** Retrieve the MSB from a WORD. */
#define CY_PDUTILS_WORD_GET_MSB(w)         ((uint8_t)((w) >> 8))

/** Retrieve the LSB from a WORD. */
#define CY_PDUTILS_WORD_GET_LSB(w)         ((uint8_t)((w) & 0xFF))

/** Retrieve the upper nibble from a byte. */
#define CY_PDUTILS_BYTE_GET_UPPER_NIBBLE(w)         ((uint8_t)(((w) >> 4) & 0xF))

/** Retrieve the lower nibble from a byte. */
#define CY_PDUTILS_BYTE_GET_LOWER_NIBBLE(w)         ((uint8_t)((w) & 0xF))

/** Retrieve the LSB from a DWORD. */
#define CY_PDUTILS_DWORD_GET_BYTE0(dw)     ((uint8_t)((dw) & 0xFF))

/** Retrieve the bits 15-8 from a DWORD. */
#define CY_PDUTILS_DWORD_GET_BYTE1(dw)     ((uint8_t)(((dw) >> 8) & 0xFF))

/** Retrieve the bits 23-16 from a DWORD. */
#define CY_PDUTILS_DWORD_GET_BYTE2(dw)     ((uint8_t)(((dw) >> 16) & 0xFF))

/** Retrieve the MSB from a DWORD. */
#define CY_PDUTILS_DWORD_GET_BYTE3(dw)     ((uint8_t)(((dw) >> 24) & 0xFF))

/** Memcpy abstraction macro. */
#define CY_PDUTILS_MEM_COPY(dest, src, size)               memcpy ((uint8_t *)(dest), (uint8_t *)(src), (size))

/** Memcmp abstraction macro. */
#define CY_PDUTILS_MEM_CMP(buf1, buf2, size)               memcmp ((uint8_t *)(buf1), (uint8_t *)(buf2), (size))

/** Memset abstraction macro. */
#define CY_PDUTILS_MEM_SET(dest, byte, size)               memset ((uint8_t *)(dest), (byte), (size))

/** Insert delay of the desired number of us using a busy spin-loop. */
#define CY_PDUTILS_BUSY_WAIT_US(us)        CyDelayCycles ((uint32_t)(us))

/**
 * Combine four bytes in reverse order to make a 32-bit integer.
 * \param n
 * Final 32-bit number.
 *
 * \param b
 * Input array of bytes to be combined.
 *
 * \param i
 * Index into the input byte array.
 */
#define CY_PDUTILS_REV_BYTE_ORDER(n,b,i)                           \
    (n) = ( (uint32_t) (b)[(i)    ] << 24 )             \
        | ( (uint32_t) (b)[(i) + 1] << 16 )             \
        | ( (uint32_t) (b)[(i) + 2] <<  8 )             \
        | ( (uint32_t) (b)[(i) + 3]       );            \


/** \} group_pdutils_macros */

/*****************************************************************************
* Global function declaration
*****************************************************************************/

/**
* \addtogroup group_pdutils_functions
* \{
*/

/*******************************************************************************
* Function name: Cy_PdUtils_MemCalculateByteChecksum
****************************************************************************//**
*
* Calculates the checksum of the specified byte array. The
* checksum is a simple function calculated as the 2's complement of the binary
* sum of all bytes in the array.
*
* \param ptr
* Pointer to the data array.
*
* \param size
* Size of the array in byte elements.
*
* \return uint8_t
* Checksum of the data array.
*******************************************************************************/
uint8_t Cy_PdUtils_MemCalculateByteChecksum(uint8_t *ptr, uint32_t size);

/*******************************************************************************
* Function name: Cy_PdUtils_MemCalculateWordChecksum
****************************************************************************//**
*
* Calculates the checksum of the specified WORD array. The
* checksum is a simple function calculated as the 2's complement of the binary
* sum of all words in the array.
*
* \param ptr
* Pointer to the data array.
*
* \param size
* Size of the array in WORD elements.
*
* \return uint16_t
* Checksum of the data array.
*******************************************************************************/
uint16_t Cy_PdUtils_MemCalculateWordChecksum(uint16_t *ptr, uint32_t size);

/*******************************************************************************
* Function name: Cy_PdUtils_MemCalculateDwordChecksum
****************************************************************************//**
*
* Calculates the checksum of the specified DWORD array. The
* checksum is a simple function calculated as the 2's complement of the binary
* sum of all DWORDs in the array.
*
* \param ptr
* Pointer to the data array.
*
* \param size
* Size of the array in DWORD elements.
*
* \return uint32_t
* Checksum of the data array.
*******************************************************************************/
uint32_t Cy_PdUtils_MemCalculateDwordChecksum(uint32_t *ptr, uint32_t size);

/*******************************************************************************
* Function name: Cy_PdUtils_Crc16
****************************************************************************//**
*
* Implements CRC-16 with polynomial x^16 + x^15 + x^2 + 1 (0xA001).
*
* \param crc
* Original CRC value.
*
* \param data
* Data byte to be included in CRC computation.
*
* \return uint16_t
* Updated CRC value including the new data byte.
*******************************************************************************/
uint16_t Cy_PdUtils_Crc16(uint16_t crc, uint8_t data);

/*******************************************************************************
* Function name: Cy_PdUtils_MemCopyWord
****************************************************************************//**
*
* Copies 32-bit data from one location to another.
*
* \param dest
* Pointer to destination.
*
* \param source
* Pointer to the source.
*
* \param size
* Size of data in 32-bit DWORD units.
*
* \return
* None.
*******************************************************************************/
void Cy_PdUtils_MemCopyWord(uint32_t* dest, const uint32_t* source, uint32_t size);

/*******************************************************************************
* Function name: Cy_PdUtils_MemCopy
****************************************************************************//**
*
* Copies the data byte-by-byte. Replacement for memcpy function.
* It is used in cases where a memcpy equivalent that can be
* placed in a specific memory region (such as the ROM) is required.
*
* \param dest
* Pointer to the destination buffer.
*
* \param source
* Pointer to source buffer.
*
* \param size
* Size of data to be copied (in bytes).
*
* \return
* None
*******************************************************************************/
void Cy_PdUtils_MemCopy (uint8_t* dest, const uint8_t* source, uint32_t size);

/*******************************************************************************
* Function name: Cy_PdUtils_MemSet
****************************************************************************//**
*
* Initializes a memory buffer. Replacement for the memset function.
* It is used in cases where a memset equivalent that can be placed
* in a specific memory region (such as the ROM) is required.
*
* \param dest
* Pointer to the destination buffer.
*
* \param c
* Data to be copied into each byte of the buffer.
*
* \param size
* Size of the buffer (in bytes).
*
* \return
* None
*******************************************************************************/
void Cy_PdUtils_MemSet (uint8_t* dest, uint8_t c, uint32_t size);

/*******************************************************************************
* Function name: Cy_PdUtils_DivRoundUp
****************************************************************************//**
*
* Divides two unsigned integers and returns the quotient rounded up
* to the nearest integer.
*
* \param x
* Dividend value.
*
* \param y
* Divisor value. Must be non-zero.
*
*
* \return uint32_t
* Quotient rounded up to the nearest integer.
*******************************************************************************/
uint32_t Cy_PdUtils_DivRoundUp(uint32_t x, uint32_t y);

/*******************************************************************************
* Function name: Cy_PdUtils_ApplyThreshold
****************************************************************************//**
*
* Calculates the threshold value based on the input value and the percentage by which
* the input value should be increased.
*
* \param val
* Input value.
*
* \param percentage
* Value means how much (in percent) input value should be increased.
*
* \return int32_t
* Sum of the input value and calculated threshold.
*******************************************************************************/
int32_t Cy_PdUtils_ApplyThreshold(int32_t val, int8_t percentage);

/*******************************************************************************
* Function name: Cy_PdUtils_EventGroup_SetEvent
****************************************************************************//**
*
* Sets a bit in an event group variable. Event groups are DWORD
* (uint32_t) variables and hold a set of request bits that can be
* set/cleared independently. An event group facilitates the management of a set of
* related tasks or status bits.
*
* \param event_map
* Holds the current status of the event group.
*
* \param event_sel
* Specifies the position of the event/task to be set.
*
* \return
* None.
*******************************************************************************/
void Cy_PdUtils_EventGroup_SetEvent(uint32_t *event_map, uint8_t event_sel);

/*******************************************************************************
* Function name: Cy_PdUtils_EventGroup_SetEventsByVal
****************************************************************************//**
*
* Set one or more task values in an event group. Event groups are
* DWORD (uint32_t) variables hold a set of request bits that can be
* set/cleared independently. An event group facilitates the management of a set of
* related tasks or status bits.
*
* \param event_map
* Holds the current status of the event group.
*
* \param event_sel
* Specifies a set of event/task bits that should be set.
*
* \return
* None.
*******************************************************************************/
void Cy_PdUtils_EventGroup_SetEventsByVal(uint32_t *event_map, uint32_t event_sel);

/*******************************************************************************
* Function name: Cy_PdUtils_EventGroup_ClearEvent
****************************************************************************//**
*
* Clears a bit in an event group variable.
*
* \param event_map
* Holds the current status of the event group.
*
* \param event_sel
* Specifies the position of the event/task to be cleared.
*
* \return
* None.
*******************************************************************************/
void Cy_PdUtils_EventGroup_ClearEvent(uint32_t *event_map, uint8_t event_sel);

/*******************************************************************************
* Function name: Cy_PdUtils_EventGroup_ClearEventsByVal
****************************************************************************//**
*
* Clears a bit in an event group variable.
*
* \param event_map
* Holds the current status of the event group.
*
* \param event_sel
* Specifies one or more event/task bits to be cleared.
*
* \return
* None.
*******************************************************************************/
void Cy_PdUtils_EventGroup_ClearEventsByVal(uint32_t *event_map, uint32_t event_sel);

/*******************************************************************************
* Function name: Cy_PdUtils_EventGroup_GetEvent
****************************************************************************//**
*
* Gets the next pending event/task from an event group variable.
*
* \param event_map
* Original status of the event group.
*
* \param clr_stat
* Specifies whether the event returned should be cleared.
*
* \return uint8_t
* Position of the event/task which has been detected and needs to be processed.
*
*******************************************************************************/
uint8_t Cy_PdUtils_EventGroup_GetEvent(volatile uint32_t *event_map, bool clr_stat);

/** \} group_pdutils_functions */

#endif /* CY_PDUTILS_H */

/* [] END OF FILE */
