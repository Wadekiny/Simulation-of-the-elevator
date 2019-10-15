#include <STC12C5A60S2.h>
#include <intrins.h>
#include "HCSR04.h"

unsigned char time_h,time_l;         //��������ʱ��������洢ʱ��
char succeed_flag;                   //����ɹ���־����

void Delay__us(unsigned int i)		//@����12.000MHz
{
	for(i;i>0;i--)
	{
	 _nop_();
	 _nop_();
	}
}

void Delay__ms(unsigned int n)		//@12.000MHz
{ 
	unsigned char i, j;
	for(n;n>0;n--)
	{

		_nop_();
		_nop_();
		i = 12;
		j = 168;
		do
		{
			while (--j);
		} 
		while (--i);
	}
}



/*------------------������ģ���ʼ������---------------------------*/
void HC_Init()
{
	Trig=0;                 //���ƶ���0
	Trig=1;                 //���ƶ���1
	Delay__us(20);            //��ʱ20us����Trig��������10us�ߵ�ƽ�ź�
	Trig=0;                 //���ƶ��ٴ���0
}


/*---------------------------���������պ���-----------------------------------*/
void HC_Out()
{	
	EA=1;                               //�����жϿ���
	while(Echo==0) ;                    //�ȴ���׽Echo�������źţ����䳬������
	succeed_flag=0;                     
	Time0_Init();                       //�򿪶�ʱ������0����ʼ��
	EX1=1;                              //���ⲿ�ж�
	Delay__ms(30);                        //��ʱ30ms���ȴ���׽Echo�½����źţ�����صĳ������źţ�
	TR0=0;	                            //�رն�ʱ������0
}


/*---------------------------------�����ຯ��------------------------------------------*/
float Distance()
{
	
	float distance;                              //����������                                  
	if(succeed_flag==1)                          //�ж��Ƿ�ɹ����յ�Echo�½����ź�
	{
		distance=(time_h*256+time_l)*0.017;        //�������
	}
	else 
	{
		distance=0.0;                              //���ʧ�ܼǾ���Ϊ0
	}
	return distance;                             //���ؾ���
}
	

/*----------------------------------T0��ʼ������---------------------------------------------------*/
void Time0_Init( )
{
	ET0=1;                             //��T0�ж�
	TMOD |=0X01;                       //ʹ��ģʽ1��16λ��ʱ����ʹ��"|"���ſ�����ʹ�ö����ʱ��ʱ����Ӱ��
	TR0=1;                             //�򿪶�ʱ��
	TH0=0X00;
	TL0=0X00;                          //��ֵ0
}



void INT1_subroutine() interrupt 2             
{ 
	TR0=0;                             //�رն�ʱ������0 
	time_h=TH0;
	time_l=TL0;                        //��¼Echo�ߵ�ƽ����ʱ�䣨�������źŷ������ʱ�䣩
	EX1=0;                             //�ر��ⲿ�ж�
	succeed_flag=1;                    //�ɹ���Ǳ���λ1
}

void Time0_subroutine() interrupt 1
{
	TR0=0;	//�رն�ʱ������0
}
