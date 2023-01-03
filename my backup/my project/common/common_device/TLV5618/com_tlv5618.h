/**
 * @file com_tlv5618.h
 * @brief tlv5618 driver
 * @details tlv5618 driver
 * @author Ding Wenju
 * @version V1.0
 * @date 2022.11.10
 */
 
#ifndef __COM_TLV5618_H__
#define __COM_TLV5618_H__

#include "stdint.h"
#include "hal_spi.h"
#include "hal_do.h"

#define WRITE_DAC_A 0xC0 
#define WRITE_DAC_B 0x40

/** @brief the allocated state of tlv5618 device */
typedef enum
{
    HAL_tlv5618_STATE_FREE   = 0,            ///< free state
    HAL_tlv5618_ALLOCATED    = 1,            ///< allocated state
} com_tlv5618_alloc_state_t;

/** @brief tlv5618_device struct. */
typedef struct {
    com_tlv5618_alloc_state_t alloc_state;
    hal_spi_bus_id_t spi_bus;
    hal_spi_dev_id_t spi_dev;
}com_tlv5618_node_t;

/**
 * @brief initial the tlv5618 devices
 * @param  tlv5618_count: the count of tlv5618 devices
 * @return error code.
 */
int com_tlv5618_init(uint8_t tlv5618_count);

/**
 * @brief creat a tlv5618 device
 * @param  spi_bus: the spi bus id
 * @param  spi_dev: the spi device id
 * @return error code.
 */
int com_tlv5618_creat(uint16_t *id,hal_spi_bus_id_t spi_bus,hal_spi_dev_id_t spi_dev);


/**
 * @brief write to tlv5618 
 * @param  id: the tlv5618 device id
 * @param  cmd: the command
 * @param  data: the data
 * @return error code.
 */
int com_tlv5618_write(uint16_t id,uint8_t cmd,uint16_t data);

/**
 * @brief set tlv5618's output voltage  
 * @param  id: the tlv5618 device id
 * @param  cmd: the command
 * @param  voltage: the settting voltage
 * @return error code.
 */
int com_tlv5618_set_voltage(uint16_t id,uint8_t cmd,float voltage);

#endif //__COM_TLV5618_H__

