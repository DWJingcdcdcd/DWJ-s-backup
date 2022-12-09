/**
 * @file com_mds560r.c
 * @brief mds560r device driver
 * @details mds560r device driver
 * @author Ding Wenju
 * @version V1.0
 * @date 2022.12.07
 */
#include "hal_uart.h"
#include "com_mds560r.h"
#include "com_error.h"
#include "com_delay.h"
#include "usart.h"
#include <stdlib.h>

com_mds560r_node_t *mds560r_node_p;
int hal_mds560r_dev_node_count = 0;

uint8_t unlock1[] = {1,6,0,0,4,87,202,244};
uint8_t commond1[] = {1,6,0,6,0,1,168,11};
uint8_t confirm1[] = {1,6,0,0,8,174,15,182};

int com_mds560r_init(uint8_t mds560r_count)
{
    uint16_t i;
    
    mds560r_node_p = (com_mds560r_node_t *)malloc(mds560r_count * sizeof(com_mds560r_node_t));   // 
    if(mds560r_node_p == NULL){
        return COM_ERROR_NO_MEM;
    }
    
    for (i = 0; i < mds560r_count; i++){
        mds560r_node_p[i].alloc_state = HAL_mds560r_STATE_FREE;
        mds560r_node_p[i].uart_dev = 0;
    }
    hal_mds560r_dev_node_count = i;
    
    return COM_SUCCESS;
}

int com_mds560r_creat(uint16_t *id,hal_uart_id_t uart_dev)
{
    if(id == NULL){
        return COM_ERROR_INVALID_STATE;
    }
    
    for(int i=0;i<hal_mds560r_dev_node_count;i++){
        if(mds560r_node_p[i].alloc_state == HAL_mds560r_STATE_FREE){
            mds560r_node_p[i].alloc_state = HAL_mds560r_ALLOCATED;
            mds560r_node_p[i].uart_dev = uart_dev;      
            *id = i;
            return COM_SUCCESS;
        }
    }
    return COM_SUCCESS;
}

int com_mds560r_read_data(uint16_t id, uint8_t *cmd, hal_do_id_t mds560r_dir)
{
    hal_do_output_high( mds560r_dir );     ///< Pull up the direction select signal
    com_delay_ms(10);
    hal_uart_trans(mds560r_node_p[id].uart_dev, cmd, 8);
    hal_do_output_low( mds560r_dir );
    hal_uart_irq(&huart1);
    
    return COM_SUCCESS;
}

int com_mds560r_modify_para(uint16_t id, hal_do_id_t mds560r_dir)
{
    hal_do_output_high( mds560r_dir );     ///< Pull up the direction select signal
    com_delay_ms(10);
    hal_uart_trans(mds560r_node_p[id].uart_dev, unlock1, 8);
    hal_uart_trans(mds560r_node_p[id].uart_dev, commond1, 8);
    hal_uart_trans(mds560r_node_p[id].uart_dev, confirm1, 8);
    hal_do_output_low( mds560r_dir );
    
    return COM_SUCCESS;
}
