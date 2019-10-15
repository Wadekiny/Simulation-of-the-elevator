#include <STC12C5A60S2.h>
#include "delay.h"
#include "oled.h"
#include "pwm.h"
#include "uart.h"
#define uchar unsigned char
#define uint unsigned int

#define DOWN 0
#define UP 1
#define WARNING 2
#define NOW 3
#define SPEED 4
#define STATE 5
#define DIR 6
#define NEXT 7
#define STOP 0
#define RUN 1

sbit up1= P2^1;
sbit up2= P0^6;
sbit up3= P0^4;
sbit up4= P0^2;

sbit down2= P2^3;
sbit down3= P2^5;
sbit down4= P2^7;
sbit down5= P0^0;

sbit led_up1= P2^0;
sbit led_up2= P0^7;
sbit led_up3= P0^5;
sbit led_up4= P0^3;

sbit led_down2= P2^2;
sbit led_down3= P2^4;
sbit led_down4= P2^6;
sbit led_down5= P0^1;

sbit r_warning= P3^2;
sbit led_warning = P3^3;
sbit buzzer=P3^7;

sbit red= P3^4;
sbit green=P3^5;
sbit led = P1^0;
uchar get[2];
uchar get_index=0;

uchar now_floor=0;
uchar next_floor=0;

uchar ele_state;
uchar ele_dir;

void KeyScan();
void Show();
void Init();
void main()
{
	Init();
	ASet(0xff);
	BSet(0xff);
	while (1)
	{
		if (ele_state == STOP)
		{
			OLED_ShowNum(0,5,0,1,16);//下降
			
			ASet(0xff);
			BSet(0xff);
	
		}
		else 
		{
			OLED_ShowNum(0,5,1,1,16);//上升
		}
		switch(now_floor)
		{
			case 1:led_up1=0;break;
			case 2:led_up2=0; led_down2=0;break;
			case 3:led_up3=0; led_down3=0;break;
			case 4:led_up4=0; led_down4=0;break;
			case 5:led_down5=0;break;
		}
		KeyScan();
		Show();
	}
}



void uart_isr() interrupt 4	//接收
{
	if (RI)
	{
		RI = 0;
		get[get_index++] = SBUF;
		if (get_index == 2)
			get_index = 0;
	}

	if (get[0] == WARNING)
	{
		led_warning = 1;
		while(1)
		{
			if (!r_warning)
			{
				Delay1ms();
				if (!r_warning)
				{
					led_warning = 0;
					while(!r_warning);
					break; 
				}
			}
		}
	}
	else if (get[0] == NOW)
	{
		now_floor = get[1];
	}
	else if (get[0] == DIR)
	{
		ele_dir = get[1];
	}
	else if (get[0] == NEXT)
	{
		next_floor = get[1];
	}
	else if (get[0] == STATE)
	{
		ele_state = get[1];
	}
	else if (get[0] == UP)
	{
		if (ele_state == STOP)
		{
			ASet(0xff);
			BSet(0xff);
		}
		else
		{
			ASet(get[1]);
			BSet(0xff);
		}
	}
	else if (get[0] == DOWN)
	{
		if (ele_state == STOP)
		{
			ASet(0xff);
			BSet(0xff);
		}
		else
		{
			ASet(0xff);
			BSet(get[1]);
		}
	}
}

void Init()
{
	led_up1= 0;
	led_up2= 0;
	led_up3= 0;
	led_up4= 0;

	led_down2= 0;
	led_down3= 0;
	led_down4= 0;
	led_down5= 0;
	
	green= 0;
	red=0;
	led_warning = 0;
	
	UartInit();
	PwmInit();
	OLED_Init();
	OLED_Clear();
}
void Show()
{
	//楼层 num
	OLED_ShowCHinese(0,0,0);
	OLED_ShowCHinese(18,0,1);
	OLED_ShowNum(38,0,now_floor,1,16);
	

	//箭头
		if (ele_dir == UP)
			OLED_ShowCHinese(80,5,5);
		else if (ele_dir == DOWN)
			OLED_ShowCHinese(80,5,6);
	
	//红绿灯
	if (ele_state == STOP)
	{
		green = 1;
		red = 0;
	}
	else if (ele_state == RUN)
	{
		red = 1;
		green = 0;
	}
}

void KeyScan()
{
	
	if (!up1)
	{
		Delay1ms();
		if (!up1)
		{
			led_up1 = 1;
			SendChar(11);
			while(!up1);
		}
	}
	if (!up2)
	{
		Delay1ms();
		if (!up2)
		{
			led_up2 = 1;
			SendChar(12);
			while(!up2);
		}
	}
	if (!up3)
	{
		Delay1ms();
		if (!up3)
		{
			led_up3 = 1;
			SendChar(13);
			while(!up3);
		}
	}
	if (!up4)
	{
		Delay1ms();
		if (!up4)
		{
			led_up4 = 1;
			SendChar(14);
			while(!up4);
		}
	}
	
	if (!down2)
	{
		Delay1ms();
		if (!down2)
		{
			led_down2 = 1;
			SendChar(2);
			while(!down2);
		}
	}
	if (!down3)
	{
		Delay1ms();
		if (!down3)
		{
			led_down3 = 1;
			SendChar(3);
			while(!down3);
		}
	}
	if (!down4)
	{
		Delay1ms();
		if (!down4)
		{
			led_down4 = 1;
			SendChar(4);
			while(!down4);
		}
	}
	if (!down5)
	{
		Delay1ms();
		if (!down5)
		{
			led_down5 = 1;
			SendChar(5);
			while(!down5);
		}
	} 
}