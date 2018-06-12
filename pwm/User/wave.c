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

static u16 sinwave_val[256];
   
void DAC1_CH1_DMA_Enable(void)
{
     DAC->CR|=1<<12;    //DAC1 DMA使能    
     
}

void DAC1_CH1_DMA_Disable(void)
{
     DAC->CR &=~(1<<12);    //DAC1 DMA不使能    
     
}

void DAC_CH1_GPIO_Init( void)
{

    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //初始化引脚时钟

    GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_4;           //DAC CH1对应的引脚
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;        //模拟输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);          
}



//DAC通道1输出初始化  
void DAC_CH1_Init(void)  
{               
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE); //使能DAC时钟


    DAC->CR|=1<<2;     //使能触发功能 TEN1=1  
    DAC->CR|=1<<1;     //DAC1输出缓存不使能 BOFF1=1  
    DAC->CR|=5<<3;     //DAC TIM4 TRGO,不过要TEN1=1才行  
    DAC->CR|=2<<6;     //生成三角波  
    DAC->CR|=12<<8;    //设置振幅4095  
    DAC->CR|=0<<12;    //DAC1 DMA不使能        
    DAC->CR|=1<<0;     //使能DAC1  

    DAC->DHR12R1=0;    //DAC通道1，该寄存器的值，当定时器触发事件发生后，计数值与该值相加然后送DOR1寄存当中，用于生成DAC的值输出。
} 

//正玄波DMA配置, 使用通道3
void SinWave_DMA_Init (void)
{					
	DMA_InitTypeDef                DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);                                    //开DMA2的时钟
	
	DMA_StructInit( &DMA_InitStructure);		                                          //DMA结构初始化
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;                                    //从存储器读数据
	DMA_InitStructure.DMA_BufferSize = 256;                                               //缓存大小，一般为256点
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;                      //外设地址不递增
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	                              //内存地址递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;           //宽度为半字
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;                   //宽度为半字
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;                               //优先级：非常高
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                          //关闭内存到内存模式
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                                       //循环发送模式 
	
	
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(DAC->DHR12R1));                      //外设地址为DAC通道1数据寄存器
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)sinwave_val;                                //内存地址为输出波形数据数组
    DMA_Init(DMA1_Channel3, &DMA_InitStructure);                                                 //初始化
    DMA_Cmd(DMA1_Channel3, ENABLE);                                                              //使能DMA通道3         
      
}


//定时器4初始化
//arr  ：自动重装值, PWM周期
//psc  ：时钟预分频数，为0时不分频
void TIM4_Init(u16 arr,u16 psc)
{  
     TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

     RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);          //使能定时器4时钟
     
     //初始化TIM4, 使用库函数
     TIM_InternalClockConfig(TIM3);                                        //配置定时器使用内部时钟
     TIM_TimeBaseStructure.TIM_Period = arr;                            //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
     TIM_TimeBaseStructure.TIM_Prescaler =psc;                         //设置用来作为TIMx时钟频率除数的预分频值 
     TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;      //设置时钟分割
     TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
     TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);                 //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
     
     
     TIM_SelectOutputTrigger(TIM4, TIM_TRGOSource_Update);        //选择更新事件作为触发输出（TRGO）

     TIM_Cmd(TIM4, ENABLE);                                                 //使能TIM4
}

//正弦波初始化
u16 SinWave_Init(void)
{	  
    u16 i;
    
	for( i=0;i < N;i++)
	{   
        //生成波形表 
		sinwave_val[i]=(u16)((Um * sin((1.0*i/(N-1))*2* PI)+ Um) * 4096 / 3.3);
	}	
    
    return 0;
					
}
