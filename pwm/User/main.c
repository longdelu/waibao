#include "stm32f10x.h"
#include "stdio.h"

#include "usart.h"
#include "gpio.h"
#include "delay.h"
#include "led.h"

#include "pwm.h"
#include "wave.h"
#include "key.h"

extern void Stm32_Clock_Init(u8 PLL);

int main(void)
{
    Stm32_Clock_Init(9);          //系统时钟设置72M
    delay_init(72);               //滴答延时初始化
    Init_Usart();                 //串口初始化，
    Usart_Configuration(9600);    //串口配置，设置波特率为9600
    LED_Init();                   //初始化与LED连接的硬件接口
    
    //72M / 72=1M 的计数频率，自动重装载为12500
    //PWM频率为1M/12500， 占空比为 6250/12500=0.5，刚好一半
    TIM3_PWM_Init(12500 - 1, 72 -1, 6250 - 1);

    //DAC初始化
    DAC1_Init();      
    DAC1_WAVE_Disable();
    DAC1_Enable(0);
    
    //正玄波初始化
    SinWave_Val_Init();
    SinWave_DMA_Init();
    
      
    //正玄波及三角波周期触发 
    TIM4_Init(10 - 1,   18 - 1);
    KEY_Init();                   //初始化与按键连接的硬件接口
    
    EXTIX_Init();                 //外部中断初始化
    
    while(1)
    { 
          
        LED1=~LED1;                           //输入一次LED1翻转一次 LED1闪烁，系统正在运行      
        delay_ms(500);                        //延时100ms  
        printf("running\r\n");                //串口打印消息表示正在运行
    }    
}     






