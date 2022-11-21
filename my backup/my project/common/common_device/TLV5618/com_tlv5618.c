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

static com_tlv5618_node_t  tlv5618_dev;
com_tlv5618_node_t *tlv5618_node_p;


int com_tlv5618_init(hal_spi_bus_id_t spi_bus,hal_spi_dev_id_t spi_dev,uint8_t tlv5618_count)
{
    uint16_t i;
    
    tlv5618_node_p = (com_tlv5618_node_t *)malloc(tlv5618_count * sizeof(com_tlv5618_node_t));   // 
    if(key_dev_node_p == NULL){
        return COM_ERROR_NO_MEM;
    }
}

int com_tlv5618_creat(hal_spi_bus_id_t spi_bus,hal_spi_dev_id_t spi_dev)
{
    tlv5618_dev.spi_bus = spi_bus;
    tlv5618_dev.spi_dev = spi_dev;
    return COM_SUCCESS;
}

int com_tlv5618_write(uint8_t cmd, uint16_t data)
{
    uint8_t two_byte[] = {(cmd | (data >> 8)), (data & 0x00ff)};
    
    hal_spi_dev_write(tlv5618_dev.spi_bus,tlv5618_dev.spi_dev,two_byte,2);
    hal_spi_dev_write(tlv5618_dev.spi_bus,tlv5618_dev.spi_dev,two_byte,2);
    
    return COM_SUCCESS;
}

int com_tlv5618_set_voltage(uint8_t cmd, float voltage)
{
    uint16_t data = (int)(2048 / 1.22 * voltage + 0.5);
    com_tlv5618_write(cmd, data);
    
    return COM_SUCCESS;
}
