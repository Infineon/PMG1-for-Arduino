/***************************************************************************//**
* \file cy_pdutils_sw_timer.h
* \version 1.10
*
* Provides API declarations of the software timer implementation.
*
********************************************************************************
* \copyright
* Copyright 2022-2023, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef _CY_PDUTILS_SW_TIMER_H_
#define _CY_PDUTILS_SW_TIMER_H_

#include <stdint.h>
#include <stdbool.h>
#include "cy_pdl.h"
#include "cy_device_headers.h"
#include "cy_pdutils_rom.h"

/*****************************************************************************
 * Macro definition
 *****************************************************************************/

/**
* \addtogroup group_pdutils_macros
* \{
*/

/** Systick timer. */
#define CY_PDUTILS_TIMER_TYPE_SYSTICK              (1u)

/** WDT timer. */
#define CY_PDUTILS_TIMER_TYPE_WDT                  (2u)

/** LF timer. */
#define CY_PDUTILS_TIMER_TYPE_PD_ILO               (3u)

/** Timer type. */
#ifndef CY_PDUTILS_TIMER_TYPE
#define CY_PDUTILS_TIMER_TYPE                      CY_PDUTILS_TIMER_TYPE_WDT
#endif /* CY_PDUTILS_TIMER_TYPE */

/** Number of timer instances. */
#define CY_PDUTILS_TIMER_NUM_INSTANCES             (1u)

/** SYS_CLK frequency should match the actual hardware SYS_CLK frequency. */
#define CY_PDUTILS_SYSCLK_FREQUENCY                (SystemCoreClock)

/** SYS_CLK counter threshold for 1 ms timeout. Division is expected
 * to be optimized by the compiler. */
#define CY_PDUTILS_SYSCLK_1MS_COUNT_VALUE          (CY_PDUTILS_SYSCLK_FREQUENCY / 1000u)

/** Mask for the WDT counter value. */
#define CY_PDUTILS_WDT_COUNTER_COUNTER_MASK        (0x0000ffff)

/** Enable the calibration sample time of 1 ms. If this macro is set to '1', then
 * calibration is done for a sample time of 1 ms, otherwise for a sample time of 10 ms. */
#define CY_PDUTILS_TIMER_CALIB_SAMPLE_TIME_1MS     (0u)

/** Macro to control whether to use a tickless timer. */
#ifndef CY_PDUTILS_TIMER_TICKLESS_ENABLE
#define CY_PDUTILS_TIMER_TICKLESS_ENABLE              (1)
#endif /* CY_PDUTILS_TIMER_TICKLESS_ENABLE */

#if CY_PDUTILS_TIMER_TICKLESS_ENABLE

/**
 * Maximum timeout value allowed. This value comprehends the maximum interrupt
 * latencies so as not to overrun. Providing for a worst-case number, 64 ticks
 * is used.
 */
#define CY_PDUTILS_TIMER_HW_MAX_TIMEOUT            (0xFFC0u)

/**
 * Time covers the worst-case interrupt latency
 * for the timer interrupt handler. Considering the worst
 * case timing of 100 us.
 */
#define CY_PDUTILS_TIMER_OVERRUN_THRESHOLD         (5u)

/** Timer counter size in the number of ticks. */
#define CY_PDUTILS_TIMER_NUM_TICK_COUNTS           (0x10000u)

#endif /* CY_PDUTILS_TIMER_TICKLESS_ENABLE */

/** Number of soft timers supported per instance of the timer module. The
   number of instances depends on the number of USB PD ports supported by
   the device. */
#define CY_PDUTILS_TIMER_NUM_TIMERS                (63u)


/** Maximum timeout value in milliseconds supported by the module. */
#define CY_PDUTILS_TIMER_MAX_TIMEOUT               (0xFFFF)

/** Invalid timer ID value. */
#define CY_PDUTILS_TIMER_INVALID_ID                (0xFFFFu)

/** Invalid timer instance. */
#define CY_PDUTILS_TIMER_INVALID_INDEX             (0xFFu)

