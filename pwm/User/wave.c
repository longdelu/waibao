#include "stm32f10x_tim.h"
#include "stm32f10x_dac.h"
#include "stm32f10x_dma.h"
#include "extigroup.h" 
#include "delay.h"
#include "pwm.h"
#include "math.h"
#include "wave.h"


//正玄波数据表
__align(4)  static u16 sinwave_val[N];

//DAC1 通道ch使能   
void DAC1_Enable(u8 ch)
{
     DAC->CR|=1<<(16 * ch);          
     
}

//DAC1 通道ch不使能      
void DAC1_Disable(u8 ch)
{
     DAC->CR &=~(1<<(16 * ch));       
     
}


//使能生成三角波
void DAC1_WAVE_Enable(void)
{
    DAC->DHR12R1=0; 
    DAC->DOR1=0;     
    DAC->CR |= (2<<6);      
}

//禁能生成三角波
void DAC1_WAVE_Disable(void)
{
    DAC->DHR12R1=0;
    DAC->DOR1=0;      
    DAC->CR &= ~(3<<6);
           
}


//DAC通道1输出初始化  
void DAC1_Init(void)  
{   
    GPIO_InitTypeDef GPIO_InitStructure;    
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);   //使能DAC时钟
           
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //引脚时钟

    GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_4;           //DAC CH1对应的引脚
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;        //模拟输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);  


    DAC->CR|=1<<2;                                       //使能触发功能 TEN1=1  
    DAC->CR|=1<<1;                                       //DAC1输出缓存不使能 BOFF1=1  
    DAC->CR|=5<<3;                                       //DAC TIM4 TRGO,不过要TEN1=1才行
    DAC->CR |=  12<<8;                                   //设置振幅4095               
    DAC->DHR12R1=0;                                      //DAC通道1，该寄存器的值，当定时器触发事件发生后，计数值与该值相加然后送DOR1寄存当中，用于生成DAC的值输出。
    DAC_SetChannel1Data(DAC_Align_12b_R, 0);             //12位右对齐数据格式设置DAC值
} 

//正玄波DMA配置
void SinWave_DMA_Init (void)
{					
	DMA_InitTypeDef   DMA_InitStructure;
    
    
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	                                  //使能DMA1传输
    
	
	DMA_StructInit( &DMA_InitStructure);		                                          //DMA结构初始化
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;                                    //内存到外设
	DMA_InitStructure.DMA_BufferSize = N;                                                 //缓存大小，一般为256点
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;                      //外设地址不递增
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	                              //内存地址递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;           //宽度为半字
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;                   //宽度为半字
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                                 //中等优先级
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                          //关闭内存到内存模式
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                                       //循环发送模式 
    	
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(DAC->DHR12R1));               //外设地址为DAC通道1数据寄存器
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)sinwave_val;                         //内存地址为输出波形数据数组
    DMA_Init(DMA1_Channel7, &DMA_InitStructure);                                          //初始化DMA通道
    DMA_SetCurrDataCounter(DMA1_Channel7,N);                                              //DMA通道的DMA缓存的大小
    DMA_Cmd(DMA1_Channel7, ENABLE);                                                       //使能DMA通道         
      
}


//定时器4初始化
//arr  ：自动重装值, PWM周期
//psc  ：时钟预分频数，为0时不分频
void TIM4_Init(u16 arr,u16 psc)
{  
     TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

     RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);          //使能定时器4时钟
     
     //初始化TIM4, 使用库函数
     TIM_InternalClockConfig(TIM4);                                //配置定时器使用内部时钟
     TIM_TimeBaseStructure.TIM_Period = arr;                       //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
     TIM_TimeBaseStructure.TIM_Prescaler =psc;                     //设置用来作为TIMx时钟频率除数的预分频值 
     TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;       //设置时钟分割
     TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //TIM向上计数模式
     TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);               //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
     
     
     TIM_SelectOutputTrigger(TIM4, TIM_TRGOSource_Update);         //选择更新事件作为触发输出（TRGO）
    
     TIM_DMACmd(TIM4,TIM_DMA_Update,ENABLE);                       //使能定时器4更新事件触发DMA，输出正玄波 
    
     TIM_Cmd(TIM4, ENABLE);                                        //使能TIM4
}

//正弦波表值初始化
u16 SinWave_Val_Init(void)
{	  
    u16 i;
       
	for( i=0;i < N;i++)
	{   
        //生成波形表 
		sinwave_val[i] = (u16)((Um*sin(( 1.0*i/(N-1))*2*PI)+Um)*4095/3.3);
	}	
    
    return 0;					
}

//设置通道1输出电压
//vol:0~3300,代表0~3.3V
void Dac1_Set_Vol(u16 vol)
{
	float temp=vol;
	temp/=1000;
	temp=temp*4096/3.3;
	DAC_SetChannel1Data(DAC_Align_12b_R,temp);                    //12位右对齐数据格式设置DAC值
}
