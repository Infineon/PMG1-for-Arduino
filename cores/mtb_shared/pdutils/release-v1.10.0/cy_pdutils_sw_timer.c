/***************************************************************************//**
* \file cy_pdutils_sw_timer.c
* \version 1.10
*
* Software timer source file.
*
********************************************************************************
* \copyright
* Copyright 2022-2023, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "cy_pdutils_sw_timer.h"
#include "cy_pdl.h"

#if (CY_PDUTILS_TIMER_TYPE == CY_PDUTILS_TIMER_TYPE_PD_ILO)
#include "cy_usbpd_common.h"
#include "cy_usbpd_defines.h"
#include "cy_usbpd_lf_counter.h"
#endif /* (CY_PDUTILS_TIMER_TYPE == CY_PDUTILS_TIMER_TYPE_PD_ILO) */

/**
 * @brief Calibrates the hardware timer module.
 *
 * Calibrates the WDT hw timer module used for
 * the soft timer implementation.
 *
 * @return None.
 */
#if (CY_PDUTILS_TIMER_TYPE == CY_PDUTILS_TIMER_TYPE_WDT)
static void Cy_PdUtils_SwTimer_WdtCalibrate(cy_stc_pdutils_sw_timer_t *context)
{
    volatile uint32_t start_count, end_count;

    /* Ensure that the timer is disabled. */
    Cy_SysTick_Disable() ;

    /* Clear the counter. */
    Cy_SysTick_Clear();

#if (CY_PDUTILS_TIMER_CALIB_SAMPLE_TIME_1MS != 1u)
    /* Load 10 ms timeout. */
    Cy_SysTick_SetReload((context->sys_clk_freq * 10u / 1000u) - 1u);
#else
    /* Load 1 ms timeout. */
    Cy_SysTick_SetReload((context->sys_clk_freq / 1000u) - 1u);
#endif /* (CY_PDUTILS_TIMER_CALIB_SAMPLE_TIME_1MS != 1u) */

    Cy_SysTick_SetClockSource(CY_SYSTICK_CLOCK_SOURCE_CLK_CPU);

    /*
     * Wait for the WDT counter to change. After that, ensure the
     * full calibration time.
     */
     start_count = Cy_WDT_GetCount();
     while (start_count == Cy_WDT_GetCount())
     {
     }

    /* Note the WDT count value. */
    start_count = Cy_WDT_GetCount();

    /* 
     * Do not add any additional instructions here because you need to start the systick timer immediately on the
     * falling edge of the clock.
     */
    Cy_SysTick_Enable();
    /* Disable the systick interrupt because it is enabled implicitly in the Cy_SysTick_Enable function. */
    Cy_SysTick_DisableInterrupt();

    while (Cy_SysTick_GetCountFlag() == 0U)
    {
    }

    end_count = Cy_WDT_GetCount();

    /* Calculate the actual calibration value. */
    if (start_count > end_count)
    {
        /* Counter had rolled over. */
        end_count |= 0x10000U;
    }
    /* Store the value for reload. */
    context->multiplier = (uint16_t)(end_count - start_count);

    Cy_SysTick_Disable() ;
}
#endif /* (CY_PDUTILS_TIMER_TYPE == CY_PDUTILS_TIMER_TYPE_WDT) */

/**
 * @brief Calibrate the hardware timer module.
 *
 * Calibrates the ILO based hw timer module used for
 * the soft timer implementation.
 *
 * @return None
 */
#if (CY_PDUTILS_TIMER_TYPE == CY_PDUTILS_TIMER_TYPE_PD_ILO)
static void Cy_PdUtils_SwTimer_LfCalibrate(cy_stc_pdutils_sw_timer_t *context)
{
    volatile uint32_t start_count, end_count;

    /* Ensure that the timer is disabled. */
    Cy_SysTick_Disable() ;

    /* Clear the counter. */
    Cy_SysTick_Clear();

#if (CY_PDUTILS_TIMER_CALIB_SAMPLE_TIME_1MS != 1u)
    /* Load 10 ms timeout. */
    Cy_SysTick_SetReload((context->sys_clk_freq * 10u/ 1000u) - 1u);
#else
    /* Load 1 ms timeout. */
    Cy_SysTick_SetReload((context->sys_clk_freq / 1000u) - 1u);
#endif /* (CY_PDUTILS_TIMER_CALIB_SAMPLE_TIME_1MS != 1u) */

    Cy_SysTick_SetClockSource(CY_SYSTICK_CLOCK_SOURCE_CLK_CPU);

    /*
     * Wait for the LF counter to change. After that, ensure the
     * full calibration time.
     */
     start_count = Cy_LF_GetCount(context->hw_timer_context);
     while (start_count == Cy_LF_GetCount(context->hw_timer_context))
     {
     }

    /* Note the LF count value. */
    start_count = Cy_LF_GetCount(context->hw_timer_context);

    /* 
     * Do not add any additional instructions here because you need to start the systick timer immediately on the
     * falling edge of the clock.
     */
    Cy_SysTick_Enable();
    /* Disable the systick interrupt because it is enabled implicitly in the Cy_SysTick_Enable function. */    
    Cy_SysTick_DisableInterrupt();

    while (Cy_SysTick_GetCountFlag() == 0U)
    {
    }

    end_count = Cy_LF_GetCount(context->hw_timer_context);

    /* Calculate the actual calibration value. */
    if (start_count > end_count)
    {
        /* Counter had rolled over. */
        end_count |= 0x10000U;
    }
    /* Store the value for reload. */
    context->multiplier = (uint16_t)(end_count - start_count);
    Cy_SysTick_Disable();
}
#endif /* (CY_PDUTILS_TIMER_TYPE == CY_PDUTILS_TIMER_TYPE_PD_ILO) */

