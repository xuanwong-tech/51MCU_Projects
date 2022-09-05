#include <REGX52.H>

//利用宏将DS1302时间信息定义
#define DS1302_SECOND		0x80
#define DS1302_MINUTE		0x82
#define DS1302_HOUR			0x84
#define DS1302_DATE			0x86
#define DS1302_MONTH		0x88
#define DS1302_DAY			0x8A
#define DS1302_YEAR			0x8C
#define DS1302_WP			0x8E

//定义DS1302与MCU通讯的三个引脚
sbit DS1302_SCLK = P3^6;
sbit DS1302_IO = P3^4;
sbit DS1302_CE = P3^5;

//定义时间数组：[year, month, date, hour, min, sec, day]
char DS1302_Time[] = {22, 9, 4, 21, 31, 1};
//有符号数范围：-128~+127

/**
  * @brief	DS1302初始化
  * @param	无
  * @retval	无
  * 因为单品机引脚上电默认为1，因此需要手动置0
  */
void DS1302_Init(void)
{
	DS1302_CE = 0;
	DS1302_SCLK = 0;
}

/**
  * @brief	DS1302写入一个字节
  * @param	command		命令字，即写入寄存器的物理地址
  * @param	writeData	写入的字节
  * @retval	无
  * DS1302写入数据的顺序：写入命令字 ——> 写入相应的数据
  */
void DS1302_WriteByte(unsigned char command, writeData)
{
	unsigned int i;
	
	//CE置1，写入开始
	DS1302_CE = 1;
	
	//写入命令字
	for (i = 0; i < 8; i ++)
	{
		DS1302_IO = command & (0x01 << i); 	//取出命令的第0位、第1位……、第7位
		DS1302_SCLK = 1;
		DS1302_SCLK = 0;
	}
	
	//写入数据
	for (i = 0; i < 8; i ++)
	{
		DS1302_IO = writeData & (0x01 << i); 	//取出命令的第0位、第1位……、第7位
		DS1302_SCLK = 1;
		DS1302_SCLK = 0;
	}
	
	//CE置0，写入结束
	DS1302_CE = 0;
}

/**
  * @brief	DS1302读取一个字节
  * @param	command		命令字，即读取寄存器的物理地址
  * @retval	readData	读取的数据(1B)
  * DS1302读取数据的顺序：写入命令字 ——> 读取相应的数据
  */
unsigned char DS1302_ReadByte(unsigned char command)
{
	unsigned char i, readData = 0x00;
	
	//读取的命令字比写入的命令字最低位多1(相同地址)
	//此举大幅减少定义读取地址的麻烦
	command |= 0x01;
	
	//CE置1，写入开始
	DS1302_CE = 1;
	
	//写入命令字
	for (i = 0; i < 8; i ++)
	{
		DS1302_IO = command & (0x01 << i); 	//取出命令的第0位、第1位……、第7位
		DS1302_SCLK = 0;
		DS1302_SCLK = 1;
		//注意此处SCLK上升沿与下降沿的顺序
		//原因：读取数据的SCLK一共有15个脉冲，与写入数据的16个脉冲并不相同
		//因为读取数据的SCLK在写入命令字结束的下降沿就立刻读出数据
		//而写入数据的SCLK的写入命令字与写入数据都是依靠上升沿
		//该处循环结束，停止在写入命令字的上升沿
	}
	
	//读取数据
	for (i = 0; i < 8; i ++)
	{
		DS1302_SCLK = 1;
		DS1302_SCLK = 0;
		if (DS1302_IO)
			readData |= (0x01 << i);
	}
	
	//CE置0，读取结束
	DS1302_CE = 0;
	
	//读取结束，清空IO
	DS1302_IO = 0;
	
	return readData;
}

/**
  * @brief	设置DS1302的计时时间
  * @param	无
  * @retval	无
  */
void DS1302_SetTime(void)
{
	DS1302_WriteByte(DS1302_WP, 0x00);	//关闭写保护
	DS1302_WriteByte(DS1302_YEAR, DS1302_Time[0]/10*16+DS1302_Time[0]%10);  //写入，将十进制数据转化为BCD码
	DS1302_WriteByte(DS1302_MONTH, DS1302_Time[1]/10*16+DS1302_Time[1]%10);
	DS1302_WriteByte(DS1302_DATE, DS1302_Time[2]/10*16+DS1302_Time[2]%10);
	DS1302_WriteByte(DS1302_HOUR, DS1302_Time[3]/10*16+DS1302_Time[3]%10);
	DS1302_WriteByte(DS1302_MINUTE, DS1302_Time[4]/10*16+DS1302_Time[4]%10);
	DS1302_WriteByte(DS1302_SECOND, DS1302_Time[5]/10*16+DS1302_Time[5]%10);
	DS1302_WriteByte(DS1302_DAY, DS1302_Time[6]/10*16+DS1302_Time[6]%10);
	DS1302_WriteByte(DS1302_WP, 0x00);	//打开写保护
}

/**
  * @brief	读取DS1302时间
  * @param	无
  * @retval	无
  */
void DS1302_ReadTime(void)
{
	unsigned char temp;
	temp = DS1302_ReadByte(DS1302_YEAR);
	DS1302_Time[0] = temp/16*10 + temp%16;		//读取，将BCD码转化为十进制数据
	temp = DS1302_ReadByte(DS1302_MONTH);
	DS1302_Time[1] = temp/16*10 + temp%16;
	temp = DS1302_ReadByte(DS1302_DATE);
	DS1302_Time[2] = temp/16*10 + temp%16;
	temp = DS1302_ReadByte(DS1302_HOUR);
	DS1302_Time[3] = temp/16*10 + temp%16;
	temp = DS1302_ReadByte(DS1302_MINUTE);
	DS1302_Time[4] = temp/16*10 + temp%16;
	temp = DS1302_ReadByte(DS1302_SECOND);
	DS1302_Time[5] = temp/16*10 + temp%16;
	temp = DS1302_ReadByte(DS1302_DAY);
	DS1302_Time[6] = temp/16*10 + temp%16;
}
