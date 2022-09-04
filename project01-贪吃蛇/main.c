#include <REGX52.H>
#include "MatrixLed.h"
#include "MatrixKey.h"
#include "Delay.h"
#include "Timer0.h"

void Food_Produce();
void Food_Eat();
void Body_Init();
void Body_Death();
void Body_Move();
void Body_Switch();

unsigned int mode = 12;											//定义行走模式
int head_x = 0, head_y = 0;										//初始化蛇头的位置
int length = 1;													//初始化蛇身长度
int body_x[14] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};	//初始化蛇身横向坐标
int body_y[14] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};	//初始化蛇身纵向坐标
int food_x, food_y;												//初始化食物坐标
int random_location[2] = {3, 7};								//初始化随机坐标
int dead;														//初始化蛇身死亡遍历索引
int o, p;														//初始化蛇身变化计数循环索引
unsigned char keyNum;											//初始化矩阵按键键码
int i;															//初始化main函数内循环索引

void main()
{
	Timer0_Init();		//定时器初始化，用于蛇头更新
	MatrixLed_Init();	//LED点阵屏初始化
	Food_Produce();		//产生食物
	
	while (1)
	{
		if ((mode != 10) && (keyNum == 10)) mode = 10;			//根据键码以及当前模式判定是否需要改变模式
		if ((mode != 12) && (keyNum == 12)) mode = 12;
		if ((mode != 15) && (keyNum == 15)) mode = 15;
		if ((mode != 7) && (keyNum == 7)) mode = 7;
		
		MatrixLed_translate(head_x, head_y);					//绘制蛇头及食物的坐标
		MatrixLed_translate(food_x, food_y);
		
		Food_Eat();												//检测食物是否被吃
		
		//根据length绘制蛇身
		//思想：长度为1时，绘制蛇头
		switch(length)		
		{			
			case 1: MatrixLed_translate(body_x[0], body_y[0]); break;
			case 2: for (i = 0; i < 2; i++) {MatrixLed_translate(body_x[i], body_y[i]);} break;
			case 3: for (i = 0; i < 3; i++) {MatrixLed_translate(body_x[i], body_y[i]);} break;
			case 4: for (i = 0; i < 5; i++) {MatrixLed_translate(body_x[i], body_y[i]);} break;
			case 5: for (i = 0; i < 6; i++) {MatrixLed_translate(body_x[i], body_y[i]);} break;
			case 6: for (i = 0; i < 7; i++) {MatrixLed_translate(body_x[i], body_y[i]);} break;
			case 7: for (i = 0; i < 8; i++) {MatrixLed_translate(body_x[i], body_y[i]);} break;
			case 8: for (i = 0; i < 9; i++) {MatrixLed_translate(body_x[i], body_y[i]);} break;
			case 9: for (i = 0; i < 10; i++) {MatrixLed_translate(body_x[i], body_y[i]);} break;
			case 10: for (i = 0; i < 11; i++) {MatrixLed_translate(body_x[i], body_y[i]);} break;
			case 11: for (i = 0; i < 12; i++) {MatrixLed_translate(body_x[i], body_y[i]);} break;
			case 12: for (i = 0; i < 13; i++) {MatrixLed_translate(body_x[i], body_y[i]);} break;
			case 13: for (i = 0; i < 14; i++) {MatrixLed_translate(body_x[i], body_y[i]);} break;
			case 14: for (i = 0; i < 15; i++) {MatrixLed_translate(body_x[i], body_y[i]);} break;
		}
	}
}

/**
  * @brief	食物产生
  * @param	无
  * @retval	无
  */
void Food_Produce()
{
	food_x = random_location[0] % 10;
	if (random_location[0] % 10 >= 8)
		food_x = 5;
	
	food_y = random_location[1] % 10;
	if (random_location[1] % 10 >= 8)
		food_y = 7;
	
	random_location[0] += 3;
	random_location[1] += 7;
}

/**
  * @brief	食物被吃掉
  * @param	无
  * @retval	无
  */
void Food_Eat()
{
	if ((head_x == food_x) && (head_y == food_y))
	{
		food_x = 8;
		food_y = 8;
		Food_Produce();
		length ++;
	}
}

/**
  * @brief	蛇身初始化
  * @param	无
  * @retval	无
  */
