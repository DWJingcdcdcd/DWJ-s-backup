#include "lcd12864.h"
#include "delay.h"

/***************************************************************************************
* 2020.11.20
* LED12864模块
* 管脚号  	管脚名称        	LEVER	         管脚功能描述
*  1	        VSS	          0	            电源地
*  2	        VDD	        +5.0V	          电源电压
*  3	        V0	          -	            液晶显示器驱动电压
*  4	        D/I(RS)    	H/L	            D/I=“H”    表示DB7∽DB0为显示数据
*                                         D/I=“L”        表示DB7∽DB0为显示指令数据
*  5	        R/W(SID)    H/L	            R/W=“H”，E=“H”数据被读到DB7∽DB0
*                                         R/W=“L”，E=“H→L”数据被写到IR或DR
*  6	        E(CLK)	    H/L	            R/W=“L”，E信号下降沿锁存DB7∽DB0
*                                         R/W=“H”，E=“H”DDRAM数据读到DB7∽DB0
*  7	        DB0	        H/L	            数据线
*  8	        DB1        	H/L	            数据线
*  9	        DB2	        H/L           	数据线
*  10	        DB3	        H/L            	数据线
*  11       	DB4       	H/L            	数据线
*  12	        DB5	        H/L	            数据线
*  13       	DB6	        H/L	            数据线
*  14        	DB7	        H/L	            数据线
*  15	        PSB(CS1)    H/L	            并/串行接口选择：H-并行 L-串行
*  16	        NC(CS2)	     	              空脚
*  17	        RST       	H/L	            复位信号,低电平复位
*  18	        VOUT	     -10V            	LCD驱动负电压
*  19	        LED+	      +              	LED背光板电源
*  20	        LED-	      -	              LED背光板电源
*********************************************************************************************/



/**************************************************
* 函数功能  ： 引脚初始化
**************************************************/
void lcd12864_Pin_Init(void)
{
;
}

/*************************************************
* 函数功能 ： LED12864数据口引脚配置为输出模式
**************************************************/
void LED12864_DB_outputMode(void)
{
;
}

/*************************************************
* 函数功能 ： LED12864数据口引脚配置为上拉输入模式
**************************************************/
void LED12864_DB_inputMode(void)
{
;

}

/*************************************************
* 函数功能 ： 写数据/命令
**************************************************/