void Cy_PdUtils_HwTimer_Init(cy_stc_pdutils_sw_timer_t *context)
{
#if (CY_PDUTILS_TIMER_TYPE == CY_PDUTILS_TIMER_TYPE_WDT)

    /* Disable the WDT timer interrupts. */
    Cy_WDT_MaskInterrupt(); 

    /* Disable the WDT reset generation. */
    Cy_WDT_Disable();

    /* Forward the interrupt to the CPU. */
    Cy_WDT_UnmaskInterrupt();

    /* Start ILO. */
    Cy_SysClk_IloEnable() ;
    /* Enable the watchdog timer interrupt. */
    Cy_WDT_ClearInterrupt();

    /* Calibrate the WDT timer. */
    Cy_PdUtils_SwTimer_WdtCalibrate (context);
#elif (CY_PDUTILS_TIMER_TYPE == CY_PDUTILS_TIMER_TYPE_PD_ILO)

    /* Disable the LF timer interrupts. */
    Cy_LF_MaskInterrupt(context->hw_timer_context);

    /* Forward the interrupt to the CPU. */
    Cy_LF_UnmaskInterrupt(context->hw_timer_context);

    /* Start ILO. */
    Cy_SysClk_IloEnable() ;

    /* Enable the LF timer interrupt. */
    Cy_LF_ClearInterrupt(context->hw_timer_context);

    /* Calibrate the LF timer. */
    Cy_PdUtils_SwTimer_LfCalibrate (context);

#else /* (CY_PDUTILS_TIMER_TYPE == CY_PDUTILS_TIMER_TYPE_SYSTICK) */

    /* Disable the timer. */
    Cy_SysTick_Disable() ;
    Cy_SysTick_SetReload((context->sys_clk_freq / 1000u) - 1u);

#endif /* (CY_PDUTILS_TIMER_TYPE) */
}

/*
 * Starts the hardware timer. The timer is expected to start
 * interrupt at the configured time.
 */
void Cy_PdUtils_HwTimer_Start(cy_stc_pdutils_sw_timer_t *context)
{
    CY_UNUSED_PARAMETER(context);
#if (CY_PDUTILS_TIMER_TYPE == CY_PDUTILS_TIMER_TYPE_WDT)

#if (CY_PDUTILS_TIMER_TICKLESS_ENABLE == 0)
    /* Load the timer match register. */
#ifdef SRSSULT
    SRSSULT->wdt_match = (uint16_t)((SRSSULT->wdt_counter & CY_PDUTILS_WDT_COUNTER_COUNTER_MASK)
            + context->multiplier);
#else /* SRSSLT */
    SRSSLT->WDT_MATCH = (uint16_t)((SRSSLT->WDT_COUNTER & CY_PDUTILS_WDT_COUNTER_COUNTER_MASK)
            + context->multiplier);
#endif /* SRSS */
    /* Need to wait for at least 3 LF cycles before the register write completes. */
    Cy_SysLib_DelayUs(100);
#endif /* (CY_PDUTILS_TIMER_TICKLESS_ENABLE == 0) */

    /* Enable the WDT interrupt. */
    Cy_WDT_UnmaskInterrupt();
#elif (CY_PDUTILS_TIMER_TYPE == CY_PDUTILS_TIMER_TYPE_PD_ILO)

#if (CY_PDUTILS_TIMER_TICKLESS_ENABLE == 0)
    cy_stc_usbpd_context_t *ctx = (cy_stc_usbpd_context_t *)(context->hw_timer_context);

    ctx->base->lf_cntr_match = (uint16_t)((ctx->base->lf_cntr & PDSS_LF_CNTR_COUNTER_MASK)
            + context->multiplier);

    /* Need to wait for at least 3 LF cycles before the register write completes. */
    Cy_SysLib_DelayUs(100);
#endif /* (CY_PDUTILS_TIMER_TICKLESS_ENABLE == 0) */

    /* Enable the LF interrupt. */
    Cy_LF_UnmaskInterrupt(context->hw_timer_context);

#else /* (CY_PDUTILS_TIMER_TYPE == CY_PDUTILS_TIMER_TYPE_SYSTICK) */

    /* Clear the counter. */
    Cy_SysTick_Clear() ;

    /* Start the timer and enable the interrupt. */
    Cy_SysTick_SetClockSource(CY_SYSTICK_CLOCK_SOURCE_CLK_CPU);
    Cy_SysTick_Enable();

#endif /* (CY_PDUTILS_TIMER_TYPE) */
}

