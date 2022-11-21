/**
 * @file hal_gpio.c
 * @brief gpio device in hal layer, just for STM32 MCU.
 * @details gpio device device in hal layer, base read and write function.
 * @author Xiao Kun
 * @version V1.0
 * @date 2020.12.14
 */

#ifndef __HAL_GPIO_H__
#define __HAL_GPIO_H__

#include <stdint.h>
#include "gpio.h"

/** @brief the allocated state of do device */
typedef enum
{
    HAL_GPIO_STATE_FREE   = 0,            ///< free state
    HAL_GPIO_ALLOCATED    = 1,            ///< allocated state
} com_gpio_alloc_state_t;

/** @brief the input style */
typedef enum
{
    HAL_GPIO_PIN_NOPULL   = 0,            ///< Pin pull-up resistor disabled.
    HAL_GPIO_PIN_PULLDOWN,                ///< Pin pull-down resistor enabled
    HAL_GPIO_PIN_PULLUP,                  ///< Pin pull-up resistor enabled.
} com_gpio_input_style_t;

/** @brief the output style */
typedef enum
{
    TODO,
} com_gpio_output_style_t;

/** @brief com timer id */
typedef uint16_t hal_gpio_id_t;

/** @brief digital output device struct. */
typedef struct {
    com_gpio_alloc_state_t alloc_state;   ///< alloc state
    GPIO_TypeDef *port_p;               ///< do port
	uint16_t pin;                       ///< do pin
}hal_gpio_dev_node_t;

/**
 * @brief digital output device init
 * @param  count: the number of digital output device
 */
int hal_gpio_init(int count);

/**
 * @brief creat a digital output device
 * @param  id: the if of digital output device
 * @param  port_p: gpio port
 * @param  pin: gpio pin
 * @param  polar: the polar of DO device
 * @return error code.
 */
int hal_gpio_creat(hal_gpio_id_t *id, GPIO_TypeDef *port_p,uint16_t pin);

/**
 * @brief set high level for DO device
 * @param  id: the if of digital output device
 * @return error code.
 */
int hal_gpio_output_high(hal_gpio_id_t id);

/**
 * @brief set low level for DO device
 * @param  id: the if of digital output device
 * @return error code.
 */
int hal_gpio_output_low(hal_gpio_id_t id);

/**
 * @brief reverse level for DO device
 * @param  id: the if of digital output device
 * @return error code.
 */
int hal_gpio_output_reverse(hal_gpio_id_t id);

/**
 * @brief get level from a DO device
 * @param  id: the if of digital output device
 * @param  level: the level of digital output device
 * @return error code.
 */
int hal_gpio_get_pin_level(hal_gpio_id_t id, uint8_t *level_p);

int set_gpio_as_output(hal_gpio_id_t id,com_gpio_output_style_t style);

int set_gpio_as_input(hal_gpio_id_t id,com_gpio_input_style_t style);

#endif  // __HAL_GPIO_H__
