#include "lcd12864.h"
#include "delay.h"

/***************************************************************************************
* 2020.11.20
* LED12864ģ��
* �ܽź�  	�ܽ�����        	LEVER	         �ܽŹ�������
*  1	        VSS	          0	            ��Դ��
*  2	        VDD	        +5.0V	          ��Դ��ѹ
*  3	        V0	          -	            Һ����ʾ��������ѹ
*  4	        D/I(RS)    	H/L	            D/I=��H��    ��ʾDB7��DB0Ϊ��ʾ����
*                                         D/I=��L��        ��ʾDB7��DB0Ϊ��ʾָ������
*  5	        R/W(SID)    H/L	            R/W=��H����E=��H�����ݱ�����DB7��DB0
*                                         R/W=��L����E=��H��L�����ݱ�д��IR��DR
*  6	        E(CLK)	    H/L	            R/W=��L����E�ź��½�������DB7��DB0
*                                         R/W=��H����E=��H��DDRAM���ݶ���DB7��DB0
*  7	        DB0	        H/L	            ������
*  8	        DB1        	H/L	            ������
*  9	        DB2	        H/L           	������
*  10	        DB3	        H/L            	������
*  11       	DB4       	H/L            	������
*  12	        DB5	        H/L	            ������
*  13       	DB6	        H/L	            ������
*  14        	DB7	        H/L	            ������
*  15	        PSB(CS1)    H/L	            ��/���нӿ�ѡ��H-���� L-����
*  16	        NC(CS2)	     	              �ս�
*  17	        RST       	H/L	            ��λ�ź�,�͵�ƽ��λ
*  18	        VOUT	     -10V            	LCD��������ѹ
*  19	        LED+	      +              	LED������Դ
*  20	        LED-	      -	              LED������Դ
*********************************************************************************************/



/**************************************************
* ��������  �� ���ų�ʼ��
**************************************************/
void lcd12864_Pin_Init(void)
{
;
}

/*************************************************
* �������� �� LED12864���ݿ���������Ϊ���ģʽ
**************************************************/
void LED12864_DB_outputMode(void)
{
;
}

/*************************************************
* �������� �� LED12864���ݿ���������Ϊ��������ģʽ
**************************************************/
void LED12864_DB_inputMode(void)
{
;

}

/*************************************************
* �������� �� д����/����
**************************************************/

/*void write_LCD12864_DB(uint8_t data)
{
	GPIO_Write(GPIOD,data);
}
*/
//
/*************************************************
* �������� �� ��DB0-7������
**************************************************/
/*uint8_t read_LCD12864_DB(void)
{
	uint8_t value;
	value=GPIO_ReadInputData(GPIOD);
	return value;
}
*/
/*************************************************
* �������� �� DB7����  æ�źż��
**************************************************/
/*void LCD_ReadyWait(void)
{
	uint8_t state;
	
	delay_ms(10);
	GPIO_WriteBit(GPIOA,RS,Bit_RESET);  //дָ��  RS=0
	GPIO_WriteBit(GPIOA,RW,Bit_SET);  //д����  R/W=1
	do
	{
		GPIO_WriteBit(GPIOA,E,Bit_SET);    //ʹ��    E=1
		LED12864_DB_inputMode();     //�������ݿ�Ϊ��������ģʽ	
		state =read_LCD12864_DB();   //��æ��־λ���ȴ���־λΪ0����ʾд����ϣ�Ϊ1����ʾæ״̬	
		LED12864_DB_outputMode();    	//�������ݿ�Ϊ���ģʽ
		GPIO_WriteBit(GPIOA,E,Bit_RESET);    //��ʹ��    E=0
	}	while( state & 0x80);  
	
}
*/
/*************************************************
* �������� �� ��LCD12864д������
* �������� ��comд�������
**************************************************/
/*void LCD_Write_com(uchar com)
{
	delay_ms(5);
	LCD_ReadyWait();
	delay_ms(10);
	GPIO_WriteBit(GPIOA,RS,Bit_RESET);  //дָ��  RS=0
	GPIO_WriteBit(GPIOA,RW,Bit_RESET);  //д����  R/W=0
	GPIO_WriteBit(GPIOA,E,Bit_SET);    //ʹ��    E=�ߵ�ƽ
	write_LCD12864_DB(com);              //д����
	GPIO_WriteBit(GPIOA,E,Bit_RESET);    //��ʹ��
	


}
*/
/*************************************************
* �������� �� ��LCD12864д������
* �������� ��dataд�������
**************************************************/
/*void LCD_Write_data(uchar data)
{
	delay_ms(10);
	GPIO_WriteBit(GPIOA,RS,Bit_SET);  //д����   RS=1
	GPIO_WriteBit(GPIOA,RW,Bit_RESET);  //д���� R/W=0
	GPIO_WriteBit(GPIOA,E,Bit_SET);    //ʹ��   E=�ߵ�ƽ
	write_LCD12864_DB(data);            //д����
	GPIO_WriteBit(GPIOA,E,Bit_RESET);    //��ʹ��
}
*/
/*************************************************
* �������� : ����
*************************************************/
/*void LCD_clear(void)
{
	LCD_Write_com(0x01);    //д����ָ��0x01  
  delay_ms(10);
}
*/
/*************************************************
* �������� : ����λ����ʾ�ַ���(дDDRAM�е�����)
* �������� ��x�ڼ��У�y�ڼ��У�s���������
*************************************************/
/*void display_DDRAM(uchar x,uchar y,uchar *s)
{
	switch(y)
	{
		case 0:  
			LCD_Write_com(0x80+x);     //��һ��
		break;
		case 1:  
			LCD_Write_com(0x90+x);     //�ڶ���
		break;
		case 2:  
			LCD_Write_com(0x88+x);     //������
		break;
		case 3:  
			LCD_Write_com(0x98+x);     //������
		break;	
	}
	while(*s>0)
	{
		LCD_Write_data(*s++);
		delay_ms(10);	
	}
}
*/
//unsigned char defchar[] = {0x09,0x00,0xFD,0x08,0x09,0x08,0x09,
//0x10,0x09,0x20,0x79,0x40,0x41,0x04,
//0x47,0xFE,0x41,0x40,0x79,0x40,0x09,
//0x20,0x09,0x20,0x09,0x10,0x09,0x4E,
//0x51,0x84,0x21,0x00};    //�Զ����ַ����飬��������
 

