#include "stm32f10x_tim.h"
#include "stm32f10x_dac.h"
#include "stm32f10x_dma.h"
#include "extigroup.h" 
#include "delay.h"
#include "pwm.h"
#include "math.h"


#define  Vref       3.3		 
#define  Um        (Vref/2)
#define  PI         3.14
#define  N          256

#ifndef  Sine_WaveOutput_Enable 

    static u16 Sinewave_val[256];
    
#endif

void DAC1_CH1_DMA_Enable(void)
{
     DAC->CR|=1<<12;    //DAC1 DMAʹ��    
     
}

void DAC1_CH1_DMA_Disable(void)
{
     DAC->CR &=~(1<<12);    //DAC1 DMA��ʹ��    
     
}

void DAC_CH1_GPIO_Init( void)
{
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��

    // GPIO_Set(GPIOA,PIN4,GPIO_MODE_AIN,0,0,GPIO_PUPD_PU);//PA4,ģ������,����         
    
}



//DACͨ��1�����ʼ��  
void DAC_CH1_Init(void)  
{            
     
     RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);                          //ʹ��DACʱ��
 

     DAC->CR|=1<<2;     //ʹ�ܴ������� TEN1=1  
     DAC->CR|=1<<1;     //DAC1������治ʹ�� BOFF1=1  
     DAC->CR|=5<<3;     //DAC TIM4 TRGO,����ҪTEN1=1����  
     DAC->CR|=2<<6;     //�������ǲ�  
     DAC->CR|=12<<8;    //�������4095  
     DAC->CR|=0<<12;    //DAC1 DMA��ʹ��        
     DAC->CR|=1<<0;     //ʹ��DAC1  
    
     DAC->DHR12R1=0;    //DACͨ��1���üĴ�����ֵ������ʱ�������¼������󣬼���ֵ���ֵ�� ��Ȼ����DOR1�Ĵ浱�У���������DAC��ֵ�����
} 

// ������DMA����, ʹ��ͨ��3
void SINWAVE_DMA_Config(DMA_Channel_TypeDef *dam_ch, u32 *src_addr,  u32 *dest_src)
{					
	DMA_InitTypeDef                DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);                                    //��DMA2��ʱ��
	
	DMA_StructInit( &DMA_InitStructure);		                                          //DMA�ṹ��ʼ��
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;                                    //�Ӵ洢��������
	DMA_InitStructure.DMA_BufferSize = 256;                                               //�����С��һ��Ϊ256��
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;                      //�����ַ������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	                              //�ڴ��ַ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;           //���Ϊ����
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;                   //���Ϊ����
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;                               //���ȼ����ǳ���
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                          //�ر��ڴ浽�ڴ�ģʽ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                                       //ѭ������ģʽ 
	
	
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(dest_src);                      //�����ַΪDACͨ��1���ݼĴ���
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)src_addr;                            //�ڴ��ַΪ���������������
    DMA_Init(dam_ch, &DMA_InitStructure);                                                 //��ʼ��
    DMA_Cmd(dam_ch, ENABLE);                                                              //ʹ��DMAͨ��3         
      
}


//��ʱ��4��ʼ��
//arr  ���Զ���װֵ, PWM����
//psc  ��ʱ��Ԥ��Ƶ����Ϊ0ʱ����Ƶ
void TIM4_Init(u16 arr,u16 psc)
{  
     TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

     RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);          //ʹ�ܶ�ʱ��4ʱ��
     
     //��ʼ��TIM4, ʹ�ÿ⺯��
     TIM_InternalClockConfig(TIM3);                                        //���ö�ʱ��ʹ���ڲ�ʱ��
     TIM_TimeBaseStructure.TIM_Period = arr;                            //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
     TIM_TimeBaseStructure.TIM_Prescaler =psc;                         //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
     TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;      //����ʱ�ӷָ�
     TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
     TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);                 //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
     
     
     TIM_SelectOutputTrigger(TIM4, TIM_TRGOSource_Update);        //ѡ������¼���Ϊ���������TRGO��

     TIM_Cmd(TIM4, ENABLE);                                                 //ʹ��TIM4
}

// �������Ҳ��������
void SINWAVE_Val(u16 cycle ,u16 *val)
{
	u16 i;
	for( i=0;i < cycle;i++)
	{
		val[i]=(u16)((Um * sin((1.0*i/(cycle-1))*2* PI)+ Um) * 4096 / 3.3);
	}
}

//���Ҳ���ʼ��
u16 SINEWAVE_Init(u8 Wave1,u16 Wave1_Fre,u8 NewState1)
{
	u16 frq = (u16)(72000000/sizeof(Sinewave_val)*2/Wave1_Fre);
    
    SINWAVE_Val(N ,Sinewave_val);		             //���ɲ��α�
    
    return frq;
					
}
