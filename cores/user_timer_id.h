#ifndef _USER_TIMER_ID_H_
#define _USER_TIMER_ID_H_

#include <stdint.h>

#define SNK_CAP_CHANGE_RETRY_TIMER_PERIOD       (5u) 
#define SRC_CAP_CHANGE_RETRY_TIMER_PERIOD       (5u) 
#define GET_PARTNER_PORT_SRC_PDO_TIMER_PERIOD   (5u)
#define GET_PARTNER_PORT_SNK_PDO_TIMER_PERIOD   (5u)
#define POWER_ROLE_SWAP_RETRY_TIMER_PERIOD      (5u)
#define EPR_ENTER_MODE_RETRY_TIMER_PERIOD       (5u)
#define EPR_EXIT_MODE_RETRY_TIMER_PERIOD        (5u)

#define GET_USER_TIMER_ID(context, id)                                 \
    (uint16_t)(((context)->port != 0U) ? (((((uint16_t)id) & 0x00FFU) + (uint16_t)CY_PDUTILS_TIMER_USER_START_ID) + 255) : (uint16_t)(id))

typedef enum {
    USER_TIMERS_START_ID = CY_PDUTILS_TIMER_USER_START_ID,
    SRC_CAP_CHANGE_RETRY_TIMER,
    SNK_CAP_CHANGE_RETRY_TIMER,
    GET_PARTNER_PORT_SRC_PDO_TIMER,
    GET_PARTNER_PORT_SNK_PDO_TIMER,
    POWER_ROLE_SWAP_RETRY_TIMER,
    EPR_ENTER_MODE_RETRY_TIMER,
    EPR_EXIT_MODE_RETRY_TIMER,
} timer_id_t;

#endif /* _TIMER_ID_H_ */
