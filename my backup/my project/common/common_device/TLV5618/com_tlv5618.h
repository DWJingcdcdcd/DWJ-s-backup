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


/** @brief spi bus struct. */
typedef struct {
    hal_spi_bus_id_t spi_bus;
    hal_spi_dev_id_t spi_dev;
    hal_do_id_t cs;
}com_tlv5618_node_t;

/**
 * @brief creat a tlv5618 device
 * @param  spi_bus: the spi bus id
 * @param  spi_dev: the spi device id
 * @param  cs: the cs gpio id
 * @return error code.
 */
int com_tlv5618_creat(hal_spi_bus_id_t spi_bus,hal_spi_dev_id_t spi_dev);


/**
 * @brief write to tlv5618 
 * @param  cmd: the command
 * @param  data: the data
 * @return error code.
 */
int com_tlv5618_write(uint8_t cmd, uint16_t data);

int com_tlv5618_set_voltage(uint8_t cmd, float voltage);

#endif //__COM_TLV5618_H__