/*
 * Stops the hardware timer. The timer is expected to
 * disable interrupts when stopped.
 */
void Cy_PdUtils_HwTimer_Stop(cy_stc_pdutils_sw_timer_t *context)
{
    CY_UNUSED_PARAMETER(context);    
#if (CY_PDUTILS_TIMER_TYPE == CY_PDUTILS_TIMER_TYPE_WDT)
    /* Disable the WDT interrupt. */
    Cy_WDT_MaskInterrupt();
#elif (CY_PDUTILS_TIMER_TYPE == CY_PDUTILS_TIMER_TYPE_PD_ILO)
    /* Disable the LF interrupt. */
    Cy_LF_MaskInterrupt(context->hw_timer_context);    
#else /* (CY_PDUTILS_TIMER_TYPE == CY_PDUTILS_TIMER_TYPE_SYSTICK) */
    /* Disable the SYSTICK interrupt. */
    Cy_SysTick_Disable() ;
#endif /* (CY_PDUTILS_TIMER_TYPE) */
}

/* Retrieves the current hardware count reading. */
uint16_t Cy_PdUtils_HwTimer_GetCount(cy_stc_pdutils_sw_timer_t *context)
{
#if (CY_PDUTILS_TIMER_TICKLESS_ENABLE != 0)
#if (CY_PDUTILS_TIMER_TYPE == CY_PDUTILS_TIMER_TYPE_WDT)
    CY_UNUSED_PARAMETER(context);
    return (uint16_t)Cy_WDT_GetCount() ;
#elif (CY_PDUTILS_TIMER_TYPE == CY_PDUTILS_TIMER_TYPE_PD_ILO)

    /* Disable the LF interrupt. */
    return (uint16_t)Cy_LF_GetCount(context->hw_timer_context);
#else /* (CY_PDUTILS_TIMER_TYPE == CY_PDUTILS_TIMER_TYPE_SYSTICK) */
    return 0;
#endif /* (CY_PDUTILS_TIMER_TYPE) */
#else /* CY_PDUTILS_TIMER_TICKLESS_ENABLE == 0 */
    CY_UNUSED_PARAMETER(context);
    return 0;
#endif /* (CY_PDUTILS_TIMER_TICKLESS_ENABLE != 0) */
}

/* Loads the hardware period for interrupt. */
void Cy_PdUtils_HwTimer_LoadPeriod(cy_stc_pdutils_sw_timer_t *context, uint16_t period)
{
#if (CY_PDUTILS_TIMER_TICKLESS_ENABLE != 0)
#if (CY_PDUTILS_TIMER_TYPE == CY_PDUTILS_TIMER_TYPE_WDT)

    if (Cy_WDT_GetMatch() != context->match)
    {
        Cy_SysLib_DelayUs(100);
    }
    Cy_WDT_SetMatch(period);
    context->match = period;
#elif (CY_PDUTILS_TIMER_TYPE == CY_PDUTILS_TIMER_TYPE_PD_ILO)
    if (Cy_LF_GetMatch(context->hw_timer_context) != context->match)
    {
        Cy_SysLib_DelayUs(100);
    }
    Cy_LF_SetMatch(context->hw_timer_context, period);
    context->match = period;    
#else /* (CY_PDUTILS_TIMER_TYPE == CY_PDUTILS_TIMER_TYPE_SYSTICK) */
#endif /* (CY_PDUTILS_TIMER_TYPE) */
#else /* CY_PDUTILS_TIMER_TICKLESS_ENABLE == 0 */
    CY_UNUSED_PARAMETER(context);
    CY_UNUSED_PARAMETER(period);    
#endif /* (CY_PDUTILS_TIMER_TICKLESS_ENABLE != 0) */
}

#if (CY_PDUTILS_TIMER_TICKLESS_ENABLE != 0)
#if (!PAG2S_SROM_CODE || (PAG2S_SROM_CODE && GENERATE_SROM_CODE))
TIMER_ATTRIBUTES static uint16_t Cy_PdUtils_HwTimer_GetTickInterval(uint16_t start, uint16_t current)
{
    uint32_t interval;

    if (start < current)
    {
        interval = ((uint32_t)current - (uint32_t)start);
    }
    else
    {
        interval = (CY_PDUTILS_TIMER_NUM_TICK_COUNTS - start) + current;
    }

    return (uint16_t)interval;
}
#endif /* (!PAG2S_SROM_CODE || (PAG2S_SROM_CODE && GENERATE_SROM_CODE)) */
#endif /* (CY_PDUTILS_TIMER_TICKLESS_ENABLE != 0) */

