/**
 * @file hal_base.h
 * @brief base function file in hal layer, just for STM32 MCU.
 * @details base function file in hal layer, just for STM32 MCU.
 * @author Xiao Kun
 * @version V1.0
 * @date 2020.11.04
 */


#ifndef __HAL_BASE_H__
#define __HAL_BASE_H__

#include <stdint.h>
#include <stdlib.h>

/** @brief timer call back function */
typedef int (*hal_timer_timeout_cb)(void);

/** @brief massage listener define */
typedef struct hal_time_list_t{
    hal_timer_timeout_cb cb;            ///< call back function
    struct hal_time_list_t *next_p;     ///< next node
}hal_time_list_t;

typedef struct{
    struct hal_time_list_t *next_p;     ///< next node
}hal_time_head_t;

int hal_base_init(void);

/*****************************************************************/
// hal base timer
int hal_base_timer_init(void);

int hal_base_time_add_fun(hal_time_list_t *node_p, hal_timer_timeout_cb cb_p);

int hal_base_time_remove_fun(hal_time_list_t *node_p);

void hal_base_timer_timeout(void);

extern size_t get_sys_time_ms(void);

#endif  // __HAL_BASE_H__
