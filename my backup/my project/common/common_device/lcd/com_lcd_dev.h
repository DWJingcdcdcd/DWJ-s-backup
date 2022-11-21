/**
 * @file com_lcd_dev.h
 * @brief lcd device driver
 * @details lcd device driver
 * @author Xiao Kun
 * @version V1.0
 * @date 2021.11.30
 */
 
#ifndef __COM_LCD_DEV_H__
#define __COM_LCD_DEV_H__

#include "stdint.h"
#include "hal_spi.h"
#include "hal_do.h"

/** @brief spi bus struct. */
typedef struct {
    hal_spi_bus_id_t spi_bus;
    hal_spi_dev_id_t spi_dev;
    hal_do_id_t rs;
    hal_do_id_t rst;
    uint16_t raw_pix_count;
    uint16_t cal_pix_count;
}com_lcd_dev_node_t;

/**
 * @brief creat a lcd device
 * @param  spi_bus: the spi bus id
 * @param  spi_dev: the spi device id
 * @param  rs: the rs gpio id
 * @param  rst: the rst gpio id
 * @return error code.
 */
int com_lcd_creat(hal_spi_bus_id_t spi_bus,hal_spi_dev_id_t spi_dev,hal_do_id_t rs,hal_do_id_t rst);

// TODO
int com_lcd_cfg(uint16_t raw_pix_count, uint16_t cal_pix_count);

/**
 * @brief write a command to lcd
 * @param  cmd: the command
 * @return error code.
 */
int com_lcd_write_cmd(uint8_t cmd);

/**
 * @brief write a data to lcd
 * @param  cmd: the command
 * @return error code.
 */
int com_lcd_write_data(uint8_t data);

/**
 * @brief init the lcd
 * @return error code.
 */
int com_lcd_init(void);

/**
 * @brief set the lcd address
 * @param  page: the lcd page
 * @param  column: the lcd column
 * @return error code.
 */
int com_set_lcd_addr(uint8_t page, uint16_t column);

/**
 * @brief clear the lcd
 * @return error code.
 */
int com_lcd_clear_screen(void);

/**
 * @brief display string in lcd
 * @param  row: the lcd row
 * @param  column: the lcd column
 * @param  str: the string pointer that want display in lcd
 * @return error code.
 */
int com_lcd_disp_str(uint8_t row, uint8_t column, uint8_t *str);

/**
 * @brief draw a 32*32 picture in lcd
 * @param  page: the lcd page
 * @param  column: the lcd column
 * @param  data_p: the picture data
 * @return error code.
 */
int com_lcd_draw_32x32(uint8_t page, uint16_t column, uint8_t *data_p);

/**
 * @brief draw a 16*16 picture in lcd
 * @param  page: the lcd page
 * @param  column: the lcd column
 * @param  data_p: the picture data
 * @return error code.
 */
int com_lcd_draw_16x16(uint8_t page, uint16_t column, uint8_t *data_p);

/**
 * @brief draw a 8*8 picture in lcd
 * @param  page: the lcd page
 * @param  column: the lcd column
 * @param  data_p: the picture data
 * @return error code.
 */
int com_lcd_draw_8x8(uint8_t page, uint16_t column, uint8_t *data_p);

/**
 * @brief draw a 16*8 picture in lcd
 * @param  page: the lcd page
 * @param  column: the lcd column
 * @param  data_p: the picture data
 * @return error code.
 */
int com_lcd_draw_16x8(uint8_t page, uint16_t column, uint8_t *data_p);

int com_lcd_draw_full_screen(uint8_t *data_p);

int com_lcd_draw_pic(uint8_t s_page, uint16_t s_column, uint16_t pic_raw, uint16_t pic_col, uint8_t *data_p);


#endif //__COM_LCD_DEV_H__

