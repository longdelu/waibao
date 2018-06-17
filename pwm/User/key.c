#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "gpio.h"
#include "delay.h"
#include "extigroup.h" 
#include "stm32f10x_exti.h"
#include "wave.h"

#define KEY0  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)//��ȡ����KEY2��ƽ״̬
#define KEY1  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)//��ȡ����KEY3��ƽ״̬
#define KEY2  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)//��ȡ����KEY4��ƽ״̬

//������ʼ������
void KEY_Init(void) 
{ 
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);//ʹ��PORTEʱ��
	
    //��ʼ��KEY0->GPIOE2, KEY0->GPIOE3 , KEY0->GPIOE4      
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //PE0���ó����룬Ĭ������      
    GPIO_Init(GPIOE, &GPIO_InitStructure);         //��ʼ��GPIOE.2  
    
    
}

//���ð����жϳ�ʼ��
void EXTIX_Init(void)
{     
    Ex_NVIC_Config(GPIO_E,2,FTIR);      //����ӳ���ϵ��ʹ���жϣ������ش��� ,
    MY_NVIC_Init(2,3,EXTI2_IRQn,2);     //��ռ2�������ȼ�3����2��ʹ��NVIC�ж���2;   
    
    Ex_NVIC_Config(GPIO_E,3,FTIR);      //����ӳ���ϵ��ʹ���жϣ������ش��� ,
    MY_NVIC_Init(2,2,EXTI3_IRQn,2);     //��ռ2�������ȼ�2����2��ʹ��NVIC�ж���3;   

    Ex_NVIC_Config(GPIO_E,4,FTIR);      //����ӳ���ϵ��ʹ���жϣ������ش��� ,
    MY_NVIC_Init(2,1,EXTI4_IRQn,2);     //��ռ2�������ȼ�1����2��ʹ��NVIC�ж���4;        
}

//�ⲿ�ж���2���жϷ��������л��ɷ���  
void EXTI2_IRQHandler(void)
{
    
    delay_ms(10);        //����
	
    if(KEY0==0)          //KEY0������PE2�ܽŰ��£�
    {          

       TIM_Cmd(TIM3, ENABLE);                               //ʹ��TIM3�����PWM��
               
       DMA_Cmd(DMA1_Channel7, DISABLE);                     //����DMAͨ�� 
       DMA_SetCurrDataCounter(DMA1_Channel7,0);             //DMAͨ����DMA����Ĵ�С��0      
       TIM_DMACmd(TIM4,TIM_DMA_Update,DISABLE);             //���ܶ�ʱ��4�����¼�����DMA�������������   
        
       DAC1_WAVE_Disable();                                 //����DAC������ǲ�          

    }
    
    EXTI_ClearITPendingBit(EXTI_Line2);                     //���LINE2�ϵ��жϱ�־λ 
 
}


//�ⲿ�ж���3���жϷ��������л������ǲ�  
void EXTI3_IRQHandler(void)
{   
    delay_ms(10);//����
	
    if(KEY1==0)                                             //KEY1������PE3�ܽŰ��£�
    {  
        
       DMA_Cmd(DMA1_Channel7, DISABLE);                     //����DMAͨ�� 
       DMA_SetCurrDataCounter(DMA1_Channel7,0);             //DMAͨ����DMA����Ĵ�С��0      
       TIM_DMACmd(TIM4,TIM_DMA_Update,DISABLE);             //���ܶ�ʱ��4�����¼�����DMA�������������   
        
       TIM_Cmd(TIM3, DISABLE);                              //����TIM3�������PWM��  
        
       DAC1_WAVE_Enable();                                  //ʹ��DAC������ǲ� 
              
    }
    
    EXTI_ClearITPendingBit(EXTI_Line3);                     //���LINE3�ϵ��жϱ�־λ 
 
}

//�ⲿ�ж���4���жϷ��������л���������    
void EXTI4_IRQHandler(void)
{
    
    delay_ms(10);//����
	
    if(KEY2==0)          //KEY2������PE4�ܽŰ��£�
    {          

        DAC1_WAVE_Disable();                                 //����DAC������ǲ�
        TIM_Cmd(TIM3, DISABLE);                              //����TIM3�������PWM�� 
        
        DMA_Cmd(DMA1_Channel7, DISABLE);                     //����DMAͨ��
        DMA_SetCurrDataCounter(DMA1_Channel7,N);             //����DMAͨ����DMA����Ĵ�С          
        DMA_Cmd(DMA1_Channel7, ENABLE);                      //ʹ��DMAͨ��
        
        TIM_DMACmd(TIM4,TIM_DMA_Update,ENABLE);              //ʹ�ܶ�ʱ��4�����¼�����DMA�����������          

    }
    
    EXTI_ClearITPendingBit(EXTI_Line4); //���LINE4�ϵ��жϱ�־λ 
 
}


