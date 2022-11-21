#include "lcd12864.h"
#include "delay.h"
#include "com_error.h"
#include "stdlib.h"

// doxygen
lcd_dev_node_t *lcd_dev_node_p;
int lcd_dev_node_count = 0;

int lcd_device_init(int count)
{
	  uint16_t i;
    lcd_dev_node_p = (lcd_dev_node_t *)malloc(count * sizeof(lcd_dev_node_t));
	  if(lcd_dev_node_p == NULL)
		{
        return COM_ERROR_NO_MEM;
    }
		for (i = 0; i < count; i++)
    {
			  lcd_dev_node_p[i].alloc_state = LCD_STATE_FREE;
        lcd_dev_node_p[i].SID_Pin = 0;
        lcd_dev_node_p[i].SID_Port = NULL;
        lcd_dev_node_p[i].CLK_Pin = 0;
        lcd_dev_node_p[i].CLK_Port = NULL;
			  lcd_dev_node_p[i].RST_Pin = 0;
			  lcd_dev_node_p[i].RST_Port = NULL;
    }
		lcd_dev_node_count = count;
		return COM_SUCCESS;
}

int lcd_device_create(lcd_device_id_t *id, uint16_t SID_Pin, GPIO_TypeDef * SID_Port, uint16_t CLK_Pin, GPIO_TypeDef * CLK_Port, uint16_t RST_Pin, GPIO_TypeDef * RST_Port)
{
	  if(NULL == id){
        return COM_ERROR_INVALID_STATE;
    }
    int i;
    for(i=0;i<lcd_dev_node_count;i++)
		{
        if(lcd_dev_node_p[i].alloc_state == LCD_STATE_FREE)
				{
            lcd_dev_node_p[i].alloc_state = LCD_ALLOCATED;
            lcd_dev_node_p[i].SID_Pin = SID_Pin;
            lcd_dev_node_p[i].SID_Port = SID_Port;
            lcd_dev_node_p[i].CLK_Pin = CLK_Pin;
            lcd_dev_node_p[i].CLK_Port = CLK_Port;
					  lcd_dev_node_p[i].RST_Pin = RST_Pin;
            lcd_dev_node_p[i].RST_Port = RST_Port;
            
            *id = i;
            return COM_SUCCESS;
        }
    }
    
    return COM_ERROR_NO_MEM;
}

void lcd_reset(lcd_device_id_t id)
{
    HAL_GPIO_WritePin(lcd_dev_node_p[id].RST_Port, lcd_dev_node_p[id].RST_Pin, GPIO_PIN_RESET);   
	  Delay_ms(300);
	  HAL_GPIO_WritePin(lcd_dev_node_p[id].RST_Port, lcd_dev_node_p[id].RST_Pin, GPIO_PIN_SET);
}

/* �ַ���ʾRAM��ַ4��8�� */
static uint8_t LCD_addr[4][8]={
	{0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87},  	//��һ��
	{0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97},		//�ڶ���
	{0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F},		//������
	{0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F}		//������
	};

//led���з���һ���ֽ�
void SendByte(lcd_device_id_t id, uint8_t byte)
{
	u8 i;
	for(i = 0; i< 8; i++)
	{
		HAL_GPIO_WritePin(lcd_dev_node_p[id].CLK_Port, lcd_dev_node_p[id].CLK_Pin, GPIO_PIN_RESET);
		if((byte <<i)&0x80)
		{
			HAL_GPIO_WritePin(lcd_dev_node_p[id].SID_Port, lcd_dev_node_p[id].SID_Pin, GPIO_PIN_SET);
			//GPIO_WriteBit(GPIOA,RW,Bit_SET);  //������ R/W=1
		}
		else
		{
			HAL_GPIO_WritePin(lcd_dev_node_p[id].SID_Port, lcd_dev_node_p[id].SID_Pin, GPIO_PIN_RESET);
			//GPIO_WriteBit(GPIOA,RW,Bit_RESET);  //д���� R/W=0		
		}
	
	//GPIO_WriteBit(GPIOA,E,Bit_RESET);    //��ʹ��
	Delay_us(5); //��ʱʹ����д��
	HAL_GPIO_WritePin(lcd_dev_node_p[id].CLK_Port, lcd_dev_node_p[id].CLK_Pin, GPIO_PIN_SET);
	//GPIO_WriteBit(GPIOA,E,Bit_SET);    //ʹ��	
	}
}

