/**
 * @file hal_key.h
 * @brief key device in hal layer, just for STM32 MCU.
 * @details key device in hal layer, just for STM32 MCU.
 * @author Xiao Kun
 * @version V1.0
 * @date 2020.11.04
 */

#ifndef __HAL_KEY_H__
#define __HAL_KEY_H__

#include <stdint.h>
#include "gpio.h"

/** @brief key state. */
typedef enum{
    HAL_KEY_STATE_PUSH = 0,     ///< key state push down
    HAL_KEY_STATE_UP,           ///< key state pop up
    HAL_KEY_STATE_INVALID,      ///< key state invalid
    HAL_KEY_STATE_NEED_CHECK,   ///< key state need check. when irq, the key state is confused, set to this state
} hal_key_state;

/** @brief key message type. */
typedef enum {
	HAL_KEY_MSG_PUSH = 0,           ///< key message push down
    HAL_KEY_MSG_LONG_PUSH,          ///< key message press down long time
    HAL_KEY_MSG_REP_PUSH,           ///< key message press down repeat
	HAL_KEY_MSG_UP,                 ///< key message pop up
	HAL_KEY_MSG_INVALID,            ///< key message invalid
} hal_key_msg_type;

/** @brief the allocated state of key device */
typedef enum
{
    HAL_KEY_STATE_FREE   = 0,            ///< free state
    HAL_KEY_ALLOCATED    = 1,            ///< allocated state
} com_key_alloc_state_t;

/** @brief the allocated state of do device */
typedef enum
{
    HAL_KEY_POLAR_POSITIVE   = 0,            ///< positive, when push down the voltage is low
    HAL_KEY_POLAR_NEGATIVE   = 1,            ///< negative, when push down the voltage is high
} com_key_polar_t;

/** @brief hal key id */
typedef uint16_t hal_key_id_t;

/** @brief listener call back function */
typedef void (*hal_key_msg_cb)(hal_key_id_t key_id, hal_key_msg_type msg_type);

/** @brief digital output device struct. */
typedef struct {
    com_key_alloc_state_t alloc_state;  ///< alloc state
    hal_key_msg_cb msg_cb;              ///< key message call back function
    size_t desire_ms;                   ///< blink desire ms
    com_key_polar_t polar;              ///< polarity
    GPIO_TypeDef *port_p;               ///< do port
    hal_key_state key_state;            ///< key state
    hal_key_state last_key_state;       ///< last key state
	uint16_t pin;                       ///< do pin
    uint16_t delay_time;                ///< blink_on_time, default 20ms, unit ms
    uint16_t count;                     ///< the count of delay time, use this count to calculate long push time. everv time count++, trans HAL_KEY_STATE_REP_PUSH message
    uint16_t long_push_count;           ///< if count >= long_push_count, trans HAL_KEY_STATE_LONG_PUSH message, default 100(1s) 
    uint16_t rep_time;                  ///< repeat time, default 330ms, unit ms
}hal_key_dev_node_t;

/**
 * @brief key device init
 * @param  count: the count of key device
 */
int hal_key_init(int count);

/**
 * @brief creat a key device
 * @param  id: the id of device
 * @param  port_p: gpio port
 * @param  pin: gpio pin
 * @param  polar: the polar of device
 * @param  msg_cb: the call back function of device message
 * @return error code.
 */
int hal_key_creat(hal_key_id_t *id, GPIO_TypeDef *port_p,uint16_t pin, com_key_polar_t polar, hal_key_msg_cb msg_cb);

/**
 * @brief config a key device
 * @param  id: the id of device
 * @param  delay_time: the delay time for elimination Buffeting
 * @param  long_push_count: if user push button exceed this count, trans HAL_KEY_STATE_LONG_PUSH message
 * @return error code.
 */
int hal_key_cfg(hal_key_id_t id, uint16_t delay_time, uint16_t long_push_ms, uint16_t rep_time);

/**
 * @brief get state from a key device
 * @param  id: the id of device
 * @param  state: the key state
 * @return error code.
 */
int hal_key_get_state(hal_key_id_t id,hal_key_state *state_p);

/**
 * @brief use polling mod get key state, this function need call by app layer
 * @return error code.
 */
int hal_key_poll(void);

/**
 * @brief if rev a interrupt, call this function
 * @param  port_p: gpio port
 * @param  pin: gpio pin
 * @return error code.
 */
int hal_key_rev_irq(GPIO_TypeDef *port_p,uint16_t pin);

/*
here is a example:

in init fun
{
    hal_key_init(5);
    hal_key_creat(&key0,KEY0_GPIO_Port,KEY0_Pin,HAL_KEY_POLAR_POSITIVE,&key_cb);
    hal_key_cfg(key0,20,1000,300);
}

void key_cb(hal_key_id_t key_id, hal_key_msg_type msg_type)
{
    if(key_id == key0){
        switch(msg_type){
            case HAL_KEY_MSG_PUSH:
                SEGGER_RTT_printf(0, "key0 push!\r\n");
                break;
            case HAL_KEY_MSG_LONG_PUSH:
                SEGGER_RTT_printf(0, "key0 long push!\r\n");
                break;
            case HAL_KEY_MSG_REP_PUSH:
                SEGGER_RTT_printf(0, "key0 rep push!\r\n");
                break;
            case HAL_KEY_MSG_UP:
                SEGGER_RTT_printf(0, "key0 up!\r\n");
                break;
            default:
                break;
        }
    }
    
    if(key_id == key1){
        switch(msg_type){
            case HAL_KEY_MSG_PUSH:
                SEGGER_RTT_printf(0, "key1 push!\r\n");
                break;
            case HAL_KEY_MSG_LONG_PUSH:
                SEGGER_RTT_printf(0, "key1 long push!\r\n");
                break;
            case HAL_KEY_MSG_REP_PUSH:
                SEGGER_RTT_printf(0, "key1 rep push!\r\n");
                break;
            case HAL_KEY_MSG_UP:
                SEGGER_RTT_printf(0, "key1 up!\r\n");
                break;
            default:
                break;
        }
    }
}
*/

#endif  // __HAL_KEY_H__
