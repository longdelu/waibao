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
    Stm32_Clock_Init(9);          //ϵͳʱ������72M
    delay_init(72);               //�δ���ʱ��ʼ��
    Init_Usart();                 //���ڳ�ʼ����
    Usart_Configuration(9600);    //�������ã����ò�����Ϊ9600
    LED_Init();                   //��ʼ����LED���ӵ�Ӳ���ӿ�
    
    //72M / 72=1M �ļ���Ƶ�ʣ��Զ���װ��Ϊ12500
    //PWMƵ��Ϊ1M/12500�� ռ�ձ�Ϊ 6250/12500=0.5���պ�һ��
    TIM3_PWM_Init(12500 - 1, 72 -1, 6250 - 1);

    //DAC��ʼ��
    DAC1_Init();      
    DAC1_WAVE_Disable();
    DAC1_Enable(0);
    
    //��������ʼ��
    SinWave_Val_Init();
    SinWave_DMA_Init();
    
      
    //�����������ǲ����ڴ��� 
    TIM4_Init(10 - 1,   18 - 1);
    KEY_Init();                   //��ʼ���밴�����ӵ�Ӳ���ӿ�
    
    EXTIX_Init();                 //�ⲿ�жϳ�ʼ��
    
    while(1)
    { 
          
        LED1=~LED1;                           //����һ��LED1��תһ�� LED1��˸��ϵͳ��������      
        delay_ms(500);                        //��ʱ100ms  
        printf("running\r\n");                //���ڴ�ӡ��Ϣ��ʾ��������
    }    
}     






