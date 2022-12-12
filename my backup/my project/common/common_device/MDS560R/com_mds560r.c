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

uint8_t unlock[]  = {1,6,0,0,4,87,202,244};
uint8_t kPa[]     = {1,6,0,6,0,0,105,203};
uint8_t PSI[]     = {1,6,0,6,0,1,168,11};
uint8_t MPa[]     = {1,6,0,6,0,2,232,10};
uint8_t Kgf[] = {1,6,0,6,0,3,41,202};
uint8_t Bar[]     = {1,6,0,6,0,4,104,8};
uint8_t Pa[]      = {1,6,0,6,0,5,169,200};
uint8_t confirm[] = {1,6,0,0,8,174,15,182};
uint8_t read_data[] = {1,3,0,0,0,1,132,10};

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
        mds560r_node_p[i].dir = 0;
    }
    hal_mds560r_dev_node_count = i;
    
    return COM_SUCCESS;
}

int com_mds560r_creat(uint16_t *id,hal_uart_id_t uart_dev,hal_do_id_t dir)
{
    if(id == NULL){
        return COM_ERROR_INVALID_STATE;
    }
    
    for(int i=0;i<hal_mds560r_dev_node_count;i++){
        if(mds560r_node_p[i].alloc_state == HAL_mds560r_STATE_FREE){
            mds560r_node_p[i].alloc_state = HAL_mds560r_ALLOCATED;
            mds560r_node_p[i].uart_dev = uart_dev;   
            mds560r_node_p[i].dir = dir;            
            *id = i;
            return COM_SUCCESS;
        }
    }
    return COM_SUCCESS;
}

int com_mds560r_read_data(uint16_t id)
{
    hal_do_output_high( mds560r_node_p[id].dir );     ///< Pull up the direction select signal
    com_delay_ms(10);
    hal_uart_trans(mds560r_node_p[id].uart_dev, read_data, 8);
    com_delay_ms(10);
    hal_do_output_low( mds560r_node_p[id].dir );
    
    return COM_SUCCESS;
}

int com_mds560r_modify_unit(uint16_t id, uint8_t unit)
{
    hal_do_output_high( mds560r_node_p[id].dir );     ///< Pull up the direction select signal
    com_delay_ms(10);
    hal_uart_trans(mds560r_node_p[id].uart_dev, unlock, 8);
    com_delay_ms(150);
    switch(unit){
        case Unit_kPa:
            hal_uart_trans(mds560r_node_p[id].uart_dev, kPa, 8);
            break;
        case Unit_PSI:
            hal_uart_trans(mds560r_node_p[id].uart_dev, PSI, 8);
            break;
        case Unit_MPa:
            hal_uart_trans(mds560r_node_p[id].uart_dev, MPa, 8);
            break;
        case Unit_Kgf:
            hal_uart_trans(mds560r_node_p[id].uart_dev, Kgf, 8);
            break;
        case Unit_Bar:
            hal_uart_trans(mds560r_node_p[id].uart_dev, Bar, 8);
            break;
        case Unit_Pa:
            hal_uart_trans(mds560r_node_p[id].uart_dev, Pa, 8);
            break;
        default:
            break;
    }   
    com_delay_ms(150);
    hal_uart_trans(mds560r_node_p[id].uart_dev, confirm, 8);
    com_delay_ms(10);
    hal_do_output_low( mds560r_node_p[id].dir );
    
    return COM_SUCCESS;
}
