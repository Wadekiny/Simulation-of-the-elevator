#ifndef __HCSR04_H
#define __HCSR04_H

sbit Trig=P3^2;           //������ƶ˶˿�
sbit Echo=P3^3;           //������ն˶˿�


void HC_Init(void);       //������ģ���ʼ������
void HC_Out(void);        //���������ܺ���
float Distance (void);    //��ຯ��
void Time0_Init(void);    //��ʱ������0��ʼ������



#endif
