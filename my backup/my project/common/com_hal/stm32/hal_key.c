/**
 * @file hal_key.c
 * @brief key device in hal layer, just for STM32 MCU.
 * @details key device in hal layer, just for STM32 MCU.
 * @author Xiao Kun
 * @version V1.0
 * @date 2020.11.04
 */

#include "hal_key.h"
#include <stdlib.h>
#include "com_error.h"
#include "hal_base.h"

hal_key_dev_node_t *key_dev_node_p;
int hal_key_dev_node_count = 0;
hal_time_list_t hal_key_time_node;

int hal_key_timer_timeout(void);
extern hal_time_head_t hal_time_head;
    
int hal_key_init(int count)
{
    uint16_t i;
    
    key_dev_node_p = (hal_key_dev_node_t *)malloc(count * sizeof(hal_key_dev_node_t));   // 
    if(key_dev_node_p == NULL){
        return COM_ERROR_NO_MEM;
    }
    
    for (i = 0; i < count; i++)
    {
        key_dev_node_p[i].alloc_state = HAL_KEY_STATE_FREE;
        key_dev_node_p[i].polar = HAL_KEY_POLAR_POSITIVE;
        key_dev_node_p[i].msg_cb = NULL;
        key_dev_node_p[i].delay_time = 20;
        key_dev_node_p[i].long_push_count = 3;
        key_dev_node_p[i].count = 0;
        key_dev_node_p[i].key_state = HAL_KEY_STATE_INVALID;
        key_dev_node_p[i].rep_time = 330;
        key_dev_node_p[i].last_key_state = HAL_KEY_STATE_UP;
    }
    hal_key_dev_node_count = count;
    
    hal_key_time_node.cb = &hal_key_timer_timeout;

    hal_time_list_t *p = hal_time_head.next_p;
    hal_time_head.next_p = &hal_key_time_node;
    hal_key_time_node.next_p = p;
    
    return COM_SUCCESS;
}

int hal_key_creat(hal_key_id_t *id, GPIO_TypeDef *port_p,uint16_t pin, com_key_polar_t polar, hal_key_msg_cb msg_cb)
{
    if(id == NULL){
        return COM_ERROR_INVALID_STATE;
    }
    
    int i;
    for(i=0;i<hal_key_dev_node_count;i++){
        if(key_dev_node_p[i].alloc_state == HAL_KEY_STATE_FREE){
            key_dev_node_p[i].alloc_state = HAL_KEY_ALLOCATED;
            key_dev_node_p[i].port_p = port_p;
            key_dev_node_p[i].pin = pin;
            key_dev_node_p[i].polar = polar;
            key_dev_node_p[i].msg_cb = msg_cb;
            
            *id = i;
            return COM_SUCCESS;
        }
    }
    
    return COM_ERROR_NO_MEM;
}

int hal_key_cfg(hal_key_id_t id, uint16_t delay_time, uint16_t long_push_ms, uint16_t rep_time)
{
    if(id >= hal_key_dev_node_count){
        return COM_ERROR_INVALID_STATE;
    }
    key_dev_node_p[id].delay_time = delay_time;
    key_dev_node_p[id].long_push_count = long_push_ms/rep_time;
    key_dev_node_p[id].rep_time = rep_time;
    
    return COM_SUCCESS;
}

int hal_key_get_state(hal_key_id_t id,hal_key_state *state_p)
{
    if((id >= hal_key_dev_node_count) || (state_p == NULL)){
        return COM_ERROR_INVALID_STATE;
    }
    
    uint8_t key_state;
    key_state = HAL_GPIO_ReadPin(key_dev_node_p[id].port_p, key_dev_node_p[id].pin);
    if(key_state ^ key_dev_node_p[id].polar){
        *state_p = HAL_KEY_STATE_UP;
    } else {
        *state_p = HAL_KEY_STATE_PUSH;
    }
    
    return COM_SUCCESS;
}

int hal_key_poll(void)
{
    hal_key_state key_state;
    
    for(int i=0;i<hal_key_dev_node_count;i++){
        if(key_dev_node_p[i].alloc_state == HAL_KEY_STATE_FREE){
            break;
        }
        hal_key_get_state(i,&key_state);
        if(key_state != key_dev_node_p[i].last_key_state){
            key_dev_node_p[i].last_key_state = key_state;
            if(key_state == HAL_KEY_STATE_PUSH){
                hal_key_rev_irq(key_dev_node_p[i].port_p,key_dev_node_p[i].pin);
            }
        }
    }
    
    return COM_SUCCESS;
}

int hal_key_rev_irq(GPIO_TypeDef *port_p,uint16_t pin)
{
    for(int i=0;i<hal_key_dev_node_count;i++){
        if((key_dev_node_p[i].port_p == port_p) && (key_dev_node_p[i].pin == pin)){
            size_t current_ms;
            current_ms = get_sys_time_ms();
            key_dev_node_p[i].desire_ms = current_ms + key_dev_node_p[i].delay_time;
            key_dev_node_p[i].count = 0;
            key_dev_node_p[i].key_state = HAL_KEY_STATE_NEED_CHECK;
            return COM_SUCCESS;
        }
    }
    return COM_ERROR_INVALID_STATE;
}

int hal_key_timer_timeout(void)
{
    hal_key_state key_state;
    size_t current_ms;
    current_ms = get_sys_time_ms();
    
    for(int i=0;i<hal_key_dev_node_count;i++){
        if(key_dev_node_p[i].alloc_state == HAL_KEY_STATE_FREE){
            break;
        }
        if(key_dev_node_p[i].key_state != HAL_KEY_STATE_INVALID){
            if(key_dev_node_p[i].desire_ms <= current_ms){
                hal_key_get_state(i,&key_state);
                switch(key_state){
                    case HAL_KEY_STATE_PUSH:
                        
                        key_dev_node_p[i].count++;
                        key_dev_node_p[i].desire_ms = current_ms + key_dev_node_p[i].rep_time;
                        if(key_dev_node_p[i].count == 1){
                            key_dev_node_p[i].msg_cb(i,HAL_KEY_MSG_PUSH);
                            break;
                        }
                        if(key_dev_node_p[i].count == key_dev_node_p[i].long_push_count){
                            key_dev_node_p[i].msg_cb(i,HAL_KEY_MSG_LONG_PUSH);
                            key_dev_node_p[i].msg_cb(i,HAL_KEY_MSG_REP_PUSH);
                            //key_dev_node_p[i].count = 0;
                        } else {
                            key_dev_node_p[i].msg_cb(i,HAL_KEY_MSG_REP_PUSH);
                        }
                        
                        break;
                    case HAL_KEY_STATE_UP:
                        //if(key_dev_node_p[i].count != 0){
                            key_dev_node_p[i].msg_cb(i,HAL_KEY_MSG_UP);
                            key_dev_node_p[i].key_state = HAL_KEY_STATE_INVALID;
                        //}
                        break;
                    case HAL_KEY_STATE_INVALID:
                        break;
                    case HAL_KEY_STATE_NEED_CHECK:
                        break;
                }
            }
        }
    }
    return COM_SUCCESS;
}


