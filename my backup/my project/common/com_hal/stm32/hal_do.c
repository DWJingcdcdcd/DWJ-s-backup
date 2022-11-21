/**
 * @file hal_do.c
 * @brief digital output device in hal layer, just for STM32 MCU.
 * @details digital output device in hal layer, just for STM32 MCU.
 * @author Xiao Kun
 * @version V1.0
 * @date 2020.11.04
 */

#include "hal_do.h"
#include <stdlib.h>
#include "com_error.h"
#include "hal_base.h"

hal_do_dev_node_t *do_dev_node_p;
int hal_do_dev_node_count = 0;
hal_time_list_t hal_do_time_node;

extern hal_time_head_t hal_time_head;

int hal_do_timer_timeout(void);

int hal_do_init(int count)
{
    uint16_t i;
    
    do_dev_node_p = (hal_do_dev_node_t *)malloc(count * sizeof(hal_do_dev_node_t));   // 
    if(do_dev_node_p == NULL){
        return COM_ERROR_NO_MEM;
    }
    
    for (i = 0; i < count; i++)
    {
        do_dev_node_p[i].alloc_state = HAL_DO_STATE_FREE;
        do_dev_node_p[i].polar = HAL_DO_POLAR_POSITIVE;
    }
    hal_do_dev_node_count = count;
    
    hal_do_time_node.cb = &hal_do_timer_timeout;

    hal_time_list_t *p = hal_time_head.next_p;
    hal_time_head.next_p = &hal_do_time_node;
    hal_do_time_node.next_p = p;
    
    return COM_SUCCESS;
}

int hal_do_creat(hal_do_id_t *id, GPIO_TypeDef *port_p,uint16_t pin, com_do_polar_t polar)
{
    if(id == NULL){
        return COM_ERROR_INVALID_STATE;
    }
    
    int i;
    for(i=0;i<hal_do_dev_node_count;i++){
        if(do_dev_node_p[i].alloc_state == HAL_DO_STATE_FREE){
            do_dev_node_p[i].alloc_state = HAL_DO_ALLOCATED;
            do_dev_node_p[i].port_p = port_p;
            do_dev_node_p[i].pin = pin;
            do_dev_node_p[i].polar = polar;
            
            *id = i;
            return COM_SUCCESS;
        }
    }
    
    return COM_ERROR_NO_MEM;
}

int hal_do_output_high(hal_do_id_t id)
{
    if(id >= hal_do_dev_node_count){
        return COM_ERROR_INVALID_PARAM;
    }
    
    if(do_dev_node_p[id].alloc_state == HAL_DO_STATE_FREE){
        return COM_ERROR_INVALID_STATE;
    }
    
    do_dev_node_p[id].do_state = HAL_DO_STATE_SINGLE;
    HAL_GPIO_WritePin(do_dev_node_p[id].port_p, do_dev_node_p[id].pin, GPIO_PIN_SET);
    return COM_SUCCESS;
}

int hal_do_output_low(hal_do_id_t id)
{
    if(id >= hal_do_dev_node_count){
        return COM_ERROR_INVALID_PARAM;
    }
    
    if(do_dev_node_p[id].alloc_state == HAL_DO_STATE_FREE){
        return COM_ERROR_INVALID_STATE;
    }
    
    do_dev_node_p[id].do_state = HAL_DO_STATE_SINGLE;
    HAL_GPIO_WritePin(do_dev_node_p[id].port_p, do_dev_node_p[id].pin, GPIO_PIN_RESET);
    return COM_SUCCESS;
}

int hal_do_output_reverse(hal_do_id_t id)
{
    if(id >= hal_do_dev_node_count){
        return COM_ERROR_INVALID_PARAM;
    }
    
    if(do_dev_node_p[id].alloc_state == HAL_DO_STATE_FREE){
        return COM_ERROR_INVALID_STATE;
    }
    
    GPIO_PinState state;
    state = HAL_GPIO_ReadPin(do_dev_node_p[id].port_p, do_dev_node_p[id].pin);
    
    do_dev_node_p[id].do_state = HAL_DO_STATE_SINGLE;
    HAL_GPIO_WritePin(do_dev_node_p[id].port_p, do_dev_node_p[id].pin, (GPIO_PinState)!state);
    return COM_SUCCESS;
}

int hal_do_turn_on(hal_do_id_t id)
{
    if(id >= hal_do_dev_node_count){
        return COM_ERROR_INVALID_PARAM;
    }
    
    if(do_dev_node_p[id].alloc_state == HAL_DO_STATE_FREE){
        return COM_ERROR_INVALID_STATE;
    }
    
    do_dev_node_p[id].do_state = HAL_DO_STATE_SINGLE;
    switch(do_dev_node_p[id].polar){
        case HAL_DO_POLAR_POSITIVE:
            HAL_GPIO_WritePin(do_dev_node_p[id].port_p, do_dev_node_p[id].pin, GPIO_PIN_SET);
            break;
        case HAL_DO_POLAR_NEGATIVE:
            HAL_GPIO_WritePin(do_dev_node_p[id].port_p, do_dev_node_p[id].pin, GPIO_PIN_RESET);
            break;
        default:
            break;
    }
    
    return COM_SUCCESS;
}

