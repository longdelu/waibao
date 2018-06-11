#include "stm32f10x_gpio.h"
#include "gpio.h"
#include "delay.h"
#include "extigroup.h" 
#include "stm32f10x_exti.h"

#define INT1  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0)//读取按键INT1电平状态

volatile uint8_t i=0;

//按键初始化函数
void KEY_Init(void) 
{ 
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);//使能PORTE时钟
	
    //初始化 INT1-->GPIOE.0      上拉输入
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //PE0设置成输入，默认上拉      
    GPIO_Init(GPIOE, &GPIO_InitStructure);         //初始化GPIOE.0   
}

//设置按键中断初始化
void EXTIX_Init(void)
{     
    Ex_NVIC_Config(GPIO_E,0,FTIR);      //设置映射关系，使能中断，下升沿触发 ,
    MY_NVIC_Init(2,3,EXTI0_IRQn,2);     //抢占2，子优先级3，组2，使能NVIC中断线0;        
    Ex_NVIC_Config(GPIO_C,13,FTIR);     //设置映射关系，使能中断，下升沿触发 ,
    MY_NVIC_Init(2,3,EXTI0_IRQn,2);     //抢占2，子优先级3，组2，使能NVIC中断线0;    
}

//外部中断线0的中断服务函数  
void EXTI0_IRQHandler(void)
{
    
    delay_ms(10);//消抖
	
    if(INT1==0)          //INT1按键（PE0管脚按下）
    {                                                            

    }
    
    EXTI_ClearITPendingBit(EXTI_Line0); //清除LINE0上的中断标志位 
 
}