void Body_Init()
{
	int x;
	for (x = 0; x < 15; x++)
	{
		body_x[x] = -1;
		body_y[x] = -1;
	}
}

/**
  * @brief	蛇身死亡
  * @param	无
  * @retval	无
  */
void Body_Death()
{
	for (dead = 0; dead < 12; dead++)
	{
		if ((head_x == body_x[dead]) && (head_y == body_y[dead]))
		{
			length = 1;
			Body_Init();
		}
	}
}

/**
  * @brief	蛇身行走模式的切换
  * @param	无
  * @retval	无
  */
void Body_Move()
{
	switch (mode)
	{
		case 10: head_x--; if (head_x <= -1) head_x = 7; break;		//左
		case 12: head_x++; if (head_x >= 8) head_x = 0; break;		//右
		case 7: head_y++; if (head_y >= 8) head_y = 0; break;		//上
		case 15: head_y--; if (head_y <= -1) head_y = 7; break;		//下
		default: break;
	}
}

/**
  * @brief	蛇身的变化
  * @param	无
  * @retval	无
  */
void Body_Switch()
{
	switch (length)
	{
		case 1: body_x[0] = head_x; body_y[0] = head_y; break;
		case 2:	body_x[1] = body_x[0]; body_y[1] = body_y[0];
					body_x[0] = head_x; body_y[0] = head_y; break;
		case 3: for (o = 2; o > -1; o--) {p = o + 1; body_x[p] = body_x[o]; body_y[p] = body_y[o]; }
					body_x[0] = head_x; body_y[0] = head_y; break;
		case 4: for (o = 3; o > -1; o--) {p = o + 1; body_x[p] = body_x[o]; body_y[p] = body_y[o]; }
					body_x[0] = head_x; body_y[0] = head_y; break;
		case 5: for (o = 4; o > -1; o--) {p = o + 1; body_x[p] = body_x[o]; body_y[p] = body_y[o]; }
					body_x[0] = head_x; body_y[0] = head_y; break;
		case 6: for (o = 5; o > -1; o--) {p = o + 1; body_x[p] = body_x[o]; body_y[p] = body_y[o]; }
					body_x[0] = head_x; body_y[0] = head_y; break;
		case 7: for (o = 6; o > -1; o--) {p = o + 1; body_x[p] = body_x[o]; body_y[p] = body_y[o]; }
					body_x[0] = head_x; body_y[0] = head_y; break;
		case 8: for (o = 7; o > -1; o--) {p = o + 1; body_x[p] = body_x[o]; body_y[p] = body_y[o]; }
					body_x[0] = head_x; body_y[0] = head_y; break;
		case 9: for (o = 8; o > -1; o--) {p = o + 1; body_x[p] = body_x[o]; body_y[p] = body_y[o]; }
					body_x[0] = head_x; body_y[0] = head_y; break;
		case 10: for (o = 9; o > -1; o--) {p = o + 1; body_x[p] = body_x[o]; body_y[p] = body_y[o]; }
					body_x[0] = head_x; body_y[0] = head_y; break;
		case 11: for (o = 10; o > -1; o--) {p = o + 1; body_x[p] = body_x[o]; body_y[p] = body_y[o]; }
					body_x[0] = head_x; body_y[0] = head_y; break;
		case 12: for (o = 11; o > -1; o--) {p = o + 1; body_x[p] = body_x[o]; body_y[p] = body_y[o]; }
					body_x[0] = head_x; body_y[0] = head_y; break;
		case 13: for (o = 12; o > -1; o--) {p = o + 1; body_x[p] = body_x[o]; body_y[p] = body_y[o]; }
					body_x[0] = head_x; body_y[0] = head_y; break;
		case 14: for (o = 13; o > -1; o--) {p = o + 1; body_x[p] = body_x[o]; body_y[p] = body_y[o]; }
					body_x[0] = head_x; body_y[0] = head_y; break;
		default: length = 1;
	}
}

void Timer0_Routine()	interrupt 1
{
	static unsigned int count0, count1;
	
	TL0 = 0x66;			
	TH0 = 0xFC;
	count0 ++;
	count1 ++;
	
	if (count0 >= 100)
	{
		count0 = 0;
		keyNum = MatrixKey();
	}
	
	if (count1 >= 400)
	{
		count1 = 0;
		P2_0 =~ P2_0;
		Body_Switch();
		Body_Move();
		Body_Death();
	}
}