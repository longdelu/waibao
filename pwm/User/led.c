#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"


// LED�Ƴ�ʼ��
void LED_Init(void)
{
    RCC-> APB2ENR|=1<<8;
    RCC->APB2ENR|=1<<5;                  //ʹ��GPIODʱ��  
    GPIOG->CRH&=0xf0ffffff;              //����GPIOG_14Ϊͨ���������
    GPIOG->CRH|=0x03000000;
    GPIOG->ODR|=1<<14;                   //LED2�Ƶ���
    GPIOD->CRH&=0XFF0FFFFF; 
    GPIOD->CRH|=0X00300000;              //PD_13 �������        
    GPIOD->ODR|=1<<13;                   //GPIOD����ߵ�ƽ��D5����
}    


