#include "stm32f10x_tim.h"
#include "extigroup.h" 
#include "delay.h"
#include "pwm.h"


//PWM�����ʼ��
//arr  ���Զ���װֵ, PWM����
//psc  ��ʱ��Ԥ��Ƶ����Ϊ0ʱ����Ƶ
//pulse: PWM����������

void TIM3_PWM_Init(u16 arr,u16 psc, u16 pulse)
{  
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;                    //����һ��GPIO�ṹ�����
 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);    //ʹ�ܶ�ʱ��3ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
    

    //���� PA7��GPIOP����Ϊ�����������,���TIM3 CH2��PWM���岨��   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;            //TIM_CH2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;      //�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);               //��ʼ��GPIO
    
    //��ʼ��TIM3, ʹ�üĴ���
//  TIM3->ARR=arr;			//�趨�������Զ���װֵ 
//	TIM3->PSC=psc;			//Ԥ��Ƶ������Ƶ 
//	TIM3->CCMR1|=7<<12;  	//CH2 PWM2ģʽ		 
//	TIM3->CCMR1|=1<<11; 	//CH2 Ԥװ��ʹ��	   
//	TIM3->CCER|=1<<4;   	//OC2 ���ʹ��	
//	TIM3->CCER|=1<<5;   	//OC2 �͵�ƽ��Ч	   
//  TIM3->CCR2 =pulse;    	//pwm����ռ�ձ�
//	TIM3->CR1|=1<<7;   		//ARPEʹ�� 
//	TIM3->CR1|=1<<0;    	//ʹ�ܶ�ʱ��3	
    
    
    
 
    //��ʼ��TIM3, ʹ�ÿ⺯��
    TIM_InternalClockConfig(TIM3);                              //���ö�ʱ��ʹ���ڲ�ʱ��
    TIM_TimeBaseStructure.TIM_Period = arr;                     //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
    TIM_TimeBaseStructure.TIM_Prescaler =psc;                   //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //����ʱ�ӷָPWM����������������
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);             //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
    
    //��ʼ����ʱ��3ͨ��2PWMģʽ     
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;             //ѡ��ʱ��ģʽPWM�����ȵ���ģʽ2
    TIM_OCInitStructure.TIM_Pulse = pulse;                        //����PWM����ռ�ձȣ�ռ�ձȵ���pulse / arr
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;      //�������:TIM����Ƚϼ��Ե�
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);                      //����ָ���Ĳ�����ʼ������PWMͨ��2

}



