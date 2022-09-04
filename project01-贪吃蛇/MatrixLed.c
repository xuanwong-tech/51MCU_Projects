#include <REGX52.H>
#include "Delay.h"
#include "Timer0.h"

#define MATRIX_LED_PORT		P0

unsigned char column;	//将要显示数据的列，范围0-7
unsigned char showData;	//将要显示的数据，高位在前，置1为亮

//将74HC595各引脚重新声明
sbit RCK = P3^5;	//RCLK
sbit SCK = P3^6;	//SRCLK
sbit SER = P3^4;	//SER

/**
  * @brief	LED点阵屏初始化函数
  * @param	无
  * @retval	无
  */
void MatrixLed_Init()
{
	SCK = 0;
	RCK = 0;
}


/**
  * @brief	向74HC595写入字节
  * @param	byte	写入的一个字节，1选中0不选中
  * @retval	无
  */
void _74HC595_WriteByte(unsigned char byte)
{
	unsigned char i;
	
	for (i = 0; i < 8; i++)
	{
		SER = byte & (0x80 >> i);	//从高到低取位
		SCK = 1;					//SRCLK上升沿，进行移位
		SCK = 0;					//操作结束，置0等待
	}
	
	RCK = 1;	//上升沿锁存一个寄存器
	RCK = 0;	//结束置0
}

/**
  * @brief	LED点阵屏显示函数
  * @param	column		列
  * @param	showData	列要显示的数据
  * @retval	无
  */
void MatrixLed_ShowData(unsigned char column, showData)
{
	_74HC595_WriteByte(showData);
	MATRIX_LED_PORT =~ (0x80>>column);
	Delay(1);
	MATRIX_LED_PORT = 0xFF;
}

/**
  * @brief	将(x, y)坐标解码为具体的行与列
  * @param	x	蛇的横坐标
  * @param	y	蛇的纵坐标
  * @retval	无
  */
void MatrixLed_translate(int x, int y)
{
	column = x;
	switch(y)
	{
		case 0: showData = 0x01; break;
		case 1: showData = 0x02; break;
		case 2: showData = 0x04; break;
		case 3: showData = 0x08; break;
		case 4: showData = 0x10; break;
		case 5: showData = 0x20; break;
		case 6: showData = 0x40; break;
		case 7: showData = 0x80; break;
	}
	MatrixLed_ShowData(column, showData);
}