/** Macro indicating the size of the array for storing index allotment. */
#define CY_PDUTILS_TIMER_INDEX_STATUS_ARRAY_SIZE      ((CY_PDUTILS_TIMER_NUM_TIMERS + 7u) >> 3u)

/** Start index of device-specific timers. */
#define CY_PDUTILS_TIMER_DEVICE_START_ID           (0x0000u)

/** Start index of USB PD stack timers for Port-0. */
#define CY_PDUTILS_TIMER_PD_PORT0_START_ID         (0x0100u)

/** Start index of USB PD stack timers for Port-1. */
#define CY_PDUTILS_TIMER_PD_PORT1_START_ID         (0x0200u)

/** Start index of application-level timers for Port-0. */
#define CY_PDUTILS_TIMER_APP_PORT0_START_ID        (0x0300u)

/** Start index of application-level timers for Port-1. */
#define CY_PDUTILS_TIMER_APP_PORT1_START_ID        (0x0400u)

/** Start index of alternate mode stack timers for Port-0. */
#define CY_PDUTILS_TIMER_ALT_MODE_PORT0_START_ID   (0x0500u)

/** Start index of alternate mode stack timers for Port-1. */
#define CY_PDUTILS_TIMER_ALT_MODE_PORT1_START_ID   (0x0600u)

/** Start index of wireless charging timers. */
#define CY_PDUTILS_TIMER_WLC_START_ID              (0x0700u)

/** Start of timer IDs left for generic solution level usage. */
#define CY_PDUTILS_TIMER_USER_START_ID             (0x0800u)

/** \} group_pdutils_macros */

/**
* \addtogroup group_pdutils_enums
* \{
*/
/**
 * @typedef cy_en_device_timer_id_t
 * @brief List of soft timer IDs defined for device-specific tasks.
 */
typedef enum {
    
    CY_PDUTILS_HPI_PD_CMD_TIMER = CY_PDUTILS_TIMER_DEVICE_START_ID,
    /**< 0x000: HPI PD command timer. */
    
    CY_PDUTILS_WATCHDOG_TIMER,
    /**< 0x001: Watchdog reset timer. */

    CY_PDUTILS_I2C_SLAVE_SCB0_TIMER,
    /**< 0x002: I2C transfer timeout for SCB0. */
    
    CY_PDUTILS_I2C_SLAVE_SCB1_TIMER,
    /**< 0x003: I2C transfer timeout for SCB1. */
    
    CY_PDUTILS_I2C_SLAVE_SCB2_TIMER,
    /**< 0x004: I2C transfer timeout for SCB2. */
    
    CY_PDUTILS_I2C_SLAVE_SCB3_TIMER,
    /**< 0x005: I2C transfer timeout for SCB3. */
    
    CY_PDUTILS_I2C_SLAVE_SCB4_TIMER,
    /**< 0x006: I2C transfer timeout for SCB4. */
    
    CY_PDUTILS_I2C_SLAVE_SCB5_TIMER,
    /**< 0x007: I2C transfer timeout for SCB5. */
    
    CY_PDUTILS_I2C_SLAVE_SCB6_TIMER,
    /**< 0x008: I2C transfer timeout for SCB6. */
    
    CY_PDUTILS_I2C_SLAVE_SCB7_TIMER,
    /**< 0x009: I2C transfer timeout for SCB7. */
    
    CY_PDUTILS_CCG_ACTIVITY_TIMER,
    /**< 0x00A: PD application level activity timer. */
    
    CY_PDUTILS_SYS_DEEPSLEEP_TIMER,
    /**< 0x00B: System Deep Sleep timer. */
    
} cy_en_device_timer_id_t;

/** \} group_pdutils_enums */

/*******************************************************************************
*                              Type definitions
*******************************************************************************/

/**
* \addtogroup group_pdutils_data_structures
* \{
*/

