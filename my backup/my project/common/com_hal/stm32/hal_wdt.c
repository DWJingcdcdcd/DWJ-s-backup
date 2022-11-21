/**
 * @file hal_wdt.c
 * @brief watch dog timer in hal layer, just for STM32 MCU.
 * @details watch dog timer in hal layer, just for STM32 MCU.
 * @author Xiao Kun
 * @version V1.0
 * @date 2020.11.09
 */

#include "hal_wdt.h"
#include <stdlib.h>
#include "com_error.h"
#include "hal_base.h"

hal_wdt_dev_node_t wdt_dev_node;
hal_time_list_t hal_wdt_time_node;

int hal_wdt_timer_timeout(void);
extern hal_time_head_t hal_time_head;

int hal_wdt_init(void)
{
    hal_wdt_time_node.cb = &hal_wdt_timer_timeout;

    hal_time_list_t *p = hal_time_head.next_p;
    hal_time_head.next_p = &hal_wdt_time_node;
    hal_wdt_time_node.next_p = p;
    
    return COM_SUCCESS;
}

int hal_wdt_creat(IWDG_HandleTypeDef *wdg_p, uint16_t feed_time)
{
    if(wdg_p == NULL){
        return COM_ERROR_NULL;
    }
    
    if(feed_time == 0){
        return COM_ERROR_INVALID_PARAM;
    }
    
    wdt_dev_node.wdg_p = wdg_p;
    wdt_dev_node.feed_time = 0;
    wdt_dev_node.is_enable = 0;
    wdt_dev_node.desire_ms = 0;
    
    return COM_SUCCESS;
}

int hal_wdt_enable(void)
{
    size_t current_ms;
    current_ms = get_sys_time_ms();
    
    wdt_dev_node.is_enable = 1;
    wdt_dev_node.desire_ms = current_ms + wdt_dev_node.feed_time;
    
    return COM_SUCCESS;
}

int hal_wdt_disable(void)
{
    wdt_dev_node.is_enable = 0;
    
    return COM_SUCCESS;
}

int hal_wdt_feed(void)
{
    HAL_IWDG_Refresh(wdt_dev_node.wdg_p);
    
    return COM_SUCCESS;
}

int hal_wdt_timer_timeout(void)
{
    size_t current_ms;
    current_ms = get_sys_time_ms();
    
    if(wdt_dev_node.is_enable == 0){
        return COM_SUCCESS;
    }
    
    if(wdt_dev_node.desire_ms <= current_ms){
        wdt_dev_node.desire_ms = current_ms + wdt_dev_node.feed_time;
        hal_wdt_feed();
    }
    
    return COM_SUCCESS;
}