/*************************************************
* �������� : ����λ����ʾ�Զ����ַ���(дCGRAM�е�����)
* �������� ��x�ڼ��У�y�ڼ��У�addr �Ա��ַ������׵�ַ��s���������
* ����     ��12864����4���Ա��ַ���ÿ���ַ�����Ϊ0000H��0002H,0004H,0006H��4���Զ����ַ�	
* CGRAM��ַ�ֱ�Ϊ00H~0FH��10H~1FH��20H~2FH��30H~3FH
*************************************************/
/*void display_CGRAM(uchar y,uchar x,uchar addr,uchar *s)
{
	unsigned char i,*address;
	address = s;   
	LCD_Write_com(0x40+addr);    //дCGRAM���е�ַ,�����ڼ����Ա��ַ���д�ڼ����Ա��ַ��ĵ�ַ
	for(i = 0;i<32;i++)
	{
		LCD_Write_data(*address++);
	}
	switch(y)
	{
		case 0:  
			LCD_Write_com(0x80+x);     //��һ��
		break;
		case 1:  
			LCD_Write_com(0x90+x);    //�ڶ���
		break;
		case 2:  
			LCD_Write_com(0x88+x);    //������
		break;
		case 3:  
			LCD_Write_com(0x98+x);   //������
		break;	
	}
	LCD_Write_data(0x00);     //д�Զ����ַ�����ĸ��ֽ�
	
	switch(addr)
	{
		case 0x00:  
			LCD_Write_data(0x00);	   //д��һ���Զ����ַ�����ĵ��ֽ�
		break;
		case 0x10:  
			LCD_Write_data(0x02);     //д�ڶ����Զ����ַ�����ĵ��ֽ�
		break;
		case 0x20:  
			LCD_Write_data(0x04);     //д�������Զ����ַ�����ĵ��ֽ�
		break;
		case 0x30:  
			LCD_Write_data(0x06);     //д���ĸ��Զ����ַ�����ĵ��ֽ�
		break;	
	}
}


//led12864���г�ʼ��
void LED_Init(void)
{
	GPIO_WriteBit(GPIOA,RST,Bit_SET);    //��λ�ø�
	delay_ms(10);	
	GPIO_WriteBit(GPIOA,RST,Bit_RESET);  //��λ
	delay_ms(10);	
	GPIO_WriteBit(GPIOA,RST,Bit_SET);    //��λ����
	delay_ms(10);	
	
	LCD_Write_com(0x30);    //����Ϊ����ָ�����
	delay_ms(10);
	LCD_Write_com(0x08);    //������ʾ����ꡢ��˸ȫ�ء�
	
	LCD_Write_com(0x01);      //����������DDRAM����ָ������
	LCD_Write_com(0x06);      //����ģʽ����
	LCD_Write_com(0x0c);      //����ʾ
	delay_ms(10);	

}
*/

