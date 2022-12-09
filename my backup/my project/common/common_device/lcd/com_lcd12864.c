/**
 * @file com_lcd12864.c
 * @brief HS12864-15C lcd device driver
 * @details HS12864-15C lcd device driver
 * @author Ding Wenju
 * @version V1.0
 * @date 2022.11.07
 */
#include "hal_spi.h"
#include "com_lcd_dev.h"
#include "com_error.h"
#include "com_delay.h"

static com_lcd_dev_node_t lcd_dev;

uint8_t LCD_addr[4][8]={
	    {0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87},  	    ///< First row
	    {0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97},		///< Second row
	    {0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F},		///< Third row
	    {0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F}		///< Fourth row
    };

int com_lcd_creat(hal_spi_bus_id_t spi_bus,hal_spi_dev_id_t spi_dev,hal_do_id_t rs,hal_do_id_t rst)
{
    lcd_dev.spi_bus = spi_bus;
    lcd_dev.spi_dev = spi_dev;
    lcd_dev.rst = rst;
    lcd_dev.rs = rs;
    
    return COM_SUCCESS;
}

int com_lcd_write_cmd(uint8_t cmd)
{
    uint8_t cmd_key = 0xF8;
    uint8_t cmd_array[] = {0xf0 & cmd, cmd << 4};
    
    com_delay_ms(1);
    hal_spi_dev_write_without_cs(lcd_dev.spi_bus,&cmd_key,1);
    com_delay_ms(5);
    hal_spi_dev_write_without_cs(lcd_dev.spi_bus,&cmd_array[0],1);
    com_delay_ms(1);
    hal_spi_dev_write_without_cs(lcd_dev.spi_bus,&cmd_array[1],1);
    com_delay_ms(1);
    
    return COM_SUCCESS;
}

int com_lcd_write_data(uint8_t data)
{
    uint8_t dat_key = 0xFA;
    uint8_t data_array[] = {0xf0 & data, data << 4};
    
    com_delay_ms(1);
    hal_spi_dev_write_without_cs(lcd_dev.spi_bus,&dat_key,1);
    com_delay_ms(5);
    hal_spi_dev_write_without_cs(lcd_dev.spi_bus,&data_array[0],1);
    com_delay_ms(1);
    hal_spi_dev_write_without_cs(lcd_dev.spi_bus,&data_array[1],1);
    com_delay_ms(1);
    
    return COM_SUCCESS;
}

int com_lcd_init(void)
{
    hal_do_output_high(lcd_dev.rs);     ///< Pull up the chip select signal
    
    //com_delay_ms(50);
    com_lcd_write_cmd(0x30);
    com_delay_ms(1);
    com_lcd_write_cmd(0x06);
    com_delay_ms(1);
    com_lcd_write_cmd(0x01);
    com_delay_ms(10);
    com_lcd_write_cmd(0x0C);
    com_delay_ms(1);
    com_lcd_write_cmd(0x02);
    com_delay_ms(10);
    
    return COM_SUCCESS;
}

int com_lcd_clear_screen(void)
{
    com_delay_ms(4);
    com_lcd_write_cmd(0x01);
    com_delay_ms(4);
    
    return COM_SUCCESS;
}

int com_lcd_disp_str(uint8_t row, uint8_t column, uint8_t *str)
{ 
    com_lcd_write_cmd(LCD_addr[row][column]); ///< the starting cursor position
    while(*str>0){ 
        com_lcd_write_data(*str);             ///< write data
        str++;     
    }
    
    return COM_SUCCESS;
}
