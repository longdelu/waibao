#include "stm32f10x_tim.h"
#include "stm32f10x_dac.h"
#include "stm32f10x_dma.h"
#include "extigroup.h" 
#include "delay.h"
#include "pwm.h"
#include "math.h"


#define  Vref       3.3	
#define  Um         (Vref/2)
#define  PI         3.1415926
#define  N          256


__align(4)  static u16 sinwave_val[N];

//DAC1 ͨ��chʹ��   
void DAC1_Enable(u8 ch)
{
     DAC->CR|=1<<(16 * ch);          
     
}

//DAC1 ͨ��ch��ʹ��      
void DAC1_Disable(u8 ch)
{
     DAC->CR &=~(1<<(16 * ch));       
     
}


//ʹ���������ǲ�
void DAC1_WAVE_Enable(void)
{
      
    DAC->CR |= (2<<6);      
}

//�����������ǲ�
void DAC1_WAVE_Disable(void)
{
      
    DAC->CR &= ~(3<<6);      
}


//DACͨ��1�����ʼ��  
void DAC1_Init(void)  
{   
    GPIO_InitTypeDef GPIO_InitStructure;    
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);   //ʹ��DACʱ��
           
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //����ʱ��

    GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_4;           //DAC CH1��Ӧ������
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;        //ģ������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);  


    DAC->CR|=1<<2;                                       //ʹ�ܴ������� TEN1=1  
    DAC->CR|=1<<1;                                       //DAC1������治ʹ�� BOFF1=1  
    DAC->CR|=5<<3;                                       //DAC TIM4 TRGO,����ҪTEN1=1����
    DAC->CR |=  12<<8;                                   //�������4095               
    DAC->DHR12R1=0;                                      //DACͨ��1���üĴ�����ֵ������ʱ�������¼������󣬼���ֵ���ֵ���Ȼ����DOR1�Ĵ浱�У���������DAC��ֵ�����
    DAC_SetChannel1Data(DAC_Align_12b_R, 0);             //12λ�Ҷ������ݸ�ʽ����DACֵ
} 

//������DMA����
void SinWave_DMA_Init (void)
{					
	DMA_InitTypeDef   DMA_InitStructure;
    
    
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	                                  //ʹ��DMA1����
    
	
	DMA_StructInit( &DMA_InitStructure);		                                          //DMA�ṹ��ʼ��
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;                                    //�ڴ浽����
	DMA_InitStructure.DMA_BufferSize = N;                                                 //�����С��һ��Ϊ256��
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;                      //�����ַ������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	                              //�ڴ��ַ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;           //���Ϊ����
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;                   //���Ϊ����
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                                 //�е����ȼ�
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                          //�ر��ڴ浽�ڴ�ģʽ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                                       //ѭ������ģʽ 
    	
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(DAC->DHR12R1));                      //�����ַΪDACͨ��1���ݼĴ���
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)sinwave_val;                                //�ڴ��ַΪ���������������
    DMA_Init(DMA1_Channel7, &DMA_InitStructure);                                                 //��ʼ��DMAͨ��
    DMA_SetCurrDataCounter(DMA1_Channel7,N);                                                     //DMAͨ����DMA����Ĵ�С
    DMA_Cmd(DMA1_Channel7, ENABLE);                                                              //ʹ��DMAͨ��7         
      
}


//��ʱ��4��ʼ��
//arr  ���Զ���װֵ, PWM����
//psc  ��ʱ��Ԥ��Ƶ����Ϊ0ʱ����Ƶ
void TIM4_Init(u16 arr,u16 psc)
{  
     TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

     RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);          //ʹ�ܶ�ʱ��4ʱ��
     
     //��ʼ��TIM4, ʹ�ÿ⺯��
     TIM_InternalClockConfig(TIM4);                                //���ö�ʱ��ʹ���ڲ�ʱ��
     TIM_TimeBaseStructure.TIM_Period = arr;                       //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
     TIM_TimeBaseStructure.TIM_Prescaler =psc;                     //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
     TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;       //����ʱ�ӷָ�
     TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //TIM���ϼ���ģʽ
     TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);               //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
     
     
     TIM_SelectOutputTrigger(TIM4, TIM_TRGOSource_Update);         //ѡ������¼���Ϊ���������TRGO��
    

     TIM_Cmd(TIM4, ENABLE);                                        //ʹ��TIM4
}

//���Ҳ���ʼ��
u16 SinWave_Val_Init(void)
{	  
    u16 i;
       
	for( i=0;i < N;i++)
	{   
        //���ɲ��α� 
		sinwave_val[i] = (u16)((Um*sin(( 1.0*i/(N-1))*2*PI)+Um)*4095/3.3);
	}	
    
    return 0;
					
}

//����ͨ��1�����ѹ
//vol:0~3300,����0~3.3V
void Dac1_Set_Vol(u16 vol)
{
	float temp=vol;
	temp/=1000;
	temp=temp*4096/3.3;
	DAC_SetChannel1Data(DAC_Align_12b_R,temp);//12λ�Ҷ������ݸ�ʽ����DACֵ
}