/*void write_LCD12864_DB(uint8_t data)
{
	GPIO_Write(GPIOD,data);
}
*/
//
/*************************************************
* 函数功能 ： 读DB0-7的数据
**************************************************/
/*uint8_t read_LCD12864_DB(void)
{
	uint8_t value;
	value=GPIO_ReadInputData(GPIOD);
	return value;
}
*/
/*************************************************
* 函数功能 ： DB7引脚  忙信号检测
**************************************************/
/*void LCD_ReadyWait(void)
{
	uint8_t state;
	
	delay_ms(10);
	GPIO_WriteBit(GPIOA,RS,Bit_RESET);  //写指令  RS=0
	GPIO_WriteBit(GPIOA,RW,Bit_SET);  //写操作  R/W=1
	do
	{
		GPIO_WriteBit(GPIOA,E,Bit_SET);    //使能    E=1
		LED12864_DB_inputMode();     //配置数据口为上拉输入模式	
		state =read_LCD12864_DB();   //查忙标志位，等待标志位为0，表示写入完毕；为1，表示忙状态	
		LED12864_DB_outputMode();    	//配置数据口为输出模式
		GPIO_WriteBit(GPIOA,E,Bit_RESET);    //不使能    E=0
	}	while( state & 0x80);  
	
}
*/
/*************************************************
* 函数功能 ： 向LCD12864写入命令
* 函数参数 ：com写入的命令
**************************************************/
/*void LCD_Write_com(uchar com)
{
	delay_ms(5);
	LCD_ReadyWait();
	delay_ms(10);
	GPIO_WriteBit(GPIOA,RS,Bit_RESET);  //写指令  RS=0
	GPIO_WriteBit(GPIOA,RW,Bit_RESET);  //写操作  R/W=0
	GPIO_WriteBit(GPIOA,E,Bit_SET);    //使能    E=高电平
	write_LCD12864_DB(com);              //写命令
	GPIO_WriteBit(GPIOA,E,Bit_RESET);    //不使能
	


}
*/
/*************************************************
* 函数功能 ： 向LCD12864写入数据
* 函数参数 ：data写入的数据
**************************************************/
/*void LCD_Write_data(uchar data)
{
	delay_ms(10);
	GPIO_WriteBit(GPIOA,RS,Bit_SET);  //写数据   RS=1
	GPIO_WriteBit(GPIOA,RW,Bit_RESET);  //写操作 R/W=0
	GPIO_WriteBit(GPIOA,E,Bit_SET);    //使能   E=高电平
	write_LCD12864_DB(data);            //写数据
	GPIO_WriteBit(GPIOA,E,Bit_RESET);    //不使能
}
*/
/*************************************************
* 函数功能 : 清屏
*************************************************/
/*void LCD_clear(void)
{
	LCD_Write_com(0x01);    //写清屏指令0x01  
  delay_ms(10);
}
*/
/*************************************************
* 函数功能 : 任意位置显示字符串(写DDRAM中的数据)
* 函数参数 ：x第几列，y第几行，s输入的数据
*************************************************/
/*void display_DDRAM(uchar x,uchar y,uchar *s)
{
	switch(y)
	{
		case 0:  
			LCD_Write_com(0x80+x);     //第一行
		break;
		case 1:  
			LCD_Write_com(0x90+x);     //第二行
		break;
		case 2:  
			LCD_Write_com(0x88+x);     //第三行
		break;
		case 3:  
			LCD_Write_com(0x98+x);     //第四行
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
//0x51,0x84,0x21,0x00};    //自定义字符数组，这里是张
 

/*************************************************
* 函数功能 : 任意位置显示自定义字符串(写CGRAM中的数据)
* 函数参数 ：x第几列，y第几行，addr 自编字符编码首地址，s输入的数据
* 声明     ：12864具有4个自编字符，每个字符编码为0000H，0002H,0004H,0006H，4个自定义字符	
* CGRAM地址分别为00H~0FH、10H~1FH、20H~2FH、30H~3FH
*************************************************/
/*void display_CGRAM(uchar y,uchar x,uchar addr,uchar *s)
{
	unsigned char i,*address;
	address = s;   
	LCD_Write_com(0x40+addr);    //写CGRAM首行地址,创建第几个自编字符就写第几个自编字符的地址
	for(i = 0;i<32;i++)
	{
		LCD_Write_data(*address++);
	}
	switch(y)
	{
		case 0:  
			LCD_Write_com(0x80+x);     //第一行
		break;
		case 1:  
			LCD_Write_com(0x90+x);    //第二行
		break;
		case 2:  
			LCD_Write_com(0x88+x);    //第三行
		break;
		case 3:  
			LCD_Write_com(0x98+x);   //第四行
		break;	
	}
	LCD_Write_data(0x00);     //写自定义字符编码的高字节
	
	switch(addr)
	{
		case 0x00:  
			LCD_Write_data(0x00);	   //写第一个自定义字符编码的低字节
		break;
		case 0x10:  
			LCD_Write_data(0x02);     //写第二个自定义字符编码的低字节
		break;
		case 0x20:  
			LCD_Write_data(0x04);     //写第三个自定义字符编码的低字节
		break;
		case 0x30:  
			LCD_Write_data(0x06);     //写第四个自定义字符编码的低字节
		break;	
	}
}


//led12864并行初始化
void LED_Init(void)
{
	GPIO_WriteBit(GPIOA,RST,Bit_SET);    //复位置高
	delay_ms(10);	
	GPIO_WriteBit(GPIOA,RST,Bit_RESET);  //复位
	delay_ms(10);	
	GPIO_WriteBit(GPIOA,RST,Bit_SET);    //复位结束
	delay_ms(10);	
	
	LCD_Write_com(0x30);    //设置为基本指令集动作
	delay_ms(10);
	LCD_Write_com(0x08);    //设置显示、光标、闪烁全关。
	
	LCD_Write_com(0x01);      //清屏，并且DDRAM数据指针清零
	LCD_Write_com(0x06);      //进入模式设置
	LCD_Write_com(0x0c);      //开显示
	delay_ms(10);	

}
*/

