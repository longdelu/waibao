#include "stm32f10x_tim.h"
#include "stm32f10x_dac.h"
#include "extigroup.h" 
#include "delay.h"
#include "pwm.h"

//DAC通道1输出初始化  
void DAC1_Init(void)  
{         
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);                          //使能DAC时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟

   // GPIO_Set(GPIOA,PIN4,GPIO_MODE_AIN,0,0,GPIO_PUPD_PU);//PA4,模拟输入,下拉     

    DAC->CR|=1<<2;    //使能触发功能 TEN1=1  
    DAC->CR|=1<<1;    //DAC1输出缓存不使能 BOFF1=1  
    DAC->CR|=5<<3;    //DAC TIM4 TRGO,不过要TEN1=1才行  
    DAC->CR|=2<<6;    //生成三角波  
    DAC->CR|=12<<8;   //设置振幅4095  
    DAC->CR|=0<<12;   //DAC1 DMA不使能      
    DAC->CR|=1<<0;    //使能DAC1  
    DAC->DHR12R1=0;  
} 


//定时器4初始化
//arr  ：自动重装值, PWM周期
//psc  ：时钟预分频数，为0时不分频

void TIM4_Init(u16 arr,u16 psc)
{  
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);        //使能定时器4时钟
    
    //初始化TIM4, 使用库函数
    TIM_InternalClockConfig(TIM3);                              //配置定时器使用内部时钟
    TIM_TimeBaseStructure.TIM_Period = arr;                     //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
    TIM_TimeBaseStructure.TIM_Prescaler =psc;                   //设置用来作为TIMx时钟频率除数的预分频值 
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //设置时钟分割
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);             //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
    
    
    TIM4->CR2|=2<<4;                                           //选择更新事件作为触发输出（TRGO）

    TIM_Cmd(TIM4, ENABLE);                                     //使能TIM4
}