#if (!PAG2S_SROM_CODE || (PAG2S_SROM_CODE && GENERATE_SROM_CODE))
TIMER_ATTRIBUTES void Cy_PdUtils_SwTimer_Init(cy_stc_pdutils_sw_timer_t *context, cy_stc_pdutils_timer_config_t *sw_timer_config)
{
    uint8_t i;

    /* Invalidate all timers. */
    for (i = 0u; i < CY_PDUTILS_TIMER_NUM_TIMERS; i++)
    {
        context->timer_handle[i].id = CY_PDUTILS_TIMER_INVALID_ID;
    }

    for (i = 0u; i < CY_PDUTILS_TIMER_INDEX_STATUS_ARRAY_SIZE; i++)
    {
        context->timer_index_status[i] = 0u;
    }

    context->num_active = 0u;
    context->total_active = 0u;
    context->sys_clk_freq = sw_timer_config->sys_clk_freq;
    context->hw_timer_context = sw_timer_config->hw_timer_ctx;

    /* Initialize the hardware timer. */
    TIMER_CALL_MAP(Cy_PdUtils_HwTimer_Init)(context);
}


/* Internal function to scan the instances to identify the allocated timer. */
TIMER_ATTRIBUTES static uint8_t Cy_PdUtils_Timer_GetIndex(cy_stc_pdutils_sw_timer_t *context, cy_timer_id_t id)
{
    uint8_t index, loop;
    uint8_t end_index;

    if (id == CY_PDUTILS_TIMER_INVALID_ID)
    {
        return CY_PDUTILS_TIMER_INVALID_INDEX;
    }

    for (loop = 0u; loop < CY_PDUTILS_TIMER_INDEX_STATUS_ARRAY_SIZE; loop++)
    {
        if(context->timer_index_status[loop] != 0x00u)
        {
            end_index = ((loop + 1u) << 3u);
            if (end_index > CY_PDUTILS_TIMER_NUM_TIMERS)
            {
                end_index = CY_PDUTILS_TIMER_NUM_TIMERS;
            }
            for (index = (loop << 3u); index < end_index; index++)
            {
                if (id == context->timer_handle[index].id)
                {
                    return index;
                }
            }
        }
    }

    return CY_PDUTILS_TIMER_INVALID_INDEX;
}

