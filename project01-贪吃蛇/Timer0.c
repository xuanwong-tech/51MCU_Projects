#include <REGX52.H>

/**
  * @brief	Timer0初始化函数，设定每1ms溢出一次，晶振频率为11.0592MHz
  * @param	无
  * @retval	无
  */
void Timer0_Init()
{
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0x66;		//设置定时初始值
	TH0 = 0xFC;		//设置定时初始值
	TF0 = 0;			//清除TF0标志
	TR0 = 1;			//定时器0开始计时
	ET0 = 1;			//设置中断系统溢出允许
	EA = 1;				//设置总中断控制位
	PT0 = 0;			//设置中断优先级
}

/**
  * @brief	定时器0所申请的中断函数，此处设定为每1ms*1000=1s执行一次
  * @param	无
  * @retval	无
  */
//void Timer0_Routine()	interrupt 1
//{
//	static unsigned int t0Count;
//	
//	//每次进入中断，TH-TL都会被清零，不符合计时1ms要求，因此需要重新赋值
//	TL0 = 0x66;			
//	TH0 = 0xFC;
//	t0Count ++;
//	if (t0Count >= 1000)
//	{
//		t0Count = 0;
//		####此处进行中断操作###
//	}
//}