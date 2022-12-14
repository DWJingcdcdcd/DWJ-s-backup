/**
 * @file com_mds560r.h
 * @brief mds560r driver
 * @details mds560r driver
 * @author Ding Wenju
 * @version V1.0
 * @date 2022.12.07
 */
 
#ifndef __COM_MDS560R_H__
#define __COM_MDS560R_H__

#include "stdint.h"
#include "hal_uart.h"
#include "hal_do.h"


/** @brief the allocated state of mds560r device */
typedef enum
{
    HAL_mds560r_STATE_FREE   = 0,            ///< free state
    HAL_mds560r_ALLOCATED    = 1,            ///< allocated state
} com_mds560r_alloc_state_t;

/** @brief mds560r device struct. */
typedef struct {
    com_mds560r_alloc_state_t alloc_state;
    hal_uart_id_t uart_dev;
    hal_do_id_t dir;
}com_mds560r_node_t;

/**
 * @brief initial the mds560r devices
 * @param  mds560r_count: the count of mds560r devices
 * @return error code.
 */
int com_mds560r_init(uint8_t mds560r_count);

/**
 * @brief creat a mds560r device
 * @param  id: mds560r device id
 * @param  uart_dev: uart device id
 * @param  dir: the direction of communication
 * @return error code.
 */
int com_mds560r_creat(uint16_t *id,hal_uart_id_t uart_dev,hal_do_id_t dir);


/**
 * @brief read data from mds560r 
 * @param  id: mds560r device id
 * @return error code.
 */
int com_mds560r_read_data(uint16_t id);




#endif //__COM_MDS560R_H__