TIMER_ATTRIBUTES bool Cy_PdUtils_SwTimer_Start(cy_stc_pdutils_sw_timer_t *context, void *callbackContext,
                       cy_timer_id_t id, uint16_t period, cy_cb_timer_t cb)
{
    uint8_t j;
    int8_t i;
    int8_t start_index;
    uint32_t state;
    bool status = false;
    uint8_t index = CY_PDUTILS_TIMER_INVALID_INDEX;

    volatile cy_stc_pdutils_timer_t *p_timer_handle = (cy_stc_pdutils_timer_t *)&(context->timer_handle);

    if ((id == CY_PDUTILS_TIMER_INVALID_ID) || (period == 0U))
    {
        return false;
    }

    /* Enter the critical section. */
    state = SYS_CALL_MAP(Cy_SysLib_EnterCriticalSection)();

    /*
     * Allocates a free instance. Two checks need to be done. First, you need
     * to identify if the ID is already active. If so, you should re-use the
     * same ID. If the timer ID is not active, then you should look for a
     * free slot. Combine the two loops to improve performance.
     * Therefore, to make the allocation happen from zero up, the loop is run from
     * the highest to lowest.
     */

    for (j = CY_PDUTILS_TIMER_INDEX_STATUS_ARRAY_SIZE; j > 0; j--)
    {
        if(context->timer_index_status[(j - 1u)] == 0x00u)
        {
            /* Pick a top instance. */
            index = ((j - 1u) << 3u);
        }
        else
        {
            start_index = ((int8_t)(j << 3u) - 1);
            if(start_index >= (int8_t)CY_PDUTILS_TIMER_NUM_TIMERS)
            {
                start_index = ((int8_t)CY_PDUTILS_TIMER_NUM_TIMERS - 1);
            }
            for(i = start_index; i >= (int8_t)((j - 1u) << 3u); i--)
            {
                /* Pick a free instance. */
                if (p_timer_handle[i].id == CY_PDUTILS_TIMER_INVALID_ID)
                {
                    index = (uint8_t)i;
                }
                /*
                * Scan again if the same timer ID has been previously allocated.
                * If so, return the previously allocated instance.
                */
                else if (p_timer_handle[i].id == id)
                {
                    index = (uint8_t)i;
                    /*
                    * Re-allocate an already running timer.
                    * This means that the previous instance is disabled.
                    */
                    status = true;
                    context->num_active--;
                    context->total_active--;
                    break;
                }
                else
                {
                    /* No Statement. */
                }
            }
            if (status)
            {
                status = false;
                break;
            }
        }
    }

    if (index != CY_PDUTILS_TIMER_INVALID_INDEX)
    {
        p_timer_handle[index].period = period;
        p_timer_handle[index].cb = cb;
        p_timer_handle[index].id = id;
        p_timer_handle[index].callbackContext = callbackContext;

        /* Updating the status of allocation of the index. */
        context->timer_index_status[index >> 3u] |= (0x01u << (index & 7u));

#if (CY_PDUTILS_TIMER_TICKLESS_ENABLE != 0)

        /* Multiply the required time with the multiplier. */
#if (CY_PDUTILS_TIMER_CALIB_SAMPLE_TIME_1MS != 1u)
        p_timer_handle[index].count = (((uint32_t)period * (uint32_t)context->multiplier) +5u) / 10u;
#else
        p_timer_handle[index].count = ((uint32_t)period * (uint32_t)context->multiplier);
#endif /* (CY_PDUTILS_TIMER_CALIB_SAMPLE_TIME_1MS != 1u) */
        /*
         * Only start or adjust the timer if outside of the timer callback.
         * When inside timer callback, the timer is already started and
         * no adjustment is required.
         */
        if (context->is_callback == false)
        {
            uint32_t count;
            uint16_t cur_count;


            cur_count = TIMER_CALL_MAP(Cy_PdUtils_HwTimer_GetCount) (context);
            /*
             * If the timer is already running, then the count has
             * to be adjusted to take care of partial updates.
             */
            if (context->total_active != 0U)
            {
                count = Cy_PdUtils_HwTimer_GetTickInterval (context->start_tick,
                        cur_count);
                p_timer_handle[index].count += count;

                if (p_timer_handle[index].count < context->tick_time)
                {
                    /*
                     * It is safe to load the timer here as it is
                     * confirmed that the timer is not going to fire
                     * immediately. In addition, ensure that the
                     * count is less than the maximum value allowed.
                     */
                    context->tick_time = (uint16_t)(p_timer_handle[index].count);


                    TIMER_CALL_MAP(Cy_PdUtils_HwTimer_LoadPeriod) (context, (uint16_t) (context->start_tick +
                                                                context->tick_time));
                }
            }
            else
            {
                /* Start the timer if not already running. */
                context->start_tick = cur_count;
                count = p_timer_handle[index].count;
                if (count > CY_PDUTILS_TIMER_HW_MAX_TIMEOUT)
                {
                    count = CY_PDUTILS_TIMER_HW_MAX_TIMEOUT;
                }
                context->tick_time = (uint16_t)count;
                TIMER_CALL_MAP(Cy_PdUtils_HwTimer_LoadPeriod) (context,(uint16_t) (count + cur_count));
                TIMER_CALL_MAP(Cy_PdUtils_HwTimer_Start) (context);
            }
        }

#else /* (CY_PDUTILS_TIMER_TICKLESS_ENABLE == 0) */

        p_timer_handle[index].count = period;
        /*
         * Turn on the hardware timer if not running. Do this only
         * if invoked outside of timer callback.
         */
        if ((context->total_active == 0U) && (context->is_callback == false))
        {
            TIMER_CALL_MAP(Cy_PdUtils_HwTimer_Start) (context);
        }

#endif /* (CY_PDUTILS_TIMER_TICKLESS_ENABLE) */

        context->num_active++;
        context->total_active++;
        status = true;
    }

    /* Exit critical section. */
    SYS_CALL_MAP(Cy_SysLib_ExitCriticalSection)(state);

    return status;
}

TIMER_ATTRIBUTES bool Cy_PdUtils_SwTimer_StartWocb(cy_stc_pdutils_sw_timer_t *context, cy_timer_id_t id, uint16_t period)
{
    return TIMER_CALL_MAP(Cy_PdUtils_SwTimer_Start)(context, NULL, id, period, NULL);
}

/*
 * Should not be called other than from timer_stop functions.
 * It expects that the call is being made from inside a critical
 * section and that the timer instance and the index into the timer handle
 * is valid.
 */
TIMER_ATTRIBUTES static void Cy_PdUtils_Timer_StopInternal(cy_stc_pdutils_sw_timer_t *context, uint8_t index)
{
    /* Stop the timer and deallocate it. */
    context->timer_handle[index].id = CY_PDUTILS_TIMER_INVALID_ID;
    context->num_active--;
    context->total_active--;

    /* Updating the status of the de-allocation of the index. */
    context->timer_index_status[index >> 3u] &= (~(0x01u << (index & 7u)));

    /*
     * Turn OFF the hardware timer if no timer is active. Turn OFF should
     * be done only if not being invoked from inside a timer callback.
     * If this is from timer callback, the interrupt handler deals
     * with this at the end of all timer checks.
     */
    if ((context->total_active == 0U) && (context->is_callback == false))
    {
        TIMER_CALL_MAP(Cy_PdUtils_HwTimer_Stop)(context);
    }
}

