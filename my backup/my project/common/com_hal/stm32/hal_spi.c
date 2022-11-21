/**
 * @file hal_spi.c
 * @brief spi in hal layer, just for STM32 MCU.
 * @details spi in hal layer, just for STM32 MCU.
 * @author Xiao Kun
 * @version V1.0
 * @date 2020.11.10
 */

#include "hal_spi.h"
#include <stdlib.h>
#include <string.h>
#include "com_error.h"
#include "hal_base.h"

static hal_spi_bus_node_t *spi_bus_node_p;
static int hal_spi_bus_node_count = 0;

int hal_spi_bus_init(uint8_t bus_count)
{
    uint16_t i;
    
    spi_bus_node_p = (hal_spi_bus_node_t *)malloc(bus_count * sizeof(hal_spi_bus_node_t));   // 
    if(spi_bus_node_p == NULL){
        return COM_ERROR_NO_MEM;
    }
    
    for (i = 0; i < bus_count; i++){
        spi_bus_node_p[i].alloc_state = HAL_SPI_STATE_FREE;
        spi_bus_node_p[i].dev_count = 0;
        spi_bus_node_p[i].spi_dev_node_p = NULL;
        spi_bus_node_p[i].spi_bus_p = NULL;
    }
    hal_spi_bus_node_count = bus_count;
    
    return COM_SUCCESS;
}

int hal_spi_bus_creat(hal_spi_bus_id_t *bus_id_p, SPI_HandleTypeDef *spi_bus_p)
{
    if((bus_id_p == NULL) || (spi_bus_p == NULL)){
        return COM_ERROR_INVALID_STATE;
    }
    
    int i;
    for(i=0;i<hal_spi_bus_node_count;i++){
        if(spi_bus_node_p[i].alloc_state == HAL_SPI_STATE_FREE){
            spi_bus_node_p[i].alloc_state = HAL_SPI_ALLOCATED;
            spi_bus_node_p[i].spi_bus_p = spi_bus_p;
            
            *bus_id_p = i;
            return COM_SUCCESS;
        }
    }
    
    return COM_ERROR_NO_MEM;
}

int hal_spi_dev_init(hal_spi_bus_id_t bus_id, uint8_t dev_count)
{
    if(bus_id >= hal_spi_bus_node_count){
        return COM_ERROR_INVALID_STATE;
    }

    if((spi_bus_node_p[bus_id].alloc_state == HAL_SPI_ALLOCATED) && (spi_bus_node_p[bus_id].spi_bus_p != NULL)){
        spi_bus_node_p[bus_id].spi_dev_node_p = (hal_spi_dev_node_t *)malloc(dev_count * sizeof(hal_spi_dev_node_t)); 
        if(spi_bus_node_p[bus_id].spi_dev_node_p == NULL){
            return COM_ERROR_NO_MEM;
        }
        for (int i = 0; i < dev_count; i++){
            spi_bus_node_p[bus_id].spi_dev_node_p[i].alloc_state = HAL_SPI_STATE_FREE;
            spi_bus_node_p[bus_id].spi_dev_node_p[i].port_p = NULL;
            spi_bus_node_p[bus_id].spi_dev_node_p[i].pin = 0;
        }
        spi_bus_node_p[bus_id].dev_count = dev_count;
        return COM_SUCCESS;
    }
    
    return COM_ERROR_INVALID_STATE;
}

int hal_spi_dev_creat(hal_spi_bus_id_t bus_id, hal_spi_dev_id_t *dev_id_p, GPIO_TypeDef *port_p, uint16_t pin)
{
    if(bus_id >= hal_spi_bus_node_count){
        return COM_ERROR_INVALID_STATE;
    }
    
    if((dev_id_p == NULL) || (port_p == NULL)){
        return COM_ERROR_INVALID_PARAM;
    }
    
    if((spi_bus_node_p[bus_id].alloc_state == HAL_SPI_ALLOCATED) && (spi_bus_node_p[bus_id].spi_bus_p != NULL)){
        for(int i=0;i<spi_bus_node_p[bus_id].dev_count;i++){
            if(spi_bus_node_p[bus_id].spi_dev_node_p[i].alloc_state == HAL_SPI_STATE_FREE){
                spi_bus_node_p[bus_id].spi_dev_node_p[i].alloc_state = HAL_SPI_ALLOCATED;
                spi_bus_node_p[bus_id].spi_dev_node_p[i].port_p = port_p;
                spi_bus_node_p[bus_id].spi_dev_node_p[i].pin = pin;
                
                *dev_id_p = i;
                return COM_SUCCESS;
            }
            return COM_ERROR_NO_MEM;
        }
    }
    
    return COM_ERROR_INVALID_STATE;
}

int hal_spi_bus_enable(hal_spi_bus_id_t bus_id)
{
    return COM_SUCCESS;
}

int hal_spi_bus_disable(hal_spi_bus_id_t bus_id)
{
    return COM_SUCCESS;
}

