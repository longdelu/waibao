#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "gpio.h"
#include "delay.h"
#include "extigroup.h" 
#include "stm32f10x_exti.h"
#include "wave.h"

#define KEY0  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)//读取按键KEY2电平状态
#define KEY1  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)//读取按键KEY3电平状态
#define KEY2  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)//读取按键KEY4电平状态

//按键初始化函数
void KEY_Init(void) 
{ 
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);//使能PORTE时钟
	
    //初始化KEY0->GPIOE2, KEY0->GPIOE3 , KEY0->GPIOE4      
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //PE0设置成输入，默认上拉      
    GPIO_Init(GPIOE, &GPIO_InitStructure);         //初始化GPIOE.2  
    
    
}

//设置按键中断初始化
void EXTIX_Init(void)
{     
    Ex_NVIC_Config(GPIO_E,2,FTIR);      //设置映射关系，使能中断，下升沿触发 ,
    MY_NVIC_Init(2,3,EXTI2_IRQn,2);     //抢占2，子优先级3，组2，使能NVIC中断线2;   
    
    Ex_NVIC_Config(GPIO_E,3,FTIR);      //设置映射关系，使能中断，下升沿触发 ,
    MY_NVIC_Init(2,2,EXTI3_IRQn,2);     //抢占2，子优先级2，组2，使能NVIC中断线3;   

    Ex_NVIC_Config(GPIO_E,4,FTIR);      //设置映射关系，使能中断，下升沿触发 ,
    MY_NVIC_Init(2,1,EXTI4_IRQn,2);     //抢占2，子优先级1，组2，使能NVIC中断线4;        
}

//外部中断线2的中断服务函数，切换成方波  
void EXTI2_IRQHandler(void)
{
    
    delay_ms(10);        //消抖
	
    if(KEY0==0)          //KEY0按键（PE2管脚按下）
    {          

       TIM_Cmd(TIM3, ENABLE);                               //使能TIM3，输出PWM波
       DAC1_WAVE_Disable();                                 //禁能DAC输出三角波  
       TIM_DMACmd(TIM4,TIM_DMA_Update,DISABLE);             //禁能定时器4更新事件触发DMA，不输出正玄波       

    }
    
    EXTI_ClearITPendingBit(EXTI_Line2);                     //清除LINE2上的中断标志位 
 
}


//外部中断线3的中断服务函数，切换成三角波  
void EXTI3_IRQHandler(void)
{   
    delay_ms(10);//消抖
	
    if(KEY1==0)                                             //KEY1按键（PE3管脚按下）
    {          
       DAC1_WAVE_Enable();                                  //使能DAC输出三角波 

       TIM_Cmd(TIM3, DISABLE);                              //禁能TIM3，不输出PWM波 
       TIM_DMACmd(TIM4,TIM_DMA_Update,DISABLE);             //禁能定时器4更新事件触发DMA，不输出正玄波           

    }
    
    EXTI_ClearITPendingBit(EXTI_Line3);                     //清除LINE3上的中断标志位 
 
}

//外部中断线4的中断服务函数，切换成正玄波    
void EXTI4_IRQHandler(void)
{
    
    delay_ms(10);//消抖
	
    if(KEY2==0)          //KEY2按键（PE4管脚按下）
    {          

        DAC1_WAVE_Disable();                                 //禁能DAC输出三角波
        TIM_Cmd(TIM3, DISABLE);                              //禁能TIM3，不输出PWM波 
        TIM_DMACmd(TIM4,TIM_DMA_Update,ENABLE);              //使能定时器4更新事件触发DMA，输出正玄波          

    }
    
    EXTI_ClearITPendingBit(EXTI_Line4); //清除LINE4上的中断标志位 
 
}