TIMER_ATTRIBUTES void Cy_PdUtils_SwTimer_Stop(cy_stc_pdutils_sw_timer_t *context, cy_timer_id_t id)
{
    uint8_t index = 0U;
    uint32_t state = 0U;

    /* Enter the critical section. */
    state = SYS_CALL_MAP(Cy_SysLib_EnterCriticalSection)();

    /* Get the timer index from the handle. */
    index = Cy_PdUtils_Timer_GetIndex (context, id);
    if (index != CY_PDUTILS_TIMER_INVALID_INDEX)
    {
        Cy_PdUtils_Timer_StopInternal(context, index);
    }

    /* Exit the critical section. */
    SYS_CALL_MAP(Cy_SysLib_ExitCriticalSection)(state);
}

TIMER_ATTRIBUTES void Cy_PdUtils_SwTimer_StopAll(cy_stc_pdutils_sw_timer_t *context)
{
    uint8_t index = 0U;
    uint8_t loop = 0U;
    uint8_t end_index;
    uint32_t state = 0U;

    /* Enter the critical section. */
    state = SYS_CALL_MAP(Cy_SysLib_EnterCriticalSection) ();

    for (loop = 0u; loop < CY_PDUTILS_TIMER_INDEX_STATUS_ARRAY_SIZE; loop++)
    {
        if(context->timer_index_status[loop] != 0x00u)
        {
            end_index = ((loop + 1u) << 3u);
            if (end_index > CY_PDUTILS_TIMER_NUM_TIMERS)
            {
                end_index = CY_PDUTILS_TIMER_NUM_TIMERS;
            }
            for (index = (loop << 3u); index < end_index; index++)
            {
                /* Stop the timer only for a valid ID. */
                if (context->timer_handle[index].id != CY_PDUTILS_TIMER_INVALID_ID)
                {
                    Cy_PdUtils_Timer_StopInternal(context, index);
                }
            }
        }
    }

    /* Exit the critical section. */
    SYS_CALL_MAP(Cy_SysLib_ExitCriticalSection)(state);
}

TIMER_ATTRIBUTES void Cy_PdUtils_SwTimer_StopRange(cy_stc_pdutils_sw_timer_t *context, cy_timer_id_t start, cy_timer_id_t end)
{
    uint8_t index = 0U;
    uint8_t loop = 0U;
    uint8_t end_index;
    uint32_t state = 0U;

    /*
     * Should not allow the stop call to happen if the range is invalid.
     * Assuming that the CY_PDUTILS_TIMER_INVALID_ID is the largest possible
     * value for the ID. If not would have to explicitly check for validity
     * of start parameter as well.
     */
    if ((start >= end) || (end == CY_PDUTILS_TIMER_INVALID_ID))
    {
        return;
    }

    /* Enter the critical section. */
    state = SYS_CALL_MAP(Cy_SysLib_EnterCriticalSection)();

    for (loop = 0u; loop < CY_PDUTILS_TIMER_INDEX_STATUS_ARRAY_SIZE; loop++)
    {
        if(context->timer_index_status[loop] != 0x00u)
        {
            end_index = ((loop + 1u) << 3u);
            if (end_index > CY_PDUTILS_TIMER_NUM_TIMERS)
            {
                end_index = CY_PDUTILS_TIMER_NUM_TIMERS;
            }
            for (index = (loop << 3u); index < end_index; index++)
            {
                cy_timer_id_t id = context->timer_handle[index].id;
                /* Stop the timer only for the specified ID range. */
                if ((id >= start) && (id <= end))
                {
                    Cy_PdUtils_Timer_StopInternal(context, index);
                }
            }
        }
    }

    /* Exit the critical section. */
    SYS_CALL_MAP(Cy_SysLib_ExitCriticalSection)(state);
}

TIMER_ATTRIBUTES bool Cy_PdUtils_SwTimer_IsRunning (cy_stc_pdutils_sw_timer_t *context, cy_timer_id_t id)
{
    uint8_t index;

    index = Cy_PdUtils_Timer_GetIndex (context, id);
    if (index != CY_PDUTILS_TIMER_INVALID_INDEX)
    {
        return true;
    }

    return false;
}

TIMER_ATTRIBUTES bool Cy_PdUtils_SwTimer_RangeEnabled(cy_stc_pdutils_sw_timer_t *context, cy_timer_id_t low, cy_timer_id_t high)
{
    cy_timer_id_t id;

    for (id = low; id <= high; id++)
    {
        if (Cy_PdUtils_Timer_GetIndex (context, id) != CY_PDUTILS_TIMER_INVALID_INDEX)
        {
            return true;
        }
    }

    return false;
}

