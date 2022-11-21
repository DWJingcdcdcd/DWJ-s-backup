#include "stm32f1xx.h"
#include <stdint.h>
	
/** @brief the allocated state of lcd device */
typedef enum
{
    LCD_STATE_FREE   = 0,            ///< free state
    LCD_ALLOCATED    = 1,            ///< allocated state
} com_lcd_alloc_state_t;

typedef uint16_t lcd_device_id_t;

typedef struct {
	  com_lcd_alloc_state_t alloc_state;  ///< alloc state
    uint16_t SID_Pin;                   ///< SID_Pin
    GPIO_TypeDef * SID_Port;            ///< SID_Port
    uint16_t CLK_Pin;                   ///< CLK_Pin
    GPIO_TypeDef * CLK_Port;            ///< CLK_Port
	  uint16_t RST_Pin;                   ///< RST_Pin
    GPIO_TypeDef * RST_Port;            ///< RST_Port
}lcd_dev_node_t;


int lcd_device_init(int count);
int lcd_device_create(lcd_device_id_t *id, uint16_t SID_Pin, GPIO_TypeDef * SID_Port, uint16_t CLK_Pin, GPIO_TypeDef * CLK_Port, uint16_t RST_Pin, GPIO_TypeDef * RST_Port);
void lcd_reset(lcd_device_id_t id);

//清屏
void LCD_clear(void);


#define WRITE_CMD	0xF8//写命令  
#define WRITE_DAT	0xFA//写数据


//清屏
void LCD_Clear(lcd_device_id_t id);
//led串行发送一个字节
void SendByte(lcd_device_id_t id, uint8_t byte);
//写命令
void Lcd_WriteCmd(lcd_device_id_t id, uint8_t Cmd);
//写数据
void Lcd_WriteData(lcd_device_id_t id, uint8_t Dat);
//显示字符或汉字
void LCD_Display_Words(lcd_device_id_t id, uint8_t x, uint8_t y, uint8_t*str);
//led12864串行初始化
void Lcd_Init(lcd_device_id_t id);
void ShowNumChar(lcd_device_id_t id, uint8_t addr, uint8_t i, uint8_t count);



