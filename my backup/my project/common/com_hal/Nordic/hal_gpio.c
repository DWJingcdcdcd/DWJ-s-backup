/**
 * @file hal_gpio.c
 * @brief gpio device in hal layer, just for STM32 MCU.
 * @details gpio device device in hal layer, base read and write function.
 * @author Xiao Kun
 * @version V1.0
 * @date 2020.11.04
 */

#include "hal_gpio.h"
#include <stdlib.h>
#include "com_error.h"
#include "nrf_gpio.h"

static hal_gpio_dev_node_t *gpio_dev_node_p;
static int hal_gpio_dev_node_count = 0;


int hal_gpio_init(int count)
{
    uint16_t i;
    
    gpio_dev_node_p = (hal_gpio_dev_node_t *)malloc(count * sizeof(hal_gpio_dev_node_t));   // 
    if(gpio_dev_node_p == NULL){
        return COM_ERROR_NO_MEM;
    }
    
    for (i = 0; i < count; i++)
    {
        gpio_dev_node_p[i].alloc_state = HAL_GPIO_STATE_FREE;
    }
    hal_gpio_dev_node_count = count;
    
    return COM_SUCCESS;
}

int hal_gpio_creat(hal_gpio_id_t *id, GPIO_TypeDef *port_p,uint16_t pin)
{
    if(id == NULL){
        return COM_ERROR_INVALID_STATE;
    }
    
    int i;
    for(i = 0; i < hal_gpio_dev_node_count; i++){
        if(gpio_dev_node_p[i].alloc_state == HAL_GPIO_STATE_FREE){
            gpio_dev_node_p[i].alloc_state = HAL_GPIO_ALLOCATED;
            gpio_dev_node_p[i].port_p = port_p;
            gpio_dev_node_p[i].pin = pin;
            
            *id = i+1;
            return COM_SUCCESS;
        }
    }
    
    return COM_ERROR_NO_MEM;
}

int hal_gpio_output_high(hal_gpio_id_t id)
{
    if(id > hal_gpio_dev_node_count){
        return COM_ERROR_INVALID_PARAM;
    }
    
    if(gpio_dev_node_p[id - 1].alloc_state == HAL_GPIO_STATE_FREE){
        return COM_ERROR_INVALID_STATE;
    }
    
    nrf_gpio_pin_set(gpio_dev_node_p[id - 1].pin);
    return COM_SUCCESS;
}

int hal_gpio_output_low(hal_gpio_id_t id)
{
    if(id > hal_gpio_dev_node_count){
        return COM_ERROR_INVALID_PARAM;
    }
    
    if(gpio_dev_node_p[id - 1].alloc_state == HAL_GPIO_STATE_FREE){
        return COM_ERROR_INVALID_STATE;
    }
    
    nrf_gpio_pin_clear(gpio_dev_node_p[id - 1].pin);
    return COM_SUCCESS;
}

int hal_gpio_output_reverse(hal_gpio_id_t id)
{
    if(id > hal_gpio_dev_node_count){
        return COM_ERROR_INVALID_PARAM;
    }
    
    if(gpio_dev_node_p[id - 1].alloc_state == HAL_GPIO_STATE_FREE){
        return COM_ERROR_INVALID_STATE;
    }
    
    nrf_gpio_pin_toggle(gpio_dev_node_p[id - 1].pin);
    return COM_SUCCESS;
}

int hal_gpio_get_pin_level(hal_gpio_id_t id, uint8_t *level_p)
{
    if(id > hal_gpio_dev_node_count){
        return COM_ERROR_INVALID_PARAM;
    }
    
    if(gpio_dev_node_p[id - 1].alloc_state == HAL_GPIO_STATE_FREE){
        return COM_ERROR_INVALID_STATE;
    }
    
    *level_p = nrf_gpio_pin_read(gpio_dev_node_p[id - 1].pin);
    
    return COM_SUCCESS;
}

int set_gpio_as_output(hal_gpio_id_t id,com_gpio_output_style_t style)
{
    if(id > hal_gpio_dev_node_count){
        return COM_ERROR_INVALID_PARAM;
    }
    
    if(gpio_dev_node_p[id - 1].alloc_state == HAL_GPIO_STATE_FREE){
        return COM_ERROR_INVALID_STATE;
    }
    
    nrf_gpio_cfg_output(gpio_dev_node_p[id - 1].pin);
    
    return COM_SUCCESS;
}

int set_gpio_as_input(hal_gpio_id_t id,com_gpio_input_style_t style)
{
    if(id > hal_gpio_dev_node_count){
        return COM_ERROR_INVALID_PARAM;
    }
    
    if(gpio_dev_node_p[id - 1].alloc_state == HAL_GPIO_STATE_FREE){
        return COM_ERROR_INVALID_STATE;
    }
    
    switch(style){
        case HAL_GPIO_PIN_NOPULL:
            nrf_gpio_cfg_input(gpio_dev_node_p[id - 1].pin,NRF_GPIO_PIN_NOPULL);
            break;
        case HAL_GPIO_PIN_PULLDOWN:
            nrf_gpio_cfg_input(gpio_dev_node_p[id - 1].pin,NRF_GPIO_PIN_PULLDOWN);
            break;
        case HAL_GPIO_PIN_PULLUP:
            nrf_gpio_cfg_input(gpio_dev_node_p[id - 1].pin,NRF_GPIO_PIN_PULLUP);
            break;
        default:
            break;
    }
    
    
    return COM_SUCCESS;
}
