#include "sys.h"

/*----------------------------------12864并行发送----------------------------------------------------*/
#define RS GPIO_Pin_0   //数据/命令选择
#define RW GPIO_Pin_1   //读/写选择
#define E GPIO_Pin_2    //使能信号
#define RST GPIO_Pin_3    //复位引脚

#define LCD_DATA GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7  //数据线

#define uchar unsigned char
	
//引脚初始化
void lcd12864_Pin_Init(void);
//DB7引脚  忙信号检测
void LCD_ReadyWait(void);
//向LCD12864写入命令
void LCD_Write_com(uchar com);
//向LCD12864写入数据
void LCD_Write_data(uchar data); 
//清屏
void LCD_clear(void);
//led12864并初始化
void LED_Init(void);

//任意位置显示字符串(写DDRAM中的数据)
void display_DDRAM(uchar x,uchar y,uchar *s);
//任意位置显示自定义字符串(写CGRAM中的数据)
void display_CGRAM(uchar y,uchar x,uchar addr,uchar *s);

/*----------------------------------12864串行发送----------------------------------------------------*/

#define WRITE_CMD	0xF8//写命令  
#define WRITE_DAT	0xFA//写数据

/*
* PSB引脚接低，串行模式
* RS引脚，拉高数据才能发送
*/
//RW引脚为PA1,E引脚为PA2

//清屏
void LCD_Clear(void);
//led串行发送一个字节
void SendByte(u8 byte);
//写命令
void Lcd_WriteCmd(u8 Cmd );
//写数据
void Lcd_WriteData(u8 Dat );
//显示字符或汉字
void LCD_Display_Words(uint8_t x,uint8_t y,uint8_t*str);
//led12864串行初始化
void Lcd_Init(void);
void ShowNumChar(uint8_t addr, uint8_t i, uint8_t count);