/**
 * @typedef cy_timer_id_t
 * @brief Timer ID type definition.
 *
 * Type definitions are used to identify the software timer objects. The timer ID
 * requires to be unique for each soft timer instance and needs to be maintained in
 * the application. To maintain uniqueness, the following timer ID allocation
 * rule is expected to be followed.
 *
 * - Device-specific                      :  0x0000 - 0x00FF : 256 timers
 * - PD and Type-C stack (Port-0)         :  0x0100 - 0x01FF : 256 timers
 * - PD and Type-C stack (Port-1)         :  0x0200 - 0x02FF : 256 timers
 * - Base application stack (Port-0)      :  0x0300 - 0x03FF : 256 timers
 * - Base application stack (Port-1)      :  0x0400 - 0x04FF : 256 timers
 * - Alternate mode stack (port-0)        :  0x0500 - 0x05FF : 256 timers
 * - Alternate mode stack (port-1)        :  0x0600 - 0x06FF : 256 timers
 * - Wireless charging                    :  0x0700 - 0x07FF : 256 timers
 * - Solution layer                       :  0x0800 - 0x08FF : 256 timers
 * - Reserved for future usage            :  0x0900 - 0x0FFF
 * - User space                           :  0x1000 - 0xFFFF
 */
typedef uint16_t cy_timer_id_t;

/**
 * @typedef cy_cb_timer_t
 * @brief Timer callback function.
 *
 * Callback function is invoked on timer expiry and
 * should be treated as an interrupt.
 */
typedef void (*cy_cb_timer_t)(
        cy_timer_id_t id,           /**< Timer ID for which callback is being generated. */
        void *callbackContext       /**< Timer module instance number. */
        );

/**
 * @brief Encapsulating information relating to a software timer.
 *
 * Encapsulates all the information related to a software timer
 * object. The timer module maintains a list of these objects corresponding to
 * the active software timers at any given point in time.
 */
typedef struct
{
    /** Actual count value. */
    uint32_t count;
    /** Callback function pointer for the timer object. */
    cy_cb_timer_t cb;
    /** Period of operation for the timer instance. */
    uint16_t period;
    /** ID handle associated with the timer object. */
    cy_timer_id_t id;
    /** Pointer to the callback context. */
    void *callbackContext;
    
} cy_stc_pdutils_timer_t;

/**
 * @brief Encapsulating information relating to a software timer
 * context.
 *
 */
typedef struct
{
    /** SysTick clock frequency in MHz. */
    uint32_t sys_clk_freq;

    /** Tick time at the timer start. */
    uint16_t volatile start_tick;

    /** Tick time from the start to match. */
    uint16_t volatile tick_time;

    /** Match register content. */
    uint16_t volatile match;

    /** Multiplier for 1 ms interval */
    uint16_t multiplier;

    /** Handle for software timers. */
    cy_stc_pdutils_timer_t volatile timer_handle[CY_PDUTILS_TIMER_NUM_TIMERS];

    /** Indicates timer index allotment */
    uint8_t volatile timer_index_status[CY_PDUTILS_TIMER_INDEX_STATUS_ARRAY_SIZE];

    /** Number of active timers. */
    uint8_t volatile num_active;

    /** Total number of active timers. */
    uint8_t volatile total_active;

    /** Flag indicating whether invoked from timer callback. */
    bool volatile is_callback;

    /** Pointer to the HW timer context */
    void *hw_timer_context;

} cy_stc_pdutils_sw_timer_t;

/**
 * @brief Encapsulating configuration information relating to the software timer
 * module.
 *
 */
typedef struct
{
    /** SysTick clock frequency in MHz. */
    uint32_t sys_clk_freq;

    /** Pointer to the HW timer context. */
    void *hw_timer_ctx;

} cy_stc_pdutils_timer_config_t;

/** \} group_pdutils_data_structures */


/*******************************************************************************
*                            Function prototypes
*******************************************************************************/

/**
* \addtogroup group_pdutils_functions
* \{
*/

/*******************************************************************************
* Function name: Cy_PdUtils_SwTimer_Init
****************************************************************************//**
*
* Initializes the software timer module. It
* initializes the data structures for timer management and enables the hardware
* timer used for the soft timer implementation.
*
* \param context
* Pointer to the context structure \ref cy_stc_pdutils_sw_timer_t allocated
* by the user. The structure is used during the SW timer operation for internal
* configuration and data retention. Do not modify anything in this
* structure.
*
* \param sw_timer_config
* Pointer to the SW timer module configuration context structure \ref
* cy_stc_pdutils_timer_config_t configured by the user from application space.
*
* \return
* None.
*
*******************************************************************************/
void Cy_PdUtils_SwTimer_Init(cy_stc_pdutils_sw_timer_t *context, cy_stc_pdutils_timer_config_t *sw_timer_config);
                       
