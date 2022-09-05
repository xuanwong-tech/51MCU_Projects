#include <REGX52.H>
#include "Delay.h"

/**
  * @brief	获取独立按键键码
  * @param	无
  * @retval	keyNum 未按按键值为0，按下不松手程序阻塞，松手立即返回键码1~4
  */
unsigned char Key()
{
	unsigned char keyNum = 0;
	
	if (P3_1 == 0) {Delay(20); while(P3_1 == 0); Delay(20); keyNum = 1; }
	if (P3_0 == 0) {Delay(20); while(P3_0 == 0); Delay(20); keyNum = 2; }
	if (P3_2 == 0) {Delay(20); while(P3_2 == 0); Delay(20); keyNum = 3; }
	if (P3_3 == 0) {Delay(20); while(P3_3 == 0); Delay(20); keyNum = 4; }
	
	return keyNum;
}
