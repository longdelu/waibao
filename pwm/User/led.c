#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"


// LED灯初始化
void LED_Init(void)
{
    RCC-> APB2ENR|=1<<8;
    RCC->APB2ENR|=1<<5;                  //使能GPIOD时钟  
    GPIOG->CRH&=0xf0ffffff;              //设置GPIOG_14为通用推挽输出
    GPIOG->CRH|=0x03000000;
    GPIOG->ODR|=1<<14;                   //LED2灯点亮
    GPIOD->CRH&=0XFF0FFFFF; 
    GPIOD->CRH|=0X00300000;              //PD_13 推挽输出        
    GPIOD->ODR|=1<<13;                   //GPIOD输出高电平，D5点亮
}    


