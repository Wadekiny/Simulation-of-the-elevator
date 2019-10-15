#include <STC12C5A60S2.h>
#include <math.h>
#include "delay.h"
#include "HCSR04.h"
#include "oled.h"
#include "uart.h"
#include "def.h"

//λ����
sbit led5 = P0^0;
sbit led4 = P0^2;
sbit led3 = P0^4;
sbit led2 = P0^6;
sbit led1 = P2^7;

sbit key5 = P0^1;
sbit key4 = P0^3;
sbit key3 = P0^5;
sbit key2 = P0^7;
sbit key1 = P2^6;

sbit warning = P2^4;
sbit warn_led = P2^5;
sbit buzzer = P2^0;


struct _Floor{		
	uchar push;			//��������
	uchar direction;	//����
}floors[6];


uchar now_floor = 1;	//��ǰ¥��
uchar next_floor;	//��һ¥��
float now_distance; //��ǰ��ؾ���
uchar pushed_flag=FALSE;

uchar counter = 0;		//50ms������
uchar stay_counter=0;	//ͣ��ʱ�������
uchar ele_dir=1;
uchar temp;
uchar ele_state=0;
uchar get_message;
void FloorInit();
void T1Init();
void Init();

void KeyScan();
void Dectect();

void IsPushed();
void IsArrived();

void FindNext();
uchar CalNowFloor();
void CalZkb();

void Send();

void ShowWelcome();
void ShowFloor();


void main()
{
	Init();
	ShowWelcome();
	while (1)
	{
		Dectect();	//���

		FindNext();//�ҵ���һ��
		now_distance = Distance();	//��ຯ��
		now_floor = CalNowFloor();	//�ж�¥��

		ShowFloor();
		OLED_ShowNum(0,6,now_distance,3,16);	//��ؾ���
		OLED_ShowNum(100,6,next_floor,1,16);	//��һ¥��
		//���
		switch(now_floor)
		{
			case 1: led1 = 0; break;
			case 2:	led2 = 0; break;
			case 3: led3 = 0; break;
			case 4: led4 = 0; break;
			case 5: led5 = 0; break;
		}
		CalZkb();
		Send();
	}
	
}

void FindNext()
{
	IsPushed();		//�Ƿ��а�������
	if (now_floor == 5)		//����¥�����dir
	{
		ele_dir = DOWN;	
	}
	else if (now_floor == 1)
	{
		ele_dir = UP;
	}
	//�ж���һ��
	if (pushed_flag == TRUE)
	{
		temp = now_floor;
		if (ele_dir == UP)
		{
			while (floors[++temp].push == FALSE)
			{
				if (temp == 5)
				{
					ele_dir = DOWN;
					break;
				}
			}
		}
		else if (ele_dir == DOWN)
		{
			while (floors[--temp].push == FALSE)
			{
				if (temp == 1)
				{
					ele_dir = UP;
					break;
				}
			}
		}
		next_floor = temp;
	}
	else
	{
		next_floor = now_floor;
	}
}

void CalZkb()
{
	
	if (now_floor == next_floor)
	{
		if (floors[now_floor].push == TRUE)
		{
			buzzer = 0;
			Delay100ms();
			Delay100ms();
			buzzer = 1;
		}
		floors[now_floor].push = FALSE;
		ele_state = STOP;
		SendChar(STATE);
		SendChar(STOP);
	}
	else if (ele_dir == UP)
	{
		SendChar(STATE);
		SendChar(RUN);
		SendChar(UP);
		SendChar(0x8f);
	}
	else if (ele_dir == DOWN)
	{
		SendChar(STATE);
		SendChar(RUN);
		SendChar(DOWN);
		SendChar(0xdf);
	}
	
}
/*
void t1timer() interrupt 3 //50ms
{
	TR1 = 0;
	TH1 = 0x3C;
    TL1 = 0xB0;

	if (stay_flag)
	{
		counter++;
	}
	
	if (counter == 19)	//���20�� �ﵽһ��
	{
		counter = 0; 	//ms���������

		if (stay_flag == TRUE)	//�������ͣ��
		{
			stay_counter++;		//ͣ��������++
			if (stay_counter == STAY_TIME)	//����Ҫͣ����ʱ��
			{
				stay_counter = 0;	//ͣ������������
				stay_flag = FALSE;
			}
		}
	}
	
	TR1 = 1;
}
*/
////////////////////////////////////////////////////////////////////////////
uchar CalNowFloor()
{
	if (now_distance<L1+1)
		return 1;
	else if (now_distance<L2+1 && now_distance>L2-1)
		return 2;
	else if (now_distance<L3+1 && now_distance>L3-1)
		return 3;
	else if (now_distance<L4+1 && now_distance>L4-1)
		return 4;
	else if (now_distance<L5+1 && now_distance>L5-1)
		return 5;
	else
		return now_floor;
}

