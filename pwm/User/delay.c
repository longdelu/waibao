#include "stm32f10x.h"

static u32  fac_us=0;  //us延时倍乘数               
static u32 fac_ms=0;  //ms延时倍乘数
 
//初始化延迟函数
//SYSTICK的时钟固定为HCLK时钟的1/8
//SYSCLK:系统时钟
void delay_init(u32 SYSCLK)
{
    SysTick->CTRL&=~(1<<2);  //SYSTICK使用外部时钟源     
    fac_us=SYSCLK/8;         //设置us下的计数
    fac_ms=fac_us*1000;      //设置ms下的计数值                           
}                                    

//延时nus
//nus为要延时的us数.                                               
void delay_us(u32 nus)
{        
    u32 count;             
    SysTick->LOAD=nus*fac_us; //时间加载               
    SysTick->VAL=0x00;        //清空计数器
    SysTick->CTRL=0x01 ;      //开始倒数      
    do
    {
        count=SysTick->CTRL;
    }
    while((count&0x01)&&!(count &(1<<16))); //等待时间到达   
    SysTick->CTRL=0x00;                     //关闭计数器
    SysTick->VAL =0X00;                     //清空计数器     
}

//延时nms
//注意nms的范围
//SysTick->LOAD为24位寄存器,所以,最大延时为:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK单位为Hz,nms单位为ms
//对72M条件下,nms<=1864 
void delay_ms(u32 nms)
{                     
    u32 temp1;           
    SysTick->LOAD=nms*fac_ms;     //时间加载(SysTick->LOAD为24bit)
    SysTick->VAL =0x00;           //清空计数器
    SysTick->CTRL=0x01 ;          //开始倒数  
    do
    {
        temp1=SysTick->CTRL;
    }
    while((temp1&0x01)&&!(temp1&(1<<16))); //等待时间到达   
    SysTick->CTRL=0x00;                    //关闭计数器
    SysTick->VAL =0X00;                    //清空计数器              
} 

             

























