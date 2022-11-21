/**
 * @file hal_wdt.h
 * @brief watch dog timer in hal layer, just for STM32 MCU.
 * @details watch dog timer in hal layer, just for STM32 MCU.
 * @author Xiao Kun
 * @version V1.0
 * @date 2020.11.09
 */

#ifndef __HAL_WDT_H__
#define __HAL_WDT_H__

#include <stdint.h>
#include "gpio.h"

/** @brief digital output device struct. */
typedef struct {
    size_t desire_ms;                   ///< feed desire ms
    IWDG_HandleTypeDef *wdg_p;          ///< watch dog timer
    uint16_t feed_time;                 ///< blink_on_time, default 10ms, unit ms
    uint8_t is_enable;                  ///< flag, is wdt enable
}hal_wdt_dev_node_t;

/**
 * @brief watch dog timer init
 * @return error code.
 */
int hal_wdt_init(void);

/**
 * @brief creat a watch dog timer
 * @param  wdg: pointer to watch dog timer
 * @param  feed_time: feed_time
 * @return error code.
 */
int hal_wdt_creat(IWDG_HandleTypeDef *wdg_p, uint16_t feed_time);

/**
 * @brief enable watch dog timer
 * @return error code.
 */
int hal_wdt_enable(void);

/**
 * @brief disable watch dog timer
 * @return error code.
 */
int hal_wdt_disable(void);

/**
 * @brief feed watch dog timer
 * @return error code.
 */
int hal_wdt_feed(void);

/*
here is a example

hal_wdt_init();
hal_wdt_creat(&hiwdg,20);
hal_wdt_enable();

*/

#endif  // __HAL_WDT_H__
