#include "sys.h"

/*----------------------------------12864���з���----------------------------------------------------*/
#define RS GPIO_Pin_0   //����/����ѡ��
#define RW GPIO_Pin_1   //��/дѡ��
#define E GPIO_Pin_2    //ʹ���ź�
#define RST GPIO_Pin_3    //��λ����

#define LCD_DATA GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7  //������

#define uchar unsigned char
	
//���ų�ʼ��
void lcd12864_Pin_Init(void);
//DB7����  æ�źż��
void LCD_ReadyWait(void);
//��LCD12864д������
void LCD_Write_com(uchar com);
//��LCD12864д������
void LCD_Write_data(uchar data); 
//����
void LCD_clear(void);
//led12864����ʼ��
void LED_Init(void);

//����λ����ʾ�ַ���(дDDRAM�е�����)
void display_DDRAM(uchar x,uchar y,uchar *s);
//����λ����ʾ�Զ����ַ���(дCGRAM�е�����)
void display_CGRAM(uchar y,uchar x,uchar addr,uchar *s);

/*----------------------------------12864���з���----------------------------------------------------*/

#define WRITE_CMD	0xF8//д����  
#define WRITE_DAT	0xFA//д����

/*
* PSB���Žӵͣ�����ģʽ
* RS���ţ��������ݲ��ܷ���
*/
//RW����ΪPA1,E����ΪPA2

//����
void LCD_Clear(void);
//led���з���һ���ֽ�
void SendByte(u8 byte);
//д����
void Lcd_WriteCmd(u8 Cmd );
//д����
void Lcd_WriteData(u8 Dat );
//��ʾ�ַ�����
void LCD_Display_Words(uint8_t x,uint8_t y,uint8_t*str);
//led12864���г�ʼ��
void Lcd_Init(void);
void ShowNumChar(uint8_t addr, uint8_t i, uint8_t count);



