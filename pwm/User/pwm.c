#include "stm32f10x_tim.h"
#include "extigroup.h" 
#include "delay.h"
#include "pwm.h"


//PWM输出初始化
//arr  ：自动重装值, PWM周期
//psc  ：时钟预分频数，为0时不分频
//pulse: PWM波脉冲周期

void TIM3_PWM_Init(u16 arr,u16 psc, u16 pulse)
{  
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;                    //定义一个GPIO结构体变量
 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);    //使能定时器3时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
    

    //设置 PA7该GPIOP引脚为复用输出功能,输出TIM3 CH2的PWM脉冲波形   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;            //TIM_CH2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;      //复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);               //初始化GPIO
    
    //初始化TIM3, 使用寄存器
//  TIM3->ARR=arr;			//设定计数器自动重装值 
//	TIM3->PSC=psc;			//预分频器不分频 
//	TIM3->CCMR1|=7<<12;  	//CH2 PWM2模式		 
//	TIM3->CCMR1|=1<<11; 	//CH2 预装载使能	   
//	TIM3->CCER|=1<<4;   	//OC2 输出使能	
//	TIM3->CCER|=1<<5;   	//OC2 低电平有效	   
//  TIM3->CCR2 =pulse;    	//pwm波形占空比
//	TIM3->CR1|=1<<7;   		//ARPE使能 
//	TIM3->CR1|=1<<0;    	//使能定时器3	
    
    
    
 
    //初始化TIM3, 使用库函数
    TIM_InternalClockConfig(TIM3);                              //配置定时器使用内部时钟
    TIM_TimeBaseStructure.TIM_Period = arr;                     //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
    TIM_TimeBaseStructure.TIM_Prescaler =psc;                   //设置用来作为TIMx时钟频率除数的预分频值 
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //设置时钟分割，PWM输出这个并不起作用
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);             //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
    
    //初始化定时器3通道2PWM模式     
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;             //选择定时器模式PWM脉冲宽度调制模式2
    TIM_OCInitStructure.TIM_Pulse = pulse;                        //设置PWM波形占空比，占空比等于pulse / arr
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;      //输出极性:TIM输出比较极性低
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);                      //根据指定的参数初始化外设PWM通道2

}



