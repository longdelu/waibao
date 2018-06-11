#include "stm32f10x_tim.h"
#include "stm32f10x_dac.h"
#include "extigroup.h" 
#include "delay.h"
#include "pwm.h"

//DACͨ��1�����ʼ��  
void DAC1_Init(void)  
{         
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);                          //ʹ��DACʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��

   // GPIO_Set(GPIOA,PIN4,GPIO_MODE_AIN,0,0,GPIO_PUPD_PU);//PA4,ģ������,����     

    DAC->CR|=1<<2;    //ʹ�ܴ������� TEN1=1  
    DAC->CR|=1<<1;    //DAC1������治ʹ�� BOFF1=1  
    DAC->CR|=5<<3;    //DAC TIM4 TRGO,����ҪTEN1=1����  
    DAC->CR|=2<<6;    //�������ǲ�  
    DAC->CR|=12<<8;   //�������4095  
    DAC->CR|=0<<12;   //DAC1 DMA��ʹ��      
    DAC->CR|=1<<0;    //ʹ��DAC1  
    DAC->DHR12R1=0;  
} 


//��ʱ��4��ʼ��
//arr  ���Զ���װֵ, PWM����
//psc  ��ʱ��Ԥ��Ƶ����Ϊ0ʱ����Ƶ

void TIM4_Init(u16 arr,u16 psc)
{  
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);        //ʹ�ܶ�ʱ��4ʱ��
    
    //��ʼ��TIM4, ʹ�ÿ⺯��
    TIM_InternalClockConfig(TIM3);                              //���ö�ʱ��ʹ���ڲ�ʱ��
    TIM_TimeBaseStructure.TIM_Period = arr;                     //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
    TIM_TimeBaseStructure.TIM_Prescaler =psc;                   //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //����ʱ�ӷָ�
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);             //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
    
    
    TIM4->CR2|=2<<4;                                           //ѡ������¼���Ϊ���������TRGO��

    TIM_Cmd(TIM4, ENABLE);                                     //ʹ��TIM4
}
