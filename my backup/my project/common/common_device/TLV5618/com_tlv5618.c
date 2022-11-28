/**
 * @file com_lcd12864.c
 * @brief HS12864-15C lcd device driver
 * @details HS12864-15C lcd device driver
 * @author Ding Wenju
 * @version V1.0
 * @date 2022.11.07
 */
#include "hal_spi.h"
#include "com_tlv5618.h"
#include "com_error.h"
#include <stdlib.h>

com_tlv5618_node_t *tlv5618_node_p;
int hal_tlv5618_dev_node_count = 0;

int com_tlv5618_init(uint8_t tlv5618_count)
{
    uint16_t i;
    
    tlv5618_node_p = (com_tlv5618_node_t *)malloc(tlv5618_count * sizeof(com_tlv5618_node_t));   // 
    if(tlv5618_node_p == NULL){
        return COM_ERROR_NO_MEM;
    }
    
    for (i = 0; i < tlv5618_count; i++){
        tlv5618_node_p[i].alloc_state = HAL_tlv5618_STATE_FREE;
    }
    hal_tlv5618_dev_node_count = i;
    
    return COM_SUCCESS;
}

int com_tlv5618_creat(uint16_t *id,hal_spi_bus_id_t spi_bus,hal_spi_dev_id_t spi_dev)
{
    if(id == NULL){
        return COM_ERROR_INVALID_STATE;
    }
    
        
    
    for(int i=0;i<hal_tlv5618_dev_node_count;i++){
        if(tlv5618_node_p[i].alloc_state == HAL_tlv5618_STATE_FREE){
            tlv5618_node_p[i].alloc_state = HAL_tlv5618_ALLOCATED;
            tlv5618_node_p[i].spi_bus = spi_bus;
            tlv5618_node_p[i].spi_dev = spi_dev;      
            *id = i;
            return COM_SUCCESS;
        }
    }
    return COM_SUCCESS;
}

int com_tlv5618_write(uint16_t id,uint8_t cmd,uint16_t data)
{
    uint8_t two_byte[] = {(cmd | (data >> 8)), (data & 0x00ff)};
    
    hal_spi_dev_write(tlv5618_node_p[id].spi_bus,tlv5618_node_p[id].spi_dev,two_byte,2);
    hal_spi_dev_write(tlv5618_node_p[id].spi_bus,tlv5618_node_p[id].spi_dev,two_byte,2);
    
    return COM_SUCCESS;
}

int com_tlv5618_set_voltage(uint16_t id,uint8_t cmd,float voltage)
{
    uint16_t data = (int)(2048 / 1.22 * voltage + 0.5);
    com_tlv5618_write(id, cmd, data);
    
    return COM_SUCCESS;
}