/*******************************************************************************
* Function name: Cy_PdUtils_SwTimer_Start
****************************************************************************//**
*
* Starts a specific soft timer. All soft timers are one-shot timers that
* run until the specified period has elapsed. The timer expiration callback
* will be called at the end of the period if one is provided.
*
* \param context
* Pointer to the context structure \ref cy_stc_pdutils_sw_timer_t allocated
* by the user. The structure is used during the SW timer operation for internal
* configuration and data retention. Do not modify anything
* in this structure.
*
* \param callbackContext
* Pointer to the caller context.
*
* \param id
* Unique timer ID.
*
* \param period
* Timer period in milliseconds.
*
* \param cb
* Timer expiration callback. Can be NULL.
*
* \return bool
* True if the timer is started and false if the timer start fails.
*
*******************************************************************************/
bool Cy_PdUtils_SwTimer_Start(cy_stc_pdutils_sw_timer_t *context, void *callbackContext,
                       cy_timer_id_t id, uint16_t period, cy_cb_timer_t cb);

/*******************************************************************************
* Function name: Cy_PdUtils_SwTimer_StartWocb
****************************************************************************//**
*
* Starts a specific soft timer without providing a callback for expiry notification.
* The status of the timer (running or not) can be checked using the
* \ref Cy_PdUtils_SwTimer_IsRunning API.
*
* \param context
* Pointer to the context structure \ref cy_stc_pdutils_sw_timer_t allocated
* by the user. The structure is used during the SW timer operation for internal
* configuration and data retention. Do not modify anything
* in this structure.
*
* \param id
* Unique timer ID.
*
* \param period
* Timer period in milliseconds.
*
* \return bool
* True if the timer is started and false if the timer start fails.
*
*******************************************************************************/
bool Cy_PdUtils_SwTimer_StartWocb(cy_stc_pdutils_sw_timer_t *context,
                            cy_timer_id_t id, uint16_t period);

/*******************************************************************************
* Function name: Cy_PdUtils_SwTimer_Stop
****************************************************************************//**
*
* Stops a soft timer that is currently running.
*
* \param context
* Pointer to the context structure \ref cy_stc_pdutils_sw_timer_t allocated
* by the user. The structure is used during the SW timer operation for internal
* configuration and data retention. Do not modify anything
* in this structure.
*
* \param id
* Unique timer ID.
*
* \return 
* None.
*
*
*******************************************************************************/
void Cy_PdUtils_SwTimer_Stop(cy_stc_pdutils_sw_timer_t *context, cy_timer_id_t id);

/*******************************************************************************
* Function name: Cy_PdUtils_SwTimer_IsRunning
****************************************************************************//**
*
* Checks whether the specified soft timer is currently running.
*
* \param context
* Pointer to the context structure \ref cy_stc_pdutils_sw_timer_t allocated
* by the user. The structure is used during the SW timer operation for internal
* configuration and data retention. Do not modify anything
* in this structure.
*
* \param id
* Unique timer ID.
*
*
* \return bool
* True if the timer is running and false if the timer start fails.
*
*******************************************************************************/
bool Cy_PdUtils_SwTimer_IsRunning (cy_stc_pdutils_sw_timer_t *context, cy_timer_id_t id);

/*******************************************************************************
* Function name: Cy_PdUtils_SwTimer_RangeEnabled
****************************************************************************//**
*
* Checks whether any of the soft timers in the specified range are currently running.
*
* \param context
* Pointer to the context structure \ref cy_stc_pdutils_sw_timer_t allocated
* by the user. The structure is used during the SW timer operation for internal
* configuration and data retention. Do not modify anything
* in this structure.
*
* \param low
* Lowest soft timer ID to be checked.
*
* \param high
* Highest soft timer ID to be checked.
*
* \return bool
* True if the timers are running and false if the timer start fails.
*
*******************************************************************************/
bool Cy_PdUtils_SwTimer_RangeEnabled(cy_stc_pdutils_sw_timer_t *context,
                               cy_timer_id_t low, cy_timer_id_t high);

