/**
 * @file hal_uart.c
 * @brief uart in hal layer, just for STM32 MCU.
 * @details uart in hal layer, just for STM32 MCU.
 * @author Xiao Kun
 * @version V1.0
 * @date 2020.11.09
 */

#include "hal_uart.h"
#include <stdlib.h>
#include <string.h>
#include "com_error.h"
#include "hal_base.h"

hal_uart_dev_node_t *uart_dev_node_p;
static int hal_uart_dev_node_count = 0;

int hal_uart_init(uint8_t count)
{
    uint16_t i;
    
    uart_dev_node_p = (hal_uart_dev_node_t *)malloc(count * sizeof(hal_uart_dev_node_t));   // 
    if(uart_dev_node_p == NULL){
        return COM_ERROR_NO_MEM;
    }
    
    for (i = 0; i < count; i++)
    {
        uart_dev_node_p[i].alloc_state = HAL_UART_STATE_FREE;
        uart_dev_node_p[i].msg_cb = NULL;
        uart_dev_node_p[i].rev_cb = NULL;
        uart_dev_node_p[i].rev_buf_p = NULL;
        uart_dev_node_p[i].rev_buf_len = 0;
        uart_dev_node_p[i].uart_p = NULL;
        uart_dev_node_p[i].dma_rx_p = NULL;
    }
    hal_uart_dev_node_count = count;
    
    return COM_SUCCESS;
}

#include "SEGGER_RTT.h"
extern UART_HandleTypeDef huart2;
int hal_uart_creat(hal_uart_id_t *id, UART_HandleTypeDef *uart_p,DMA_HandleTypeDef *dma_rx_p, hal_uart_msg_cb msg_cb, hal_uart_rev_cb rev_cb, uint8_t *data_p, uint16_t len)
{
    if((id == NULL) || (uart_p == NULL)){
        return COM_ERROR_INVALID_STATE;
    }
    
    for(int i=0;i<hal_uart_dev_node_count;i++){
        if(uart_dev_node_p[i].alloc_state == HAL_UART_STATE_FREE){
            uart_dev_node_p[i].alloc_state = HAL_UART_ALLOCATED;
            uart_dev_node_p[i].uart_p = uart_p;
            uart_dev_node_p[i].msg_cb = msg_cb;
            uart_dev_node_p[i].rev_cb = rev_cb;
            uart_dev_node_p[i].rev_buf_p = data_p;
            uart_dev_node_p[i].rev_buf_len = len;
            uart_dev_node_p[i].dma_rx_p = dma_rx_p;
            __HAL_UART_ENABLE_IT(uart_dev_node_p[i].uart_p, UART_IT_IDLE);
            //while(__HAL_UART_GET_FLAG(uart_dev_node_p[i].uart_p,UART_FLAG_IDLE)==RESET){}

            //huart2.Instance->CR1 |= 0x10;
            if((data_p != NULL) && (len != 0)){
                HAL_UART_Receive_DMA(uart_dev_node_p[i].uart_p, data_p, len);
            } else {
                uart_dev_node_p[i].alloc_state = HAL_UART_STATE_FREE;
                return COM_ERROR_INVALID_PARAM;
            }
            *id = i;
            return COM_SUCCESS;
        }
    }
    return COM_ERROR_NO_MEM;
}

int hal_uart_enable(hal_uart_id_t id)
{
    return COM_SUCCESS;
}

int hal_uart_disable(hal_uart_id_t id)
{
    return COM_SUCCESS;
}

int hal_uart_trans(hal_uart_id_t id, uint8_t *data_p, uint16_t len)
{
    if(id >= hal_uart_dev_node_count){
        return COM_ERROR_INVALID_STATE;
    }
    
    if((data_p == NULL) || (len == 0)){
        return COM_ERROR_INVALID_PARAM;
    }
    
    if(uart_dev_node_p[id].uart_p != NULL){
        return HAL_UART_Transmit_DMA(uart_dev_node_p[id].uart_p, data_p, len);
    } else {
        return COM_ERROR_NULL;
    }
}

int hal_uart_irq(UART_HandleTypeDef *huart)
{
    if(huart == NULL){
        return COM_ERROR_INVALID_STATE;
    }
    
    int data_length;
    
    for(int i=0;i<hal_uart_dev_node_count;i++){
        if(uart_dev_node_p[i].uart_p->Instance == huart->Instance){
            // check if the uart is idle
            if(RESET != __HAL_UART_GET_FLAG(uart_dev_node_p[i].uart_p, UART_FLAG_IDLE)){
                // clear idle flage
                __HAL_UART_CLEAR_IDLEFLAG(uart_dev_node_p[i].uart_p);
                // stop dma
                HAL_UART_DMAStop(uart_dev_node_p[i].uart_p);
                if((uart_dev_node_p[i].rev_buf_p != NULL) && (uart_dev_node_p[i].rev_buf_len > 0)){
                    data_length  = uart_dev_node_p[i].rev_buf_len - __HAL_DMA_GET_COUNTER(uart_dev_node_p[i].dma_rx_p);
                    if(uart_dev_node_p[i].rev_cb != NULL){
                        uart_dev_node_p[i].rev_cb(i,uart_dev_node_p[i].rev_buf_p,data_length);
                    }
                    memset(uart_dev_node_p[i].rev_buf_p,0,data_length);
                }
                // restart dma rev
                HAL_UART_Receive_DMA(uart_dev_node_p[i].uart_p, uart_dev_node_p[i].rev_buf_p, uart_dev_node_p[i].rev_buf_len);
                return COM_SUCCESS;
            }
        }
    }
    
    return COM_ERROR_NOT_FOUND;
}
