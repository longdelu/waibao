#ifndef __WAVE_H
#define __WAVE_H

#include "stm32f10x.h"

#define  Vref       3.3	
#define  Um         (Vref/2)
#define  PI         3.1415926
#define  N          1024


//DAC1 ͨ��chʹ��   
void DAC1_Enable(u8 ch);

//DAC1 ͨ��ch��ʹ��      
void DAC1_Disable(u8 ch);


void DAC1_WAVE_Disable(void);
void DAC1_WAVE_Enable(void);

//DACͨ��1�����ʼ��  
void DAC1_Init(void);

//������DMA����, ʹ��ͨ��3
void SinWave_DMA_Init (void);

//��ʱ��4��ʼ��
//arr  ���Զ���װֵ, PWM����
//psc  ��ʱ��Ԥ��Ƶ����Ϊ0ʱ����Ƶ
void TIM4_Init(u16 arr,u16 psc);


//���Ҳ���ʼ��
u16 SinWave_Val_Init(void);

#endif 
