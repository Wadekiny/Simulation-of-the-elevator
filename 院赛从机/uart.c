#include <STC12C5A60S2.h>
#include "uart.h"

void UartInit()
{
	//不用PCON的SMOD 不加倍

	/*SCON八位数据 如果用第九位设置成d0--(为了蓝牙推荐设置成50)*/
	SCON |= 0x50; //设置模式1 允许接收 禁止多机
	AUXR |= 0X11;//使用独立波特率 12分频
	//装初值 公式：
	//RELOAD = 256-(INT)(SYSclk/Baud0/32/T*2^SOMD+0.5)
	BRT=0Xfd;
	EA = 1;	//总中断
	ES = 1;	//串口中断 用来接收数据
}
void SendChar(unsigned char dat)
{
	SBUF = dat;		//送SBUF
	while(!TI);		//等待送完
    TI = 0;
}
void SendStr(unsigned char *s)
{
	while(*s != '\0')         
	{
		SendChar(*s);
		s++;
	}
}
