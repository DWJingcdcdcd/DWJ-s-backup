/**
 * @file com_delay.h
 * @brief com delay
 * @details delay function. 
 * @author Xiao Kun
 * @version V1.0
 * @date 2019.08.30
 */

#ifndef __COM_DELAY_H__
#define __COM_DELAY_H__

#include <stdint.h>

/**
 * @brief delay init, set the system frequency
 * @param  fre: system frequency
 * @return None.
 */
void com_delay_init(int fre);

/**
 * @brief delay ms
 * @return None.
 */
void com_delay_ms(uint16_t number_of_ms);

/**
 * @brief delay us
 * @return None.
 */
void com_delay_us(uint16_t number_of_us);

/**
 * @brief delay us
 * @param  number_of_tick: delay ticks
 * @return None.
 */
void com_delay_tick(uint16_t number_of_tick);

#endif  // __COM_DELAY_H__