void Dectect()	//���
{
	KeyScan();		//������
	HC_Init();      //������ģ���ʼ������
	HC_Out();       //���������ܺ���
}

void Send()	//����
{/*
	SendChar(STATE);		
	SendChar(ele_state);
	*/
	SendChar(NOW);		//¥��
	SendChar(now_floor);
	
	SendChar(NEXT);
	SendChar(next_floor);
	
	SendChar(DIR);		
	SendChar(ele_dir);
}

void KeyScan()
{
	if (!warning)
	{
		Delay1ms();
		if (!warning)
		{
			floors[0].push = TRUE;
			//buzzer = 0;
			warn_led = 1;
			SendChar(WARNING);
			SendChar(0);
			while (!warning);
		}
	}
	if (!key1)
	{
		Delay1ms();
		if (!key1  && now_floor!=1)
		{
			floors[1].push = TRUE;
			floors[1].direction = UP;
			led1 = 1;
			while (!key1);
		}
	}
	if (!key2)
	{
		Delay1ms();
		if (!key2 && now_floor!=2)
		{
			floors[2].push = TRUE;
			if (now_floor > 2)
				floors[2].direction = DOWN;
			else if (now_floor < 2)
				floors[2].direction = UP;
			led2 = 1;
			while (!key2);
		}
	}
	if (!key3)
	{
		Delay1ms();
		if (!key3 && now_floor!=3)
		{
			floors[3].push = TRUE;
			if (now_floor > 3)
				floors[3].direction = DOWN;
			else if (now_floor < 3)
				floors[3].direction = UP;
			led3 = 1;
			while (!key3);
		}
	}
	if (!key4)
	{
		Delay1ms();
		if (!key4 && now_floor!=4)
		{
			floors[4].push = TRUE;
			if (now_floor > 4)
				floors[4].direction = DOWN;
			else if (now_floor < 4)
				floors[4].direction = UP;
			led4 = 1;
			while (!key4);
		}
	}
	if (!key5)
	{

		Delay1ms();
		if (!key5 && now_floor!=5)
		{
			floors[5].push = TRUE;
			floors[5].direction = DOWN;
			led5 = 1;
			while (!key5);
		}
	}
}

void uart_isr() interrupt 4	//����
{
	if (RI)
	{
		RI = 0;
		get_message = SBUF;
		if (get_message == 0xff)
		{
			warn_led = 0;
			floors[0].push = FALSE;
		}
		floors[get_message%10].push = TRUE;
		floors[get_message%10].direction = get_message/10;
	}
}

void Init()	//��ʼ��
{
	led1 = 0;
	led2 = 0;
	led3 = 0;
	led4 = 0;
	led5 = 0;
	warn_led =0;
	buzzer = 1;
	FloorInit();
	UartInit();	
	OLED_Init();
	OLED_Clear();	
	//T1Init();		
}
/*
void T1Init()//T1��ʼ��
{
	TMOD |= 0x10;
	EA = 1;
	ET1 = 1;
	TH1 = 0x3C;
    TL1 = 0xB0;
	TR1 = 1;
}
*/
void FloorInit()//¥����Ϣ��ʼ��
{
	unsigned char i;
	for (i=0; i<=5; i++)
	{
		floors[i].push = FALSE;
		floors[i].direction = DOWN;
	}
}



void ShowWelcome()
{
	/*---------------
	| ?X????! 	|  
	|             	|
	| ??:n   ?/?	|	
	---------------*/
	OLED_ShowCHinese(4,0,4);
	OLED_ShowChar(24,0,'X');
	OLED_ShowCHinese(37,0,5);
	OLED_ShowCHinese(55,0,6);
	OLED_ShowCHinese(73,0,7);
	OLED_ShowCHinese(91,0,8);
	OLED_ShowCHinese(109,0,9);
	
	OLED_ShowCHinese(5,4,10);
	OLED_ShowCHinese(23,4,11);
}

void ShowFloor()
{
	OLED_ShowNum(50,4,now_floor,1,16);
		if (ele_dir == UP)
			OLED_ShowCHinese(100,4,12);	
		else if (ele_dir == DOWN)
			OLED_ShowCHinese(100,4,13);
}

void IsPushed()
{
	uchar i;
	pushed_flag = FALSE;
	for (i=0; i<=5; i++)
	{
		if (floors[i].push == TRUE)
		{
			pushed_flag = TRUE;
			break;
		}
	}
}
/*
void IsArrived()	//�Ƿ񵽴�¥��
	{
		if (floors[now_floor].push && floors[now_floor].direction==ele_dir)
		{
			counter = 0;
			floors[now_floor].push = FALSE;	//����floor�ṹ���push
			ele_state = STOP;				//״̬����ΪSTOP
		//	buzzer = 0;						//��������
		}
	}
*/