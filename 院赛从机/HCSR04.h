#ifndef __HCSR04_H
#define __HCSR04_H

sbit Trig=P3^2;           //定义控制端端口
sbit Echo=P3^3;           //定义接收端端口


void HC_Init(void);       //超声波模块初始化函数
void HC_Out(void);        //超声波接受函数
float Distance (void);    //测距函数
void Time0_Init(void);    //定时计数器0初始化函数



#endif
