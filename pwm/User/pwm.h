#ifndef __pwm_H
#define __pwm_H
#include "stm32f10x.h"

void TIM3_PWM_Init(u16 arr, u16 psc, u16 pulse);

void Change_duty_cycle(void);

#endif 
