#include "pwm.h"
#include <STC12C5A60S2.h>
#define HIGH 0x1a   //90zkb
#define LOW 0X80 	//50zkb
#define NORMAL 0x4d	//70zkb

//pwm0左电机 P1.3
//pwm1右电机 P1.4
void PwmInit()
{
	CCON = 0;
	CL = 0;
	CH = 0;
	CMOD = 0X02;

	CCAP0H = CCAP0L = NORMAL;
	CCAPM0 = 0X42;
	CCAP1H = CCAP1L = NORMAL;
	CCAPM1 = 0X42;
	CR = 1;
}

void LeftSetHigh(){ CCAP0H = CCAP0L = HIGH; }
void RightSetHigh(){ CCAP1H = CCAP1L = HIGH; }
void LeftSetNormal(){ CCAP0H = CCAP0L = NORMAL; }
void RightSetNormal(){ CCAP1H = CCAP1L = NORMAL; }
void LeftSetLow(){ CCAP0H = CCAP0L = LOW; }
void RightSetLow(){ CCAP1H = CCAP1L = LOW; }
void ASet(unsigned char set){ CCAP0H = CCAP0L = set; }
void BSet(unsigned char set){ CCAP1H = CCAP1L = set; }