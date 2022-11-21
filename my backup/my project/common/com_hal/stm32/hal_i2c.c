/**
 * @file hal_i2c.c
 * @brief i2c in hal layer, just for STM32 MCU.
 * @details i2c in hal layer, just for STM32 MCU.
 * @author Xiao Kun
 * @version V1.0
 * @date 2020.11.09
 */

#include "hal_i2c.h"
#include <stdlib.h>
#include <string.h>
#include "com_error.h"
#include "hal_base.h"

static hal_i2c_bus_node_t *i2c_bus_node_p;
static int hal_i2c_bus_node_count = 0;

int hal_i2c_bus_init(uint8_t bus_count)
{
    uint16_t i;
    
    i2c_bus_node_p = (hal_i2c_bus_node_t *)malloc(bus_count * sizeof(hal_i2c_bus_node_t));   // 
    if(i2c_bus_node_p == NULL){
        return COM_ERROR_NO_MEM;
    }
    
    for (i = 0; i < bus_count; i++){
        i2c_bus_node_p[i].alloc_state = HAL_I2C_STATE_FREE;
        i2c_bus_node_p[i].dev_count = 0;
        i2c_bus_node_p[i].i2c_dev_node_p = NULL;
        i2c_bus_node_p[i].i2c_bus_p = NULL;
    }
    hal_i2c_bus_node_count = bus_count;
    
    return COM_SUCCESS;
}

int hal_i2c_bus_creat(hal_i2c_bus_id_t *bus_id_p, I2C_HandleTypeDef *i2c_bus_p)
{
    if((bus_id_p == NULL) || (i2c_bus_p == NULL)){
        return COM_ERROR_INVALID_STATE;
    }
    
    int i;
    for(i=0;i<hal_i2c_bus_node_count;i++){
        if(i2c_bus_node_p[i].alloc_state == HAL_I2C_STATE_FREE){
            i2c_bus_node_p[i].alloc_state = HAL_I2C_ALLOCATED;
            i2c_bus_node_p[i].i2c_bus_p = i2c_bus_p;
            
            *bus_id_p = i;
            return COM_SUCCESS;
        }
    }
    
    return COM_ERROR_NO_MEM;
}

int hal_i2c_dev_init(hal_i2c_bus_id_t bus_id, uint8_t dev_count)
{
    if(bus_id >= hal_i2c_bus_node_count){
        return COM_ERROR_INVALID_STATE;
    }

    if((i2c_bus_node_p[bus_id].alloc_state == HAL_I2C_ALLOCATED) && (i2c_bus_node_p[bus_id].i2c_bus_p != NULL)){
        i2c_bus_node_p[bus_id].i2c_dev_node_p = (hal_i2c_dev_node_t *)malloc(dev_count * sizeof(hal_i2c_dev_node_t)); 
        if(i2c_bus_node_p[bus_id].i2c_dev_node_p == NULL){
            return COM_ERROR_NO_MEM;
        }
        for (int i = 0; i < dev_count; i++){
            i2c_bus_node_p[bus_id].i2c_dev_node_p[i].alloc_state = HAL_I2C_STATE_FREE;
            i2c_bus_node_p[bus_id].i2c_dev_node_p[i].dev_addr = 0x00;
        }
        i2c_bus_node_p[bus_id].dev_count = dev_count;
        return COM_SUCCESS;
    }
    
    return COM_ERROR_INVALID_STATE;
}

int hal_i2c_dev_creat(hal_i2c_bus_id_t bus_id, hal_i2c_dev_id_t *dev_id_p, uint8_t dev_addr)
{
    if(bus_id >= hal_i2c_bus_node_count){
        return COM_ERROR_INVALID_STATE;
    }
    
    if((dev_id_p == NULL) || (dev_addr == 0)){
        return COM_ERROR_INVALID_PARAM;
    }
    
    if((i2c_bus_node_p[bus_id].alloc_state == HAL_I2C_ALLOCATED) && (i2c_bus_node_p[bus_id].i2c_bus_p != NULL)){
        for(int i=0;i<i2c_bus_node_p[bus_id].dev_count;i++){
            if(i2c_bus_node_p[bus_id].i2c_dev_node_p[i].alloc_state == HAL_I2C_STATE_FREE){
                i2c_bus_node_p[bus_id].i2c_dev_node_p[i].alloc_state = HAL_I2C_ALLOCATED;
                i2c_bus_node_p[bus_id].i2c_dev_node_p[i].dev_addr = dev_addr;
                
                *dev_id_p = i;
                return COM_SUCCESS;
            }
            return COM_ERROR_NO_MEM;
        }
    }
    
    return COM_ERROR_INVALID_STATE;
}

int hal_i2c_bus_enable(hal_i2c_bus_id_t bus_id)
{
    return COM_SUCCESS;
}

int hal_i2c_bus_disable(hal_i2c_bus_id_t bus_id)
{
    return COM_SUCCESS;
}

int hal_i2c_dev_write(hal_i2c_bus_id_t bus_id, hal_i2c_dev_id_t dev_id, uint16_t mem_addr, uint16_t mem_size, uint8_t *data_p, uint16_t data_len)
{
    if(bus_id >= hal_i2c_bus_node_count){
        return COM_ERROR_INVALID_STATE;
    }
    
    if(dev_id >= i2c_bus_node_p[bus_id].dev_count){
        return COM_ERROR_INVALID_STATE;
    }
    
    if((mem_size != I2C_MEMADD_SIZE_8BIT) && (mem_size != I2C_MEMADD_SIZE_16BIT)){
        return COM_ERROR_INVALID_PARAM;
    }
    
    if((data_p == NULL) || (data_len == 0)){
        return COM_ERROR_INVALID_PARAM;
    }
    
    HAL_StatusTypeDef sta;
    sta = HAL_I2C_Mem_Write(i2c_bus_node_p[bus_id].i2c_bus_p,i2c_bus_node_p[bus_id].i2c_dev_node_p[dev_id].dev_addr,mem_addr,mem_size,data_p,data_len,1000);
    if(sta!=HAL_OK){
        return sta;
    }
    
    //HAL_I2C_Mem_Write_IT(i2c_bus_node_p[bus_id].i2c_bus_p,i2c_bus_node_p[bus_id].uart_dev_node_p[dev_id].dev_addr,mem_addr,mem_size,data_p,data_len);

    return COM_SUCCESS;
}

int hal_i2c_dev_read(hal_i2c_bus_id_t bus_id, hal_i2c_dev_id_t dev_id, uint16_t mem_addr, uint16_t mem_size, uint8_t *data_p, uint16_t data_len)
{
    if(bus_id >= hal_i2c_bus_node_count){
        return COM_ERROR_INVALID_STATE;
    }
    
    if(dev_id >= i2c_bus_node_p[bus_id].dev_count){
        return COM_ERROR_INVALID_STATE;
    }
    
    if((mem_size != I2C_MEMADD_SIZE_8BIT) && (mem_size != I2C_MEMADD_SIZE_16BIT)){
        return COM_ERROR_INVALID_PARAM;
    }
    
    if((data_p == NULL) || (data_len == 0)){
        return COM_ERROR_INVALID_PARAM;
    }
    
    HAL_StatusTypeDef sta;
    sta = HAL_I2C_Mem_Read(i2c_bus_node_p[bus_id].i2c_bus_p,i2c_bus_node_p[bus_id].i2c_dev_node_p[dev_id].dev_addr,mem_addr,mem_size,data_p,data_len,1000);
    if(sta!=HAL_OK){
        return sta;
    }

    return COM_SUCCESS;
}
