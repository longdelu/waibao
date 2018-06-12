#include "stm32f10x_gpio.h"
#include "gpio.h"
#include "delay.h"
#include "extigroup.h" 
#include "stm32f10x_exti.h"

#define INT1  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0)//��ȡ����INT1��ƽ״̬

volatile uint8_t i=0;

//������ʼ������
void KEY_Init(void) 
{ 
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);//ʹ��PORTEʱ��
	
    //��ʼ�� INT1-->GPIOE.0      ��������
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //PE0���ó����룬Ĭ������      
    GPIO_Init(GPIOE, &GPIO_InitStructure);         //��ʼ��GPIOE.0   
}

//���ð����жϳ�ʼ��
void EXTIX_Init(void)
{     
    Ex_NVIC_Config(GPIO_E,0,FTIR);      //����ӳ���ϵ��ʹ���жϣ������ش��� ,
    MY_NVIC_Init(2,3,EXTI0_IRQn,2);     //��ռ2�������ȼ�3����2��ʹ��NVIC�ж���0;        
    Ex_NVIC_Config(GPIO_C,13,FTIR);     //����ӳ���ϵ��ʹ���жϣ������ش��� ,
    MY_NVIC_Init(2,3,EXTI0_IRQn,2);     //��ռ2�������ȼ�3����2��ʹ��NVIC�ж���0;    
}

//�ⲿ�ж���0���жϷ�����  
void EXTI0_IRQHandler(void)
{
    
    delay_ms(10);//����
	
    if(INT1==0)          //INT1������PE0�ܽŰ��£�
    {          

        

    }
    
    EXTI_ClearITPendingBit(EXTI_Line0); //���LINE0�ϵ��жϱ�־λ 
 
}



