/**
 * @file hal_uart.c
 * @brief uart in hal layer, just for STM32 MCU.
 * @details uart in hal layer, just for STM32 MCU.
 * @author Xiao Kun
 * @version V1.0
 * @date 2020.11.09
 */

#ifndef __HAL_UART_H__
#define __HAL_UART_H__

#include <stdint.h>
#include "gpio.h"

/** @brief the allocated state of uart device */
typedef enum
{
    HAL_UART_STATE_FREE   = 0,            ///< free state
    HAL_UART_ALLOCATED    = 1,            ///< allocated state
} com_uart_alloc_state_t;

/** @brief uart message type. */
typedef enum {
	HAL_UART_MSG_TRANS_START = 0,       ///< uart message trans start
    HAL_UART_MSG_TRANS_DONE,            ///< uart message trans done
} hal_uart_msg_type;

/** @brief hal uart device id */
typedef uint16_t hal_uart_id_t;

/** @brief uart mesage call back function */
typedef void (*hal_uart_msg_cb)(hal_uart_id_t key_id, hal_uart_msg_type msg_type);

/** @brief uart receive call back function */
typedef void (*hal_uart_rev_cb)(hal_uart_id_t key_id, uint8_t *data_p, uint16_t len);

/** @brief uart device struct. */
typedef struct {
    com_uart_alloc_state_t alloc_state; ///< alloc state
    hal_uart_msg_cb msg_cb;             ///< uart message call back function
    hal_uart_rev_cb rev_cb;             ///< uart receive call back function
    UART_HandleTypeDef *uart_p;         ///< uart device
    DMA_HandleTypeDef *dma_rx_p;        ///< uart dma rx device
    uint8_t *rev_buf_p;                 ///< rx buf
    uint16_t rev_buf_len;               ///< rx buf length
}hal_uart_dev_node_t;

/**
 * @brief uart init
 * @return error code.
 */
int hal_uart_init(uint8_t count);

/**
 * @brief creat a uart device
 * @param  id: the uart device id
 * @param  uart_p: stm32 uart
 * @param  dma_rx_p: stm32 dma rx
 * @param  msg_cb: uart message call back function
 * @param  rev_cb: uart receive call back function
 * @param  data_p: rx buf
 * @param  len: rx buf length
 * @return error code.
 */
int hal_uart_creat(hal_uart_id_t *id, UART_HandleTypeDef *uart_p, DMA_HandleTypeDef *dma_rx_p, hal_uart_msg_cb msg_cb, hal_uart_rev_cb rev_cb, uint8_t *data_p, uint16_t len);

/**
 * @brief enable uart
 * @return error code.
 */
int hal_uart_enable(hal_uart_id_t id);

/**
 * @brief disable uart
 * @return error code.
 */
int hal_uart_disable(hal_uart_id_t id);

/**
 * @brief uart trans data
 * @param  id: the uart device id
 * @param  data_p: tx buf
 * @param  len: tx buf length
 * @return error code.
 */
int hal_uart_trans(hal_uart_id_t id, uint8_t *data_p, uint16_t len);

/**
 * @brief uart rev function. please call this function in uart irq
 * @param  huart: stm32 uart
 * @return error code.
 */
int hal_uart_irq(UART_HandleTypeDef *huart);

/*
here is a example

hal_uart_id_t app_uart;
uint8_t uart_rx_buf[UART_RX_BUF_LEN];		//
uint8_t uart_tx_buf[UART_TX_BUF_LEN];		//

void uart_cb(hal_uart_id_t key_id, uint8_t *data_p, uint16_t len)
{
    SEGGER_RTT_printf(0, "%s\r\n",data_p);
}

hal_uart_init(1);
hal_uart_creat(&app_uart,&huart4,&hdma_usart4_rx,NULL,uart_cb,uart_rx_buf,UART_RX_BUF_LEN);
hal_uart_trans(app_uart,(uint8_t*)"Hello",6);

do not forget add this line in IT file
hal_uart_irq(&huart4);

*/

#endif  // __HAL_UART_H__
