#include <STC12C5A60S2.h>
#include <intrins.h>
#include "HCSR04.h"

unsigned char time_h,time_l;         //定义两个时间变量，存储时间
char succeed_flag;                   //定义成功标志变量

void Delay__us(unsigned int i)		//@晶振12.000MHz
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



/*------------------超声波模块初始化函数---------------------------*/
void HC_Init()
{
	Trig=0;                 //控制端置0
	Trig=1;                 //控制端置1
	Delay__us(20);            //延时20us，给Trig发送至少10us高电平信号
	Trig=0;                 //控制端再次置0
}


/*---------------------------超声波接收函数-----------------------------------*/
void HC_Out()
{	
	EA=1;                               //打开总中断开关
	while(Echo==0) ;                    //等待捕捉Echo上升沿信号（发射超声波）
	succeed_flag=0;                     
	Time0_Init();                       //打开定时计数器0并初始化
	EX1=1;                              //打开外部中断
	Delay__ms(30);                        //延时30ms，等待捕捉Echo下降沿信号（反射回的超声波信号）
	TR0=0;	                            //关闭定时计数器0
}


/*---------------------------------计算测距函数------------------------------------------*/
float Distance()
{
	
	float distance;                              //定义距离变量                                  
	if(succeed_flag==1)                          //判断是否成功接收到Echo下降沿信号
	{
		distance=(time_h*256+time_l)*0.017;        //计算距离
	}
	else 
	{
		distance=0.0;                              //测距失败记距离为0
	}
	return distance;                             //返回距离
}
	

/*----------------------------------T0初始化函数---------------------------------------------------*/
void Time0_Init( )
{
	ET0=1;                             //打开T0中断
	TMOD |=0X01;                       //使用模式1，16位定时器，使用"|"符号可以在使用多个定时器时不受影响
	TR0=1;                             //打开定时器
	TH0=0X00;
	TL0=0X00;                          //赋值0
}



void INT1_subroutine() interrupt 2             
{ 
	TR0=0;                             //关闭定时计数器0 
	time_h=TH0;
	time_l=TL0;                        //记录Echo高电平持续时间（超声波信号发射接收时间）
	EX1=0;                             //关闭外部中断
	succeed_flag=1;                    //成功标记变量位1
}

void Time0_subroutine() interrupt 1
{
	TR0=0;	//关闭定时计数器0
}
