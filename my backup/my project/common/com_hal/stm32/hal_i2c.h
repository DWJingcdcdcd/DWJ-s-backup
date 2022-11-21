/**
 * @file hal_i2c.h
 * @brief i2c in hal layer, just for STM32 MCU.
 * @details i2c in hal layer, just for STM32 MCU.
 * @author Xiao Kun
 * @version V1.0
 * @date 2020.11.09
 */

#ifndef __HAL_I2C_H__
#define __HAL_I2C_H__

#include <stdint.h>
#include "gpio.h"

/** @brief the allocated state of I2C device */
typedef enum
{
    HAL_I2C_STATE_FREE   = 0,            ///< free state
    HAL_I2C_ALLOCATED    = 1,            ///< allocated state
} com_i2c_alloc_state_t;

/** @brief I2C message type. */
typedef enum {
	HAL_I2C_MSG_TRANS_START = 0,       ///< uart message trans start
    HAL_I2C_MSG_TRANS_DONE,            ///< uart message trans done
} hal_i2c_msg_type;

/** @brief hal I2C device id */
typedef uint16_t hal_i2c_bus_id_t;

/** @brief hal I2C device id */
typedef uint16_t hal_i2c_dev_id_t;

/** @brief I2C device struct. */
typedef struct {
    com_i2c_alloc_state_t alloc_state;  ///< alloc state
    uint8_t dev_addr;
}hal_i2c_dev_node_t;

/** @brief I2C bus struct. */
typedef struct {
    com_i2c_alloc_state_t alloc_state; ///< alloc state
    I2C_HandleTypeDef *i2c_bus_p;         ///< i2c bus
    uint8_t dev_count;
    hal_i2c_dev_node_t *i2c_dev_node_p;
}hal_i2c_bus_node_t;

/**
 * @brief I2C bus init
 * @param  count: the I2C bus count
 * @return error code.
 */
int hal_i2c_bus_init(uint8_t bus_count);

/**
 * @brief creat a I2C device
 * @param  bus_id_p: the I2C bus id
 * @param  i2c_bus_p: stm32 I2C bus
 * @return error code.
 */
int hal_i2c_bus_creat(hal_i2c_bus_id_t *bus_id_p, I2C_HandleTypeDef *i2c_bus_p);

/**
 * @brief I2C dev init
 * @param  bus_id: the I2C bus id
 * @param  count: the I2C device count in the I2C bus
 * @return error code.
 */
int hal_i2c_dev_init(hal_i2c_bus_id_t bus_id, uint8_t dev_count);

/**
 * @brief I2C dev init
 * @param  bus_id: the I2C bus id
 * @param  dev_id_p: the I2C device id
 * @param  dev_addr: the I2C device address
 * @return error code.
 */
int hal_i2c_dev_creat(hal_i2c_bus_id_t bus_id, hal_i2c_dev_id_t *dev_id_p, uint8_t dev_addr);

/**
 * @brief enable i2c bus
 * @param  bus_id: the I2C bus id
 * @return error code.
 */
int hal_i2c_bus_enable(hal_i2c_bus_id_t bus_id);

/**
 * @brief disable i2c bus
 * @param  bus_id: the I2C bus id
 * @return error code.
 */
int hal_i2c_bus_disable(hal_i2c_bus_id_t bus_id);

/**
 * @brief write data to I2C device
 * @param  bus_id: the I2C bus id
 * @param  dev_id: the I2C device id
 * @param  mem_addr: the I2C device register/memory address
 * @param  mem_size: the I2C device register/memory size. I2C_MEMADD_SIZE_8BIT-8bit, I2C_MEMADD_SIZE_16BIT-16bit
 * @param  data_p: the data pointer want write to i2c device
 * @param  data_len: the data len
 * @return error code.
 */
int hal_i2c_dev_write(hal_i2c_bus_id_t bus_id, hal_i2c_dev_id_t dev_id, uint16_t mem_addr, uint16_t mem_size, uint8_t *data_p, uint16_t data_len);

/**
 * @brief read data to I2C device
 * @param  bus_id: the I2C bus id
 * @param  dev_id: the I2C device id
 * @param  mem_addr: the I2C device register/memory address
 * @param  mem_size: the I2C device register/memory size. I2C_MEMADD_SIZE_8BIT-8bit, I2C_MEMADD_SIZE_16BIT-16bit
 * @param  data_p: the data pointer want read from i2c device
 * @param  data_len: the data len
 * @return error code.
 */
int hal_i2c_dev_read(hal_i2c_bus_id_t bus_id, hal_i2c_dev_id_t dev_id, uint16_t mem_addr, uint16_t mem_size, uint8_t *data_p, uint16_t data_len);

#endif  // __HAL_I2C_H__