/*******************************************************************************
* Function name: Cy_PdUtils_SwTimer_GetCount
****************************************************************************//**
*
* Returns the time (in ms) remaining for timer expiration.
*
* \param context
* Pointer to the context structure \ref cy_stc_pdutils_sw_timer_t allocated
* by the user. The structure is used during the SW timer operation for internal
* configuration and data retention. Do not modify anything
* in this structure.
*
* \param id
* Unique timer ID.
*
*
* \return
* Time remaining for the expiration of the soft timer.
*
*******************************************************************************/
uint16_t Cy_PdUtils_SwTimer_GetCount(cy_stc_pdutils_sw_timer_t *context, cy_timer_id_t id);

/*******************************************************************************
* Function name: Cy_PdUtils_SwTimer_StopAll
****************************************************************************//**
*
* Stops all active soft timers.
*
* \param context
* Pointer to the context structure \ref cy_stc_pdutils_sw_timer_t allocated
* by the user. The structure is used during the SW timer operation for internal
* configuration and data retention. Do not modify anything
* in this structure.
*
* \return None
*
*******************************************************************************/
void Cy_PdUtils_SwTimer_StopAll(cy_stc_pdutils_sw_timer_t *context);

/*******************************************************************************
* Function name: Cy_PdUtils_SwTimer_StopRange
****************************************************************************//**
*
* Stops all soft timers with ids in the specified range.
*
* \param context
* Pointer to the context structure \ref cy_stc_pdutils_sw_timer_t allocated
* by the user. The structure is used during the SW Timer operation for internal
* configuration and data retention. Do not modify anything
* in this structure.
*
* \param start
* Starting timer ID. The value is inclusive.
*
* \param end
* Ending timer ID. The value is inclusive.
*
* \return 
* None.
*
*
*******************************************************************************/
void Cy_PdUtils_SwTimer_StopRange(cy_stc_pdutils_sw_timer_t *context,
                            cy_timer_id_t start, cy_timer_id_t end);

/*******************************************************************************
* Function name: Cy_PdUtils_SwTimer_NumActive
****************************************************************************//**
*
* Returns the number of active timers.
*
* \param context
* Pointer to the context structure \ref cy_stc_pdutils_sw_timer_t allocated
* by the user. The structure is used during the SW timer operation for internal
* configuration and data retention. Do not modify anything
* in this structure.
*
* \return
* Number of active timers.
*
*******************************************************************************/
uint8_t Cy_PdUtils_SwTimer_NumActive(cy_stc_pdutils_sw_timer_t *context);

/*******************************************************************************
* Function name: Cy_PdUtils_SwTimer_EnterSleep
****************************************************************************//**
*
* Prepares the timer module and the hardware timer for entering
* device Deep Sleep. This must be called before entering Deep Sleep mode.
*
* \param context
* Pointer to the context structure \ref cy_stc_pdutils_sw_timer_t allocated
* by the user. The structure is used during the SW timer operation for internal
* configuration and data retention. Do not modify anything
* in this structure.
*
* \return
* None.
*
*******************************************************************************/
void Cy_PdUtils_SwTimer_EnterSleep(cy_stc_pdutils_sw_timer_t *context);

/*******************************************************************************
* Function name: Cy_PdUtils_SwTimer_InterruptHandler
****************************************************************************//**
*
* Implements the interrupt handler callback used by the soft timer
* module and is expected to be called by the application whenever the appropriate
* SysTick or WDT interrupt is received.
*
* It scans all running timer instances and decrements their count
* as appropriate. If any of the timers has expired, it is stopped, de-allocated, and
* the corresponding callback is called (if not NULL). The timer can be restarted
* from the callback function if so desired.

* \param context
* Pointer to the context structure \ref cy_stc_pdutils_sw_timer_t allocated
* by the user. The structure is used during the SW timer operation for internal
* configuration and data retention. Do not modify anything
* in this structure.
*
* \return
* None.
*
*******************************************************************************/
void Cy_PdUtils_SwTimer_InterruptHandler(cy_stc_pdutils_sw_timer_t *context);