//д����
void Lcd_WriteCmd(lcd_device_id_t id, uint8_t Cmd)
{
     Delay_ms(1);         //��������û��дLCD��æ�ļ�⣬����ֱ����ʱ1ms��ʹÿ��д�����ݻ�ָ��������1ms ��ɲ���дæ״̬���
     SendByte(id, WRITE_CMD);  //11111,RW(0),RS(0),0   
     SendByte(id, 0xf0&Cmd);   //����λ
     SendByte(id, Cmd<<4);     //����λ(��ִ��<<)
}

//д����
void Lcd_WriteData(lcd_device_id_t id, uint8_t Dat)
{
     Delay_ms(1);             //��������û��дLCD��æ�ļ�⣬����ֱ����ʱ1ms��ʹÿ��д�����ݻ�ָ��������1ms ��ɲ���дæ״̬���
     SendByte(id, WRITE_DAT);      //11111,RW(0),RS(1),0
     SendByte(id, 0xf0&Dat);       //����λ
     SendByte(id, Dat<<4);         //����λ(��ִ��<<)
}

//����
void LCD_Clear(lcd_device_id_t id)
{
	Delay_ms(4);
	Lcd_WriteCmd(id, 0x01);	//����ָ��
	Delay_ms(4);				//��ʱ�Դ�Һ���ȶ�������1.6ms��
}
	
/*********************************************************** 
 *  �������� ��  ��ʾ�ַ�����
 *  ����     �� x: row(0~3)
 *              y: line(0~7) 
 *       	      str: Ҫ��ʾ���ַ�����
 ***********************************************************/
void LCD_Display_Words(lcd_device_id_t id, uint8_t x, uint8_t y, uint8_t*str)
{ 
	Lcd_WriteCmd(id, LCD_addr[x][y]); //д��ʼ���λ��
	while(*str>0)
  { 
      Lcd_WriteData(id, *str);      //д����
      str++;     
  }
}
/*
* lcd12864���г�ʼ��
*/
void Lcd_Init(lcd_device_id_t id)
{ 
    
	Delay_ms(50);   	//�ȴ�Һ���Լ죨��ʱ>40ms��
	Lcd_WriteCmd(id, 0x30);        //�����趨:ѡ�����ָ�  ��ѡ��8bit������
  Delay_ms(1);//��ʱ>137us 
	Lcd_WriteCmd(id, 0x06);        //ÿ�ε�ַ�Զ�+1����ʼ�����
	Delay_ms(1);
	Lcd_WriteCmd(id, 0x01);        //�����ʾ�������趨��ַָ��Ϊ00H
  Delay_ms(10);	//��ʱ>10ms
  Lcd_WriteCmd(id, 0x0c);        //����ʾ
  Delay_ms(1);	//��ʱ>100us
	Lcd_WriteCmd(id, 0x02);        //�����ʾ�������趨��ַָ��Ϊ00H
  Delay_ms(10);	//��ʱ>10ms
}

void ShowNumChar(lcd_device_id_t id, uint8_t addr, uint8_t i, uint8_t count)
{
    uint8_t j;
    for (j = 0; j < count;){
   
		    Lcd_WriteCmd(id, addr);
			  Lcd_WriteData(id, i + j);
			  j++;
			  Lcd_WriteData(id, i + j);
			  addr++;
			  j++;
		}
}


