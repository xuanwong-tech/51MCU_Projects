#ifndef	__MATRIXLED_H__
#define	__MATRIXLED_H__

void MatrixLed_Init();
void _74HC595_WriteByte(unsigned char byte);
void MatrixLed_ShowData(unsigned char column, showData);
void MatrixLed_translate(int x, int y);

#endif