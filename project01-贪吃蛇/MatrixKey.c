#include <REGX52.H>
#include "Delay.h"

/**
  * @brief	获取矩阵键盘键码值
  * @param	无
  * @retval	keyNumber 按下按键的键码值
  如果按键按下不放，程序会阻塞在此函数，松手的一瞬间返回键码
  */
unsigned char MatrixKey()
{
	unsigned char keyNumber = 0;
	
	//扫描第一列，根据原理图，P1_3置0
	P1 = 0xFF;
	P1_3 = 0;
	if (P1_7 == 0) {Delay(20); while(P1_7 == 0); Delay(20); keyNumber = 1;}  //第一行
	if (P1_6 == 0) {Delay(20); while(P1_6 == 0); Delay(20); keyNumber = 5;}  //第二行
	if (P1_5 == 0) {Delay(20); while(P1_5 == 0); Delay(20); keyNumber = 9;}  //第三行
	if (P1_4 == 0) {Delay(20); while(P1_4 == 0); Delay(20); keyNumber = 13;}  //第四行
	
	//扫描第二列，根据原理图，P1_2置0
	P1 = 0xFF;
	P1_2 = 0;
	if (P1_7 == 0) {Delay(20); while(P1_7 == 0); Delay(20); keyNumber = 2;}  //第一行
	if (P1_6 == 0) {Delay(20); while(P1_6 == 0); Delay(20); keyNumber = 6;}  //第二行
	if (P1_5 == 0) {keyNumber = 10;}  //第三行
	if (P1_4 == 0) {Delay(20); while(P1_4 == 0); Delay(20); keyNumber = 14;}  //第四行
	
	//扫描第三列，根据原理图，P1_1置0
	P1 = 0xFF;
	P1_1 = 0;
	if (P1_7 == 0) {Delay(20); while(P1_7 == 0); Delay(20); keyNumber = 3;}  //第一行
	if (P1_6 == 0) {keyNumber = 7;}  //第二行
	if (P1_5 == 0) {Delay(20); while(P1_5 == 0); Delay(20); keyNumber = 11;}  //第三行
	if (P1_4 == 0) {keyNumber = 15;}  //第四行
	
	//扫描第四列，根据原理图，P1_0置0
	P1 = 0xFF;
	P1_0 = 0;
	if (P1_7 == 0) {Delay(20); while(P1_7 == 0); Delay(20); keyNumber = 4;}  //第一行
	if (P1_6 == 0) {Delay(20); while(P1_6 == 0); Delay(20); keyNumber = 8;}  //第二行
	if (P1_5 == 0) {keyNumber = 12;}  //第三行
	if (P1_4 == 0) {Delay(20); while(P1_4 == 0); Delay(20); keyNumber = 16;}  //第四行
	
	return keyNumber;
}