TIMER_ATTRIBUTES uint16_t Cy_PdUtils_SwTimer_GetCount (cy_stc_pdutils_sw_timer_t *context, cy_timer_id_t id)
{
    uint8_t index = 0U;
    uint32_t state = 0U;
    uint32_t count = 0U;

    /* Enter the critical section. */
    state = SYS_CALL_MAP(Cy_SysLib_EnterCriticalSection) ();

    index = Cy_PdUtils_Timer_GetIndex (context, id);
    if(index != CY_PDUTILS_TIMER_INVALID_INDEX)
    {
#if (CY_PDUTILS_TIMER_TICKLESS_ENABLE != 0)
        uint32_t temp_count = context->timer_handle[index].count;

        count = Cy_PdUtils_HwTimer_GetTickInterval(context->start_tick, TIMER_CALL_MAP(Cy_PdUtils_HwTimer_GetCount)(context));
        if (temp_count > count)
        {
            /** Multiplier is for 10 ms; ensure to scale it back to 1 ms and return */
#if (CY_PDUTILS_TIMER_CALIB_SAMPLE_TIME_1MS != 1u)
            count = (((temp_count - count) * 10u)) / (context->multiplier);
#else
            count = ((temp_count - count) / context->multiplier);
#endif /* (CY_PDUTILS_TIMER_CALIB_SAMPLE_TIME_1MS != 1u) */
        }
        else
        {
            count = 0;
        }
#else /* (CY_PDUTILS_TIMER_TICKLESS_ENABLE == 0) */
        count = context->timer_handle[index].count;
#endif /* (CY_PDUTILS_TIMER_TICKLESS_ENABLE) */
    }

    /* Exit the critical section. */
    SYS_CALL_MAP(Cy_SysLib_ExitCriticalSection) (state);

    return (uint16_t)count;
}

TIMER_ATTRIBUTES uint16_t Cy_PdUtils_SwTimer_GetMultiplier(cy_stc_pdutils_sw_timer_t *context)
{
#if ((CY_PDUTILS_TIMER_TYPE == CY_PDUTILS_TIMER_TYPE_WDT) || (CY_PDUTILS_TIMER_TYPE == CY_PDUTILS_TIMER_TYPE_PD_ILO))
#if (CY_PDUTILS_TIMER_CALIB_SAMPLE_TIME_1MS != 1u)
    return((context->multiplier + 5u) / 10u);
#else
    return(context->multiplier);
#endif /* (CY_PDUTILS_TIMER_CALIB_SAMPLE_TIME_1MS != 1u) */
#else
    /* Value is not known because calibration is not run. Return a nominal value assuming LFCLK of 40 kHz. */
    CY_UNUSED_PARAMETER(context);
    return(25);
#endif /* CY_PDUTILS_TIMER_TYPE */
}
#endif /* (!PAG2S_SROM_CODE || (PAG2S_SROM_CODE && GENERATE_SROM_CODE)) */

void Cy_PdUtils_SwTimer_EnterSleep(cy_stc_pdutils_sw_timer_t *context)
{
#if (CY_PDUTILS_TIMER_TYPE == CY_PDUTILS_TIMER_TYPE_WDT)
    if (context->total_active != 0U)
    {
#if (CY_PDUTILS_TIMER_TICKLESS_ENABLE != 0)

        if (Cy_WDT_GetMatch() != context->match)
#endif /* (CY_PDUTILS_TIMER_TICKLESS_ENABLE != 0) */
        {
            /* To allow WDT registers to synchronize. */
            SYS_CALL_MAP(Cy_SysLib_DelayUs)(50);
        }
    }
#elif (CY_PDUTILS_TIMER_TYPE == CY_PDUTILS_TIMER_TYPE_PD_ILO)
    if (context->total_active != 0U)
    {
        if (Cy_LF_GetMatch(context->hw_timer_context) != context->match)
        {
            /* To allow LF registers to synchronize. */
            SYS_CALL_MAP(Cy_SysLib_DelayUs)(50);
        }
    }
#endif /* (CY_PDUTILS_TIMER_TYPE) */
    CY_UNUSED_PARAMETER(context);
}

#if (!PAG2S_SROM_CODE || (PAG2S_SROM_CODE && GENERATE_SROM_CODE))
TIMER_ATTRIBUTES uint8_t Cy_PdUtils_SwTimer_NumActive(cy_stc_pdutils_sw_timer_t *context)
{
    return context->num_active;
}