/*******************************************************************************
* Function name: Cy_PdUtils_SwTimer_GetMultiplier
****************************************************************************//**
*
* Retrieves the number of low frequency (LF) clock ticks required per ms.
*
* \param context
* Pointer to the context structure \ref cy_stc_pdutils_sw_timer_t allocated
* by the user. The structure is used during the SW timer operation for internal
* configuration and data retention. Do not modify anything
* in this structure.
*
* \return
* Returns the number of LF clock ticks per ms.
*
*******************************************************************************/
uint16_t Cy_PdUtils_SwTimer_GetMultiplier(cy_stc_pdutils_sw_timer_t *context);

/*******************************************************************************
* Function name: Cy_PdUtils_HwTimer_Init
****************************************************************************//**
*
* Initializes the hardware timer block used by the soft timer
* module and is called as a part of the SW timer initialization.
*
* \param context
* Pointer to the context structure \ref cy_stc_pdutils_sw_timer_t allocated
* by the user. The structure is used during the SW timer operation for internal
* configuration and data retention. Do not modify anything
* in this structure.
*
* \return
* None.
*
*******************************************************************************/
void Cy_PdUtils_HwTimer_Init(cy_stc_pdutils_sw_timer_t *context);

/*******************************************************************************
* Function name: Cy_PdUtils_HwTimer_Start
****************************************************************************//**
*
* Starts a hardware timer. The timer is expected to trigger an interrupt
* at the configured time period.
*
* \param context
* Pointer to the context structure \ref cy_stc_pdutils_sw_timer_t allocated
* by the user. The structure is used during the SW timer operation for internal
* configuration and data retention. Do not modify anything
* in this structure.
*
* \return
* None.
*
*******************************************************************************/
void Cy_PdUtils_HwTimer_Start(cy_stc_pdutils_sw_timer_t *context);

/*******************************************************************************
* Function name: Cy_PdUtils_HwTimer_Stop
****************************************************************************//**
*
* Stops the hardware timer. The timer is expected to disable interrupts
* when it is stopped.
*
* \param context
* Pointer to the context structure \ref cy_stc_pdutils_sw_timer_t allocated
* by the user. The structure is used during the SW timer operation for internal
* configuration and data retention. Do not modify anything
* in this structure.
*
* \return
* None.
*
*******************************************************************************/
void Cy_PdUtils_HwTimer_Stop(cy_stc_pdutils_sw_timer_t *context);

/*******************************************************************************
* Function name: Cy_PdUtils_HwTimer_GetCount
****************************************************************************//**
*
* Returns the reading of the current hardware timer count.
*
* \param context
* Pointer to the context structure \ref cy_stc_pdutils_sw_timer_t allocated
* by the user. The structure is used during the SW timer operation for internal
* configuration and data retention. Do not modify anything
* in this structure.
*
* \return
* Returns the elapsed hardware block timer count.
*
*******************************************************************************/
uint16_t Cy_PdUtils_HwTimer_GetCount(cy_stc_pdutils_sw_timer_t *context);

/*******************************************************************************
* Function name: Cy_PdUtils_HwTimer_LoadPeriod
****************************************************************************//**
*
* Loads the hardware time period that is to be configured for interrupt.
*
* \param context
* Pointer to the context structure \ref cy_stc_pdutils_sw_timer_t allocated
* by the user. The structure is used during the SW timer operation for internal
* configuration and data retention. Do not modify anything
* in this structure.
*
* \param period
* Time period to be configured for interrupt.
*
* \return
* None.
*
*******************************************************************************/
void Cy_PdUtils_HwTimer_LoadPeriod(cy_stc_pdutils_sw_timer_t *context, uint16_t period);

#endif /* _CY_PDUTILS_SW_TIMER_H_ */

/** \} group_pdutils_functions */

/* [] END OF FILE */