int hal_spi_dev_write(hal_spi_bus_id_t bus_id, hal_spi_dev_id_t dev_id, uint8_t *data_p, uint16_t data_len)
{
    if(bus_id >= hal_spi_bus_node_count){
        return COM_ERROR_INVALID_STATE;
    }
    
    if(dev_id >= spi_bus_node_p[bus_id].dev_count){
        return COM_ERROR_INVALID_STATE;
    }
    
    if((data_p == NULL) || (data_len == 0)){
        return COM_ERROR_INVALID_PARAM;
    }
    
    HAL_StatusTypeDef sta;
    HAL_GPIO_WritePin(spi_bus_node_p[bus_id].spi_dev_node_p[dev_id].port_p, spi_bus_node_p[bus_id].spi_dev_node_p[dev_id].pin, GPIO_PIN_RESET);
    sta = HAL_SPI_Transmit(spi_bus_node_p[bus_id].spi_bus_p, data_p, data_len, 100);
    HAL_GPIO_WritePin(spi_bus_node_p[bus_id].spi_dev_node_p[dev_id].port_p, spi_bus_node_p[bus_id].spi_dev_node_p[dev_id].pin, GPIO_PIN_SET);
    if(sta!=HAL_OK){
        return sta;
    }

    return COM_SUCCESS;
}

int hal_spi_dev_write_without_cs(hal_spi_bus_id_t bus_id, hal_spi_dev_id_t dev_id, uint8_t *data_p, uint16_t data_len)
{
    if(bus_id >= hal_spi_bus_node_count){
        return COM_ERROR_INVALID_STATE;
    }
    
    if(dev_id >= spi_bus_node_p[bus_id].dev_count){
        return COM_ERROR_INVALID_STATE;
    }
    
    if((data_p == NULL) || (data_len == 0)){
        return COM_ERROR_INVALID_PARAM;
    }
    
    HAL_StatusTypeDef sta;
    sta = HAL_SPI_Transmit(spi_bus_node_p[bus_id].spi_bus_p, data_p, data_len, 100);
    if(sta!=HAL_OK){
        return sta;
    }

    return COM_SUCCESS;
}

int hal_spi_dev_read(hal_spi_bus_id_t bus_id, hal_spi_dev_id_t dev_id, uint8_t *data_p, uint16_t data_len)
{
    if(bus_id >= hal_spi_bus_node_count){
        return COM_ERROR_INVALID_STATE;
    }
    
    if(dev_id >= spi_bus_node_p[bus_id].dev_count){
        return COM_ERROR_INVALID_STATE;
    }
    
    if((data_p == NULL) || (data_len == 0)){
        return COM_ERROR_INVALID_PARAM;
    }
    
    HAL_StatusTypeDef sta;
    HAL_GPIO_WritePin(spi_bus_node_p[bus_id].spi_dev_node_p[dev_id].port_p, spi_bus_node_p[bus_id].spi_dev_node_p[dev_id].pin, GPIO_PIN_RESET);
    sta = HAL_SPI_Receive(spi_bus_node_p[bus_id].spi_bus_p, data_p, data_len, 100);
    HAL_GPIO_WritePin(spi_bus_node_p[bus_id].spi_dev_node_p[dev_id].port_p, spi_bus_node_p[bus_id].spi_dev_node_p[dev_id].pin, GPIO_PIN_SET);
    if(sta!=HAL_OK){
        return sta;
    }

    return COM_SUCCESS;
}

int hal_spi_dev_write_read(hal_spi_bus_id_t bus_id, hal_spi_dev_id_t dev_id, uint8_t *tx_data_p, uint8_t *rx_data_p, uint16_t data_len)
{
    if(bus_id >= hal_spi_bus_node_count){
        return COM_ERROR_INVALID_STATE;
    }
    
    if(dev_id >= spi_bus_node_p[bus_id].dev_count){
        return COM_ERROR_INVALID_STATE;
    }
    
    if((tx_data_p == NULL) || (data_len == 0) || (rx_data_p == NULL)){
        return COM_ERROR_INVALID_PARAM;
    }
    
    HAL_StatusTypeDef sta;
    HAL_GPIO_WritePin(spi_bus_node_p[bus_id].spi_dev_node_p[dev_id].port_p, spi_bus_node_p[bus_id].spi_dev_node_p[dev_id].pin, GPIO_PIN_RESET);
    sta = HAL_SPI_TransmitReceive(spi_bus_node_p[bus_id].spi_bus_p, tx_data_p, rx_data_p, data_len, 100);
    HAL_GPIO_WritePin(spi_bus_node_p[bus_id].spi_dev_node_p[dev_id].port_p, spi_bus_node_p[bus_id].spi_dev_node_p[dev_id].pin, GPIO_PIN_SET);
    if(sta!=HAL_OK){
        return sta;
    }

    return COM_SUCCESS;
}
