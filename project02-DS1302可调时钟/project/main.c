#include <REGX52.H>
#include "LCD1602.h"
#include "DS1302.h"
#include "Key.h"
#include "Timer0.h"

void TimeShow(void);
void TimeSet(void);

unsigned char keyNum;			//初始化独立按键键码
unsigned char mode;				//初始化时钟显示模式
unsigned char timeSetSelect;	//初始化时间修改索引，为DS1302.c中的DS1302_Time[]的数组索引
unsigned char timeSetFlashFlag;	//初始化时间修改刷新标志位

void main()
{
	LCD_Init();					//初始化LCD
	DS1302_Init();				//初始化DS1302
	Timer0_Init();				//初始化定时器0
	
	DS1302_SetTime();			//上电就立即开始设置好时钟
	
	LCD_ShowString(1, 3, "-");	//出于美观考虑，显示 "-"
	LCD_ShowString(1, 6, "-");
	LCD_ShowString(2, 3, "-");
	LCD_ShowString(2, 6, "-");
	
	while (1)
	{
		keyNum = Key();			//获取独立按键键码，按下K1-1，K2-2，K3-3，K4-4，不按返回0
		if (keyNum == 1)		//按键K1负责切换模式
		{
			if (mode == 1)
			{
				mode = 0;
				DS1302_SetTime();	//每次从修改模式切换至显示模式，就立即用修改值进行修改
			}
			else if (mode == 0)
				mode = 1;
		}		
		
		switch (mode)			//0为显示模式，1为修改模式
		{
			case 0: TimeShow(); break;
			case 1: TimeSet(); break;
		}
	}
}

/**
  * @brief	显示模式函数
  * @param	无
  * @retval	无
  *	通过执行DS1302_ReadTime();，不断刷新时间值
  */
void TimeShow(void)
{
	DS1302_ReadTime();						//读取DS1302的寄存器数值并转换为十进制，存入DS1302_Time[]数组
	LCD_ShowNum(1, 1, DS1302_Time[0], 2);
	LCD_ShowNum(1, 4, DS1302_Time[1], 2);
	LCD_ShowNum(1, 7, DS1302_Time[2], 2);
	LCD_ShowNum(2, 1, DS1302_Time[3], 2);
	LCD_ShowNum(2, 4, DS1302_Time[4], 2);
	LCD_ShowNum(2, 7, DS1302_Time[5], 2);
}

/**
  * @brief	修改模式函数
  * @param	无
  * @retval	无
  */
