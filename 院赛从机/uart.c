#include <STC12C5A60S2.h>
#include "uart.h"

void UartInit()
{
	//����PCON��SMOD ���ӱ�

	/*SCON��λ���� ����õھ�λ���ó�d0--(Ϊ�������Ƽ����ó�50)*/
	SCON |= 0x50; //����ģʽ1 ������� ��ֹ���
	AUXR |= 0X11;//ʹ�ö��������� 12��Ƶ
	//װ��ֵ ��ʽ��
	//RELOAD = 256-(INT)(SYSclk/Baud0/32/T*2^SOMD+0.5)
	BRT=0Xfd;
	EA = 1;	//���ж�
	ES = 1;	//�����ж� ������������
}
void SendChar(unsigned char dat)
{
	SBUF = dat;		//��SBUF
	while(!TI);		//�ȴ�����
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