/*----------------------------------12864���з�������----------------------------------------------------*/
/*
* PSB���Žӵͣ�����ģʽ
* RS���ţ��������ݲ��ܷ���
*/
/*---------����----------------*/
/* �ַ���ʾRAM��ַ4��8�� */
uint8_t LCD_addr[4][8]={
	{0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87},  	//��һ��
	{0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97},		//�ڶ���
	{0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F},		//������
	{0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F}		//������
	};

//led���з���һ���ֽ�
void SendByte(u8 byte)
{
	u8 i;
	for(i = 0; i< 8; i++)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
		if((byte <<i)&0x80)
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
			//GPIO_WriteBit(GPIOA,RW,Bit_SET);  //������ R/W=1
		}
		else
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
			//GPIO_WriteBit(GPIOA,RW,Bit_RESET);  //д���� R/W=0		
		}
	
	//GPIO_WriteBit(GPIOA,E,Bit_RESET);    //��ʹ��
	Delay_us(50); //��ʱʹ����д��
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
	//GPIO_WriteBit(GPIOA,E,Bit_SET);    //ʹ��	
	}
}

//д����
void Lcd_WriteCmd(u8 Cmd )
{
     Delay_ms(1);         //��������û��дLCD��æ�ļ�⣬����ֱ����ʱ1ms��ʹÿ��д�����ݻ�ָ��������1ms ��ɲ���дæ״̬���
     SendByte(WRITE_CMD);  //11111,RW(0),RS(0),0   
     SendByte(0xf0&Cmd);   //����λ
     SendByte(Cmd<<4);     //����λ(��ִ��<<)
}

//д����
void Lcd_WriteData(u8 Dat )
{
     Delay_ms(1);             //��������û��дLCD��æ�ļ�⣬����ֱ����ʱ1ms��ʹÿ��д�����ݻ�ָ��������1ms ��ɲ���дæ״̬���
     SendByte(WRITE_DAT);      //11111,RW(0),RS(1),0
     SendByte(0xf0&Dat);       //����λ
     SendByte(Dat<<4);         //����λ(��ִ��<<)
}

//����
void LCD_Clear(void)
{
	Delay_ms(4);
	Lcd_WriteCmd(0x01);	//����ָ��
	Delay_ms(4);				//��ʱ�Դ�Һ���ȶ�������1.6ms��
}
	
/*********************************************************** 
 *  �������� ��  ��ʾ�ַ�����
 *  ����     �� x: row(0~3)
 *              y: line(0~7) 
 *       	      str: Ҫ��ʾ���ַ�����
 ***********************************************************/
void LCD_Display_Words(uint8_t x,uint8_t y,uint8_t*str)
{ 
	Lcd_WriteCmd(LCD_addr[x][y]); //д��ʼ���λ��
	while(*str>0)
  { 
      Lcd_WriteData(*str);      //д����
      str++;     
  }
}
/*
* lcd12864���г�ʼ��
*/
void Lcd_Init(void)
{ 
    
	Delay_ms(500);   	//�ȴ�Һ���Լ죨��ʱ>40ms��
	Lcd_WriteCmd(0x30);        //�����趨:ѡ�����ָ�  ��ѡ��8bit������
  Delay_ms(10);//��ʱ>137us 
	Lcd_WriteCmd(0x06);        //ÿ�ε�ַ�Զ�+1����ʼ�����
	Delay_ms(10);
	Lcd_WriteCmd(0x01);        //�����ʾ�������趨��ַָ��Ϊ00H
  Delay_ms(100);	//��ʱ>10ms
  Lcd_WriteCmd(0x0c);        //����ʾ
  Delay_ms(10);	//��ʱ>100us
	Lcd_WriteCmd(0x02);        //�����ʾ�������趨��ַָ��Ϊ00H
  Delay_ms(100);	//��ʱ>10ms
}

void ShowNumChar(uint8_t addr, uint8_t i, uint8_t count)
{
	  uint8_t j;
	  for (j = 0; j < count;)
	  {
		    Lcd_WriteCmd(addr);
			  Lcd_WriteData(i + j);
			  j++;
			  Lcd_WriteData(i + j);
			  addr++;
			  j++;
		}
}


