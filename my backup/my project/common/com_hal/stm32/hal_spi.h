/**
 * @file hal_spi.c
 * @brief spi in hal layer, just for STM32 MCU.
 * @details spi in hal layer, just for STM32 MCU.
 * @author Xiao Kun
 * @version V1.0
 * @date 2020.11.10
 */

#ifndef __HAL_SPI_H__
#define __HAL_SPI_H__

#include <stdint.h>
#include "gpio.h"

/** @brief the allocated state of spi device */
typedef enum
{
    HAL_SPI_STATE_FREE   = 0,            ///< free state
    HAL_SPI_ALLOCATED    = 1,            ///< allocated state
} com_spi_alloc_state_t;

/** @brief spi message type. */
typedef enum {
	HAL_SPI_MSG_TRANS_START = 0,       ///< uart message trans start
    HAL_SPI_MSG_TRANS_DONE,            ///< uart message trans done
} hal_spi_msg_type;

/** @brief hal spi device id */
typedef uint16_t hal_spi_bus_id_t;

/** @brief hal spi device id */
typedef uint16_t hal_spi_dev_id_t;

/** @brief spi device struct. */
typedef struct {
    com_spi_alloc_state_t alloc_state;  ///< alloc state
    GPIO_TypeDef *port_p;               ///< spi cs port
    uint16_t pin;                       ///< spi cs pin
}hal_spi_dev_node_t;

/** @brief spi bus struct. */
typedef struct {
    com_spi_alloc_state_t alloc_state; ///< alloc state
    SPI_HandleTypeDef *spi_bus_p;         ///< spi bus
    uint8_t dev_count;
    hal_spi_dev_node_t *spi_dev_node_p;
}hal_spi_bus_node_t;

/**
 * @brief spi bus init
 * @param  count: the spi bus count
 * @return error code.
 */
int hal_spi_bus_init(uint8_t bus_count);

/**
 * @brief creat a spi device
 * @param  bus_id_p: the spi bus id
 * @param  spi_bus_p: stm32 spi bus
 * @return error code.
 */
int hal_spi_bus_creat(hal_spi_bus_id_t *bus_id_p, SPI_HandleTypeDef *spi_bus_p);

/**
 * @brief spi dev init
 * @param  bus_id: the spi bus id
 * @param  count: the spi device count in the I2C bus
 * @return error code.
 */
int hal_spi_dev_init(hal_spi_bus_id_t bus_id, uint8_t dev_count);

/**
 * @brief spi dev init
 * @param  bus_id: the spi bus id
 * @param  dev_id_p: the spi device id
 * @param  port_p: the cs pin port
 * @param  pin: the cs pin num
 * @return error code.
 */
int hal_spi_dev_creat(hal_spi_bus_id_t bus_id, hal_spi_dev_id_t *dev_id_p, GPIO_TypeDef *port_p, uint16_t pin);

/**
 * @brief enable spi bus
 * @param  bus_id: the spi bus id
 * @return error code.
 */
int hal_spi_bus_enable(hal_spi_bus_id_t bus_id);

/**
 * @brief disable i2c bus
 * @param  bus_id: the I2C bus id
 * @return error code.
 */
int hal_spi_bus_disable(hal_spi_bus_id_t bus_id);

/**
 * @brief write data to spi device
 * @param  bus_id: the spi bus id
 * @param  dev_id: the spi device id
 * @param  data_p: the data pointer want write to spi device
 * @param  data_len: the data len
 * @return error code.
 */
int hal_spi_dev_write(hal_spi_bus_id_t bus_id, hal_spi_dev_id_t dev_id, uint8_t *data_p, uint16_t data_len);

/**
 * @brief write data to spi device without CS pin
 * @param  bus_id: the spi bus id
 * @param  dev_id: the spi device id
 * @param  data_p: the data pointer want write to spi device
 * @param  data_len: the data len
 * @return error code.
 */
int hal_spi_dev_write_without_cs(hal_spi_bus_id_t bus_id, uint8_t *data_p, uint16_t data_len);

/**
 * @brief read data to spi device
 * @param  bus_id: the spi bus id
 * @param  dev_id: the spi device id
 * @param  data_p: the data pointer want read from i2c device
 * @param  data_len: the data len
 * @return error code.
 */
int hal_spi_dev_read(hal_spi_bus_id_t bus_id, hal_spi_dev_id_t dev_id, uint8_t *data_p, uint16_t data_len);

/**
 * @brief read data to spi device
 * @param  bus_id: the spi bus id
 * @param  dev_id: the spi device id
 * @param  data_p: the data pointer want read from i2c device
 * @param  data_len: the data len
 * @return error code.
 */
int hal_spi_dev_write_read(hal_spi_bus_id_t bus_id, hal_spi_dev_id_t dev_id, uint8_t *tx_data_p, uint8_t *rx_data_p, uint16_t data_len);

/*
here is a example

hal_spi_bus_init(2);
hal_spi_bus_creat(&hal_spi1,&hspi1);
hal_spi_dev_init(hal_spi1,1);
hal_spi_dev_creat(hal_spi1,&lcd_rom,ROM_CS_GPIO_Port,ROM_CS_Pin);
    
*/

#endif  // __HAL_SPI_H__
