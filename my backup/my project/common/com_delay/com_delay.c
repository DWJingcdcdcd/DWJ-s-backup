/**
 * @file com_delay.c
 * @brief com delay
 * @details delay function. 
 * @author Xiao Kun
 * @version V1.0
 * @date 2019.08.30
 */

#include "com_delay.h"

#define ESTIMATE_TICK (10)			///< estimate value
/** @brief default value ms*/
uint32_t delay_tick_in_one_ms = (64000000/1000/ESTIMATE_TICK);
/** @brief default value us*/
uint32_t delay_tick_in_one_us = (64000000/1000000/ESTIMATE_TICK);

/**
 * @brief delay init, set the system frequency
 * @param fre: system frequency
 */
void com_delay_init(int fre)
{
    delay_tick_in_one_ms = fre/1000/ESTIMATE_TICK;          // estimate
    delay_tick_in_one_us = fre/1000000/ESTIMATE_TICK;       // estimate
}

/**
 * @brief delay ms
 * @param number_of_ms: delay ms
 */
void com_delay_ms(uint16_t number_of_ms)
{
    volatile uint32_t i;
		volatile uint32_t tick;
    while(number_of_ms != 0)
    {
        number_of_ms--;
        for(i = delay_tick_in_one_ms; i > 0; i--){
            tick++;
        }
    }
}

/**
 * @brief delay us
 * @param number_of_us: delay us
 */
void com_delay_us(uint16_t number_of_us)
{
    volatile uint16_t i;
		volatile uint32_t tick;
    while(number_of_us != 0)
    {
        number_of_us--;
        for(i = delay_tick_in_one_us; i > 0; i--){
            tick++;
        }
    }
}

/**
 * @brief delay us
 * @param number_of_tick: delay ticks
 */
void com_delay_tick(uint16_t number_of_tick)
{
		volatile uint32_t tick;
    while((number_of_tick--) != 0)
    {
        tick++;
    }
}
