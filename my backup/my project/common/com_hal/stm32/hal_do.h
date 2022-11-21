/**
 * @file hal_do.h
 * @brief digital output device in hal layer, just for STM32 MCU.
 * @details digital output device in hal layer, just for STM32 MCU.
 * @author Xiao Kun
 * @version V1.0
 * @date 2020.11.04
 */

#ifndef __HAL_DO_H__
#define __HAL_DO_H__

#include <stdint.h>
#include "gpio.h"

/** @brief digital output device state. */
typedef enum {
	HAL_DO_STATE_SINGLE,                ///< single state
    HAL_DO_STATE_BLINK_ALWAYS,          ///< blink always state
    HAL_DO_STATE_BLINK_IN_TIMES,        ///< blink in times state
} hal_do_dev_state;

/** @brief the allocated state of do device */
typedef enum
{
    HAL_DO_STATE_FREE   = 0,            ///< free state
    HAL_DO_ALLOCATED    = 1,            ///< allocated state
} com_do_alloc_state_t;

/** @brief the allocated state of do device */
typedef enum
{
    HAL_DO_POLAR_POSITIVE   = 0,            ///< positive, active in high level
    HAL_DO_POLAR_NEGATIVE   = 1,            ///< negative, active in low level
} com_do_polar_t;

/** @brief com timer id */
typedef uint16_t hal_do_id_t;

/** @brief digital output device struct. */
typedef struct {
    com_do_alloc_state_t alloc_state;   ///< alloc state
    hal_do_dev_state do_state;          ///< state of device
    uint16_t blink_on_time;             ///< blink_on_time, unit ms
    uint16_t blink_off_time;            ///< blink_off_time, unit ms
    uint16_t blink_total_count;         ///< blink total count
    uint16_t blink_index;               ///< blink index
    size_t desire_ms;                   ///< blink desire ms
    GPIO_TypeDef *port_p;               ///< do port
    com_do_polar_t polar;               ///< polarity
	uint16_t pin;                       ///< do pin
}hal_do_dev_node_t;

/**
 * @brief digital output device init
 * @param  count: the number of digital output device
 */
int hal_do_init(int count);

/**
 * @brief creat a digital output device
 * @param  id: the if of digital output device
 * @param  port_p: gpio port
 * @param  pin: gpio pin
 * @param  polar: the polar of DO device
 * @return error code.
 */
int hal_do_creat(hal_do_id_t *id, GPIO_TypeDef *port_p,uint16_t pin, com_do_polar_t polar);

/**
 * @brief set high level for DO device
 * @param  id: the if of digital output device
 * @return error code.
 */
int hal_do_output_high(hal_do_id_t id);

/**
 * @brief set low level for DO device
 * @param  id: the if of digital output device
 * @return error code.
 */
int hal_do_output_low(hal_do_id_t id);

/**
 * @brief reverse level for DO device
 * @param  id: the if of digital output device
 * @return error code.
 */
int hal_do_output_reverse(hal_do_id_t id);

/**
 * @brief turn on for DO device, this function will consider device polar
 * @param  id: the if of digital output device
 * @return error code.
 */
int hal_do_turn_on(hal_do_id_t id);

/**
 * @brief turn off for DO device, this function will consider device polar
 * @param  id: the if of digital output device
 * @return error code.
 */
int hal_do_turn_off(hal_do_id_t id);

/**
 * @brief get level from a DO device
 * @param  id: the if of digital output device
 * @param  level: the level of digital output device
 * @return error code.
 */
int hal_do_get_pin_level(hal_do_id_t id, uint8_t *level_p);

/**
 * @brief get level from a DO device, this function will consider device polar
 * @param  id: the if of digital output device
 * @param  level: the level of digital output device
 * @return error code.
 */
int hal_do_get_pin_polar_level(hal_do_id_t id, uint8_t *level_p);

/**
 * @brief blink a device
 * @param  id: the if of digital output device
 * @param  on_time: the turn on time in one period
 * @param  off_time: the turn off time in one period
 * @return error code.
 */
int hal_do_blink(hal_do_id_t id, uint16_t on_time, uint16_t off_time);

/**
 * @brief blink a device several times
 * @param  id: the if of digital output device
 * @param  on_time: the turn on time in one period
 * @param  off_time: the turn off time in one period
 * @return error code.
 */
int hal_do_blink_in_times(hal_do_id_t id, uint16_t on_time, uint16_t off_time, uint16_t blink_total_count);

#endif  // __HAL_DO_H__