TIMER_ATTRIBUTES void Cy_PdUtils_SwTimer_InterruptHandler(cy_stc_pdutils_sw_timer_t *context)
{
    uint8_t index;
    uint8_t loop;
    uint8_t end_index;
    cy_timer_id_t id;
#if (CY_PDUTILS_TIMER_TICKLESS_ENABLE != 0)
    uint16_t cur_count;
    uint32_t interval, period;
#endif /* (CY_PDUTILS_TIMER_TICKLESS_ENABLE != 0) */

    volatile cy_stc_pdutils_timer_t *p_timer_handle;

    /* Indicate being inside the interrupt handler. */
    context->is_callback = true;

    do
    {
#if (CY_PDUTILS_TIMER_TICKLESS_ENABLE != 0)
        cur_count = TIMER_CALL_MAP(Cy_PdUtils_HwTimer_GetCount) (context);
        interval = Cy_PdUtils_HwTimer_GetTickInterval (context->start_tick, cur_count);

        context->start_tick = cur_count;
#endif /* (CY_PDUTILS_TIMER_TICKLESS_ENABLE != 0) */


        p_timer_handle = (cy_stc_pdutils_timer_t *)&(context->timer_handle);
        /* Loop through all the soft timers. */
        for (loop = 0u; loop < CY_PDUTILS_TIMER_INDEX_STATUS_ARRAY_SIZE; loop++)
        {
            if(context->timer_index_status[loop] != 0x00u)
            {
                end_index = ((loop + 1u) << 3u);
                if (end_index > CY_PDUTILS_TIMER_NUM_TIMERS)
                {
                    end_index = CY_PDUTILS_TIMER_NUM_TIMERS;
                }
                for (index = (loop << 3u); index < end_index; index++)
                {
                    id = p_timer_handle[index].id;
                    if (id != CY_PDUTILS_TIMER_INVALID_ID)
                    {
#if (CY_PDUTILS_TIMER_TICKLESS_ENABLE != 0)
                        /* Decrement the counters. */
                        if (p_timer_handle[index].count >
                                (interval + CY_PDUTILS_TIMER_OVERRUN_THRESHOLD))
                        {
                            p_timer_handle[index].count -= interval;
                        }
                        else
                        {
                            p_timer_handle[index].count = 0;
                        }
#else /* (CY_PDUTILS_TIMER_TICKLESS_ENABLE == 0) */
                        p_timer_handle[index].count--;
#endif /* (CY_PDUTILS_TIMER_TICKLESS_ENABLE) */
                        if (p_timer_handle[index].count == 0U)
                        {
                            p_timer_handle[index].id = CY_PDUTILS_TIMER_INVALID_ID;
                            /* Updating the status of the de-allocation of the index. */
                            context->timer_index_status[index >> 3u] &= (~(0x01u << (index & 7u)));
                            context->num_active--;
                            context->total_active--;
                            if (p_timer_handle[index].cb != NULL)
                            {
                                p_timer_handle[index].cb (id, p_timer_handle[index].callbackContext);
                            }
                        }
                    }
                }
            }
        }


#if (CY_PDUTILS_TIMER_TICKLESS_ENABLE != 0)
        period = CY_PDUTILS_TIMER_HW_MAX_TIMEOUT;
        interval = 0;
        if (context->total_active != 0U)
        {
            /* Identify the next timer setting. */
            p_timer_handle = (cy_stc_pdutils_timer_t *)&(context->timer_handle);
            for (loop = 0u; loop < CY_PDUTILS_TIMER_INDEX_STATUS_ARRAY_SIZE; loop++)
            {
                if(context->timer_index_status[loop] != 0x00u)
                {
                    end_index = ((loop + 1u) << 3u);
                    if (end_index > CY_PDUTILS_TIMER_NUM_TIMERS)
                    {
                        end_index = CY_PDUTILS_TIMER_NUM_TIMERS;
                    }
                    for (index = (loop << 3u); index < end_index; index++)
                    {
                        if (p_timer_handle[index].id != CY_PDUTILS_TIMER_INVALID_ID)
                        {
                            if (p_timer_handle[index].count < period)
                            {
                                period = p_timer_handle[index].count;
                            }
                        }
                    }
                }
            }

            /*
             * Because there is a remote possibility of missing the timer
             * window while in the callback, re-run to avoid. The design
             * of the software should be to have only a small callback
             * handlers. There is a possibility that if the callbacks are
             * large, the firmware will not get any time to execute the main task.
             */
            interval = Cy_PdUtils_HwTimer_GetTickInterval (context->start_tick,
                    TIMER_CALL_MAP(Cy_PdUtils_HwTimer_GetCount)(context));
        }

    } while (period < (interval + CY_PDUTILS_TIMER_OVERRUN_THRESHOLD));

    /*
     * Load the timer with the required period. The value is intentionally
     * type-casted to 16-bit, therefore, it takes care of wrap-around.
     */
    context->tick_time = (uint16_t)period;
    TIMER_CALL_MAP(Cy_PdUtils_HwTimer_LoadPeriod) (context, (uint16_t)(period + context->start_tick));

#else /* (CY_PDUTILS_TIMER_TICKLESS_ENABLE == 0) */
    } while (0);
#endif /* (CY_PDUTILS_TIMER_TICKLESS_ENABLE) */

    /* Indicates that done with the interrupt handler. */
    context->is_callback = false;

    /* If no timer is running, turn OFF the hardware timer */
    if(context->total_active == 0U)
    {
        TIMER_CALL_MAP(Cy_PdUtils_HwTimer_Stop)(context);
    }
}
#endif /* (!PAG2S_SROM_CODE || (PAG2S_SROM_CODE && GENERATE_SROM_CODE)) */

/* [] END OF FILE */
