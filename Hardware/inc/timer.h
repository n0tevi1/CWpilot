#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

extern uint8_t  loop200HzFlag,loop500HzFlag,loop50HzFlag,loop600HzFlag,loop100HzFlag,loop10HzFlag,loop20HzFlag;
void TIM3_Int_Init(u16 arr,u16 psc);
void TIM3_PWM_Init(u16 arr,u16 psc);
void TIM4_Init(void);
#endif