void TimeSet(void)
{
	//K2被按下，选择修改位，以timeSetSelect为索引修改DS1302_Time[]时间数组，按下一次增加一次，具有越界判断功能
	if (keyNum == 2)
	{
		timeSetSelect ++;
		timeSetSelect %= 6;		//越界清零，等效于↓
								//if (timeSetSelect > 5)
								//timeSetSelect = 0;
		//或者可以直接简写为：
		//timeSetSelect++ %= 6;
	}

	//K3被按下，对应被选中的修改位数值递增，按下一次增加一次，具有越界判断功能
	if (keyNum == 3)
	{
		//对应被选中的修改位数值递增
		DS1302_Time[timeSetSelect] ++;
		
		//越界判断
		//年
		if (DS1302_Time[0] > 99) {DS1302_Time[0] = 0;}
		
		//月
		if (DS1302_Time[1] > 12) {DS1302_Time[1] = 1;}
		
		//日
		//大月
		if ((DS1302_Time[1] == 1) || (DS1302_Time[1] == 3) || (DS1302_Time[1] == 5) ||
			 (DS1302_Time[1] == 7) || (DS1302_Time[1] == 8) || (DS1302_Time[1] == 10) || (DS1302_Time[1] == 12))
		{
			if (DS1302_Time[2] > 31)
				DS1302_Time[2]  = 1;
		}
		//小月
		else if ((DS1302_Time[1] == 4) || (DS1302_Time[1] == 6) || (DS1302_Time[1] == 9) ||
			 (DS1302_Time[1] == 11))
		{
			if (DS1302_Time[2] > 30)
				DS1302_Time[2]  = 1;
		}
		//二月
		else if (DS1302_Time[1] == 2)
		{
			//闰年二月29天
			if (DS1302_Time[0] % 4 == 0)
			{
				if (DS1302_Time[2] > 29)
					DS1302_Time[2] = 1;
			}
			//平年二月28天
			else
			{
				if (DS1302_Time[2] > 28)
					DS1302_Time[2] = 1;
			}
		}
		
		//时
		if (DS1302_Time[3] > 23) {DS1302_Time[3] = 0;}
		
		//分
		if (DS1302_Time[4] > 59) {DS1302_Time[4] = 0;}
		
		//秒
		if (DS1302_Time[5] > 59) {DS1302_Time[5] = 0;}
	}
	
	//K4被按下，对应被选中的修改位数值递减，按下一次减少一次，具有越界判断功能
	if (keyNum == 4)
	{
		//对应被选中的修改位数值递减
		DS1302_Time[timeSetSelect] --;
		
		//越界判断
		//注意，减的时候也需要对过大数值进行判断
		//年
		if (DS1302_Time[0] < 0) {DS1302_Time[0] = 99;}
		if (DS1302_Time[0] > 99) {DS1302_Time[0] = 0;}
		
		//月
		if (DS1302_Time[1] < 1) {DS1302_Time[1] = 12;}
		if (DS1302_Time[1] > 12) {DS1302_Time[1] = 1;}
		
		//日
		//大月
		if ((DS1302_Time[1] == 1) || (DS1302_Time[1] == 3) || (DS1302_Time[1] == 5) ||
			 (DS1302_Time[1] == 7) || (DS1302_Time[1] == 8) || (DS1302_Time[1] == 10) || (DS1302_Time[1] == 12))
		{
			if (DS1302_Time[2] < 1)
				DS1302_Time[2]  = 31;
			if (DS1302_Time[2] > 31)
				DS1302_Time[2]  = 1;
		}
		//小月
		else if ((DS1302_Time[1] == 4) || (DS1302_Time[1] == 6) || (DS1302_Time[1] == 9) ||
			 (DS1302_Time[1] == 11))
		{
			if (DS1302_Time[2] < 1)
				DS1302_Time[2]  = 30;
			if (DS1302_Time[2] > 30)
				DS1302_Time[2]  = 1;
		}
		//二月
		else if (DS1302_Time[1] == 2)
		{
			//闰年二月29天
			if (DS1302_Time[0] % 4 == 0)
			{
				if (DS1302_Time[2] < 1)
					DS1302_Time[2] = 29;
				if (DS1302_Time[2] >29)
					DS1302_Time[2] = 1;
			}
			//平年二月28天
			else
			{
				if (DS1302_Time[2] < 1)
					DS1302_Time[2] = 28;
				if (DS1302_Time[2] > 28)
					DS1302_Time[2] = 1;
			}
		}
		
		//时
		if (DS1302_Time[3] < 0) {DS1302_Time[3] = 23;}
		if (DS1302_Time[3] > 23) {DS1302_Time[3] = 0;}
		
		//分
		if (DS1302_Time[4] < 0) {DS1302_Time[4] = 59;}
		if (DS1302_Time[4] > 59) {DS1302_Time[4] = 0;}
		
		//秒
		if (DS1302_Time[5] < 0) {DS1302_Time[5] = 59;}
		if (DS1302_Time[5] > 59) {DS1302_Time[5] = 0;}
	}
	
	//对修改进行显示
	
	//修改时间时，Flash功能的实现（对选择的修改位进行闪烁显示以便于用户调节）：
	//当时间位处于修改状态并且timeSetFlashFlag（时间修改刷新标志位）为1时，显示空白。
	//不处于此状态，则显示时间值
	if (timeSetSelect == 0 && timeSetFlashFlag == 1)  	//Flash
		LCD_ShowString(1, 1, "  ");						//正常显示
	else
		LCD_ShowNum(1, 1, DS1302_Time[0], 2);
	if (timeSetSelect == 1 && timeSetFlashFlag == 1)
		LCD_ShowString(1, 4, "  ");
	else
		LCD_ShowNum(1, 4, DS1302_Time[1], 2);
	if (timeSetSelect == 2 && timeSetFlashFlag == 1)
		LCD_ShowString(1, 7, "  ");
	else
		LCD_ShowNum(1, 7, DS1302_Time[2], 2);
	if (timeSetSelect == 3 && timeSetFlashFlag == 1)
		LCD_ShowString(2, 1, "  ");
	else
		LCD_ShowNum(2, 1, DS1302_Time[3], 2);
	if (timeSetSelect == 4 && timeSetFlashFlag == 1)
		LCD_ShowString(2, 4, "  ");
	else
		LCD_ShowNum(2, 4, DS1302_Time[4], 2);
	if (timeSetSelect == 5 && timeSetFlashFlag == 1)
		LCD_ShowString(2, 7, "  ");
	else
		LCD_ShowNum(2, 7, DS1302_Time[5], 2);
}

/**
  * @brief	定时器0中断系统服务函数
  * @param	无
  * @retval	无
  * 定时器0每500ms修改一次timeSetFlashFlag的数值，实现Flash与正常显示的切换
  */
void Timer0_Routine()	interrupt 1
{
	static unsigned int t0Count;
	
	//每次进入中断，TH-TL都会被清零，不符合计时1ms要求，因此需要重新赋值
	TL0 = 0x66;			
	TH0 = 0xFC;
	t0Count ++;
	if (t0Count >= 500)
	{
		t0Count = 0;
		timeSetFlashFlag =! timeSetFlashFlag;
	}
}
