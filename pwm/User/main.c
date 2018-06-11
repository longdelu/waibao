#include "stm32f10x.h"
#include "stdio.h"

#include "usart.h"
#include "gpio.h"
#include "delay.h"
#include "led.h"

#include "pwm.h"
#include "key.h"

extern void Stm32_Clock_Init(u8 PLL);

int main(void)
{

    Stm32_Clock_Init(9);          //ϵͳʱ������72M
    delay_init(72);               //�δ���ʱ��ʼ��
    Init_Usart();                 //���ڳ�ʼ����
    Usart_Configuration(9600);    //�������ã� ���ò�����Ϊ9600
    LED_Init();                   //��ʼ����LED���ӵ�Ӳ���ӿ�
    TIM3_PWM_Init(500,72 -1, 249);//72M / 72=1M �ļ���Ƶ�ʣ��Զ���װ��Ϊ 500
                                  //PWMƵ��Ϊ1M/500=2kHZ�� ռ�ձ�Ϊ 250 / 500 = 0.5�� �պ�һ��
    
    KEY_Init();                   //��ʼ���밴�����ӵ�Ӳ���ӿ�
    EXTIX_Init();                 //�ⲿ�жϳ�ʼ��
    
    while(1)
    { 
          
        LED2=~LED2;                           //����һ��LED2��תһ�� LED2��˸��ϵͳ��������      
        delay_ms(500);                        //��ʱ100ms  
        printf("running\r\n");                //���ڴ�ӡ��Ϣ��ʾ��������
    }    
}     






