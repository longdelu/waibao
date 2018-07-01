#ifndef __WAVE_H
#define __WAVE_H

#include "stm32f10x.h"

#define  Vref       3.3	
#define  Um         (Vref/2)
#define  PI         3.1415926
#define  N          1024


//DAC1 通道ch使能   
void DAC1_Enable(u8 ch);

//DAC1 通道ch不使能      
void DAC1_Disable(u8 ch);


void DAC1_WAVE_Disable(void);
void DAC1_WAVE_Enable(void);

//DAC通道1输出初始化  
void DAC1_Init(void);

//正玄波DMA配置, 使用通道3
void SinWave_DMA_Init (void);

//定时器4初始化
//arr  ：自动重装值, PWM周期
//psc  ：时钟预分频数，为0时不分频
void TIM4_Init(u16 arr,u16 psc);


//正弦波初始化
u16 SinWave_Val_Init(void);

#endif 