/*----------------------------------12864串行发送数据----------------------------------------------------*/
/*
* PSB引脚接低，串行模式
* RS引脚，拉高数据才能发送
*/
/*---------串行----------------*/
/* 字符显示RAM地址4行8列 */
uint8_t LCD_addr[4][8]={
	{0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87},  	//第一行
	{0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97},		//第二行
	{0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F},		//第三行
	{0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F}		//第四行
	};

//led串行发送一个字节
void SendByte(u8 byte)
{
	u8 i;
	for(i = 0; i< 8; i++)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
		if((byte <<i)&0x80)
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
			//GPIO_WriteBit(GPIOA,RW,Bit_SET);  //读操作 R/W=1
		}
		else
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
			//GPIO_WriteBit(GPIOA,RW,Bit_RESET);  //写操作 R/W=0		
		}
	
	//GPIO_WriteBit(GPIOA,E,Bit_RESET);    //不使能
	Delay_us(50); //延时使数据写入
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
	//GPIO_WriteBit(GPIOA,E,Bit_SET);    //使能	
	}
}

//写命令
void Lcd_WriteCmd(u8 Cmd )
{
     Delay_ms(1);         //由于我们没有写LCD正忙的检测，所以直接延时1ms，使每次写入数据或指令间隔大于1ms 便可不用写忙状态检测
     SendByte(WRITE_CMD);  //11111,RW(0),RS(0),0   
     SendByte(0xf0&Cmd);   //高四位
     SendByte(Cmd<<4);     //低四位(先执行<<)
}

//写数据
void Lcd_WriteData(u8 Dat )
{
     Delay_ms(1);             //由于我们没有写LCD正忙的检测，所以直接延时1ms，使每次写入数据或指令间隔大于1ms 便可不用写忙状态检测
     SendByte(WRITE_DAT);      //11111,RW(0),RS(1),0
     SendByte(0xf0&Dat);       //高四位
     SendByte(Dat<<4);         //低四位(先执行<<)
}

//清屏
void LCD_Clear(void)
{
	Delay_ms(4);
	Lcd_WriteCmd(0x01);	//清屏指令
	Delay_ms(4);				//延时以待液晶稳定【至少1.6ms】
}
	
/*********************************************************** 
 *  函数功能 ：  显示字符或汉字
 *  参数     ： x: row(0~3)
 *              y: line(0~7) 
 *       	      str: 要显示的字符或汉字
 ***********************************************************/
void LCD_Display_Words(uint8_t x,uint8_t y,uint8_t*str)
{ 
	Lcd_WriteCmd(LCD_addr[x][y]); //写初始光标位置
	while(*str>0)
  { 
      Lcd_WriteData(*str);      //写数据
      str++;     
  }
}
/*
* lcd12864串行初始化
*/
void Lcd_Init(void)
{ 
    
	Delay_ms(500);   	//等待液晶自检（延时>40ms）
	Lcd_WriteCmd(0x30);        //功能设定:选择基本指令集  ，选择8bit数据流
  Delay_ms(10);//延时>137us 
	Lcd_WriteCmd(0x06);        //每次地址自动+1，初始化完成
	Delay_ms(10);
	Lcd_WriteCmd(0x01);        //清除显示，并且设定地址指针为00H
  Delay_ms(100);	//延时>10ms
  Lcd_WriteCmd(0x0c);        //开显示
  Delay_ms(10);	//延时>100us
	Lcd_WriteCmd(0x02);        //清除显示，并且设定地址指针为00H
  Delay_ms(100);	//延时>10ms
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