int hal_do_turn_off(hal_do_id_t id)
{
    if(id >= hal_do_dev_node_count){
        return COM_ERROR_INVALID_PARAM;
    }
    
    if(do_dev_node_p[id].alloc_state == HAL_DO_STATE_FREE){
        return COM_ERROR_INVALID_STATE;
    }
    
    do_dev_node_p[id].do_state = HAL_DO_STATE_SINGLE;
    switch(do_dev_node_p[id].polar){
        case HAL_DO_POLAR_POSITIVE:
            HAL_GPIO_WritePin(do_dev_node_p[id].port_p, do_dev_node_p[id].pin, GPIO_PIN_RESET);
            break;
        case HAL_DO_POLAR_NEGATIVE:
            HAL_GPIO_WritePin(do_dev_node_p[id].port_p, do_dev_node_p[id].pin, GPIO_PIN_SET);
            break;
        default:
            break;
    }
    
    return COM_SUCCESS;
}

int hal_do_get_pin_level(hal_do_id_t id, uint8_t *level_p)
{
    if(id >= hal_do_dev_node_count){
        return COM_ERROR_INVALID_PARAM;
    }
    
    if(do_dev_node_p[id].alloc_state == HAL_DO_STATE_FREE){
        return COM_ERROR_INVALID_STATE;
    }
    
    GPIO_PinState state;
    state = HAL_GPIO_ReadPin(do_dev_node_p[id].port_p, do_dev_node_p[id].pin);
    *level_p = state;
    
    return COM_SUCCESS;
}

int hal_do_get_pin_polar_level(hal_do_id_t id, uint8_t *level_p)
{
    if(id >= hal_do_dev_node_count){
        return COM_ERROR_INVALID_PARAM;
    }
    
    if(do_dev_node_p[id].alloc_state == HAL_DO_STATE_FREE){
        return COM_ERROR_INVALID_STATE;
    }
    
    GPIO_PinState state;
    state = HAL_GPIO_ReadPin(do_dev_node_p[id].port_p, do_dev_node_p[id].pin);
    
    switch(do_dev_node_p[id].polar){
        case HAL_DO_POLAR_POSITIVE:
            *level_p = state;
            break;
        case HAL_DO_POLAR_NEGATIVE:
            *level_p = !state;
            break;
        default:
            break;
    }
    
    return COM_SUCCESS;
}

int hal_do_timer_timeout(void)
{
    size_t current_ms;
    current_ms = get_sys_time_ms();
    uint8_t state;
    for(int i=0;i<hal_do_dev_node_count;i++){
        if(do_dev_node_p[i].alloc_state == HAL_DO_STATE_FREE){
            break;
        }
        switch(do_dev_node_p[i].do_state){
            case HAL_DO_STATE_SINGLE:
                break;
            case HAL_DO_STATE_BLINK_ALWAYS:
                if(do_dev_node_p[i].desire_ms <= current_ms){
                    state = HAL_GPIO_ReadPin(do_dev_node_p[i].port_p, do_dev_node_p[i].pin);
                    if(state ^ do_dev_node_p[i].polar){
                        do_dev_node_p[i].desire_ms = current_ms + do_dev_node_p[i].blink_off_time;
                    } else {
                        do_dev_node_p[i].desire_ms = current_ms + do_dev_node_p[i].blink_on_time;
                    }
                    HAL_GPIO_WritePin(do_dev_node_p[i].port_p, do_dev_node_p[i].pin, (GPIO_PinState)!state);
                }
                break;
            case HAL_DO_STATE_BLINK_IN_TIMES:
                if(do_dev_node_p[i].desire_ms <= current_ms){
                    state = HAL_GPIO_ReadPin(do_dev_node_p[i].port_p, do_dev_node_p[i].pin);
                    if(state ^ do_dev_node_p[i].polar){
                        do_dev_node_p[i].desire_ms = current_ms + do_dev_node_p[i].blink_off_time;
                        do_dev_node_p[i].blink_index++;
                        if(do_dev_node_p[i].blink_index >= do_dev_node_p[i].blink_total_count){
                            do_dev_node_p[i].do_state = HAL_DO_STATE_SINGLE;
                        }
                    } else {
                        do_dev_node_p[i].desire_ms = current_ms + do_dev_node_p[i].blink_on_time;
                    }
                    HAL_GPIO_WritePin(do_dev_node_p[i].port_p, do_dev_node_p[i].pin, (GPIO_PinState)!state);
                }
                break;
            default:
                break;
        }
    }
    return COM_SUCCESS;
}

int hal_do_blink(hal_do_id_t id, uint16_t on_time, uint16_t off_time)
{
    if(id >= hal_do_dev_node_count){
        return COM_ERROR_INVALID_PARAM;
    }
    
    if(do_dev_node_p[id].alloc_state == HAL_DO_STATE_FREE){
        return COM_ERROR_INVALID_STATE;
    }
    
    do_dev_node_p[id].blink_on_time = on_time;
    do_dev_node_p[id].blink_off_time = off_time;
    do_dev_node_p[id].do_state = HAL_DO_STATE_BLINK_ALWAYS;
    do_dev_node_p[id].desire_ms = get_sys_time_ms() + on_time;
    
    return COM_SUCCESS;
}

int hal_do_blink_in_times(hal_do_id_t id, uint16_t on_time, uint16_t off_time, uint16_t blink_total_count)
{
    if(id >= hal_do_dev_node_count){
        return COM_ERROR_INVALID_PARAM;
    }
    
    if(do_dev_node_p[id].alloc_state == HAL_DO_STATE_FREE){
        return COM_ERROR_INVALID_STATE;
    }
    
    do_dev_node_p[id].blink_on_time = on_time;
    do_dev_node_p[id].blink_off_time = off_time;
    do_dev_node_p[id].do_state = HAL_DO_STATE_BLINK_IN_TIMES;
    do_dev_node_p[id].desire_ms = get_sys_time_ms() + on_time;
    do_dev_node_p[id].blink_total_count = blink_total_count;
    do_dev_node_p[id].blink_index = 0;
    
    return COM_SUCCESS;
}

