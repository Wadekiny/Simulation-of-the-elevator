#ifndef _PWM_H_
#define _PWM_H_
void PwmInit();
void LeftSetHigh();
void RightSetHigh();
void LeftSetNormal();
void RightSetNormal();
void LeftSetLow();
void RightSetLow();
void ASet(unsigned char set);
void BSet(unsigned char set);
#endif