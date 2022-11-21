/**
 * @file hal_base.c
 * @brief base function file in hal layer, just for STM32 MCU.
 * @details base function file in hal layer, just for STM32 MCU.
 * @author Xiao Kun
 * @version V1.0
 * @date 2020.11.04
 */

#include "hal_base.h"
#include <stdlib.h>
#include "com_error.h"

hal_time_head_t hal_time_head;

int hal_base_init()
{
    hal_base_timer_init();
    return COM_SUCCESS;
}

/*****************************************************************/
// hal base timer
int hal_base_timer_init()
{
    hal_time_head.next_p = NULL;
    return COM_SUCCESS;
}

void hal_base_timer_timeout(void)
{
    hal_time_list_t *p = hal_time_head.next_p;
    while(p != NULL){
        p->cb();
        p = p->next_p;
    }
}











