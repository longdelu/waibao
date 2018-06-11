#include "stm32f10x.h"
#include "stm32f10x.h"

//系统时钟初始化函数
//pll:选择的倍频数，从2开始，最大值为16    
void Stm32_Clock_Init(u8 PLL)
{
    unsigned char temp2=0; 
    RCC->APB1RSTR = 0x00000000;          
    RCC->APB2RSTR = 0x00000000;   //外设复位
    RCC->CR|=0x00010000;          //外部高速时钟使能HSEON
    while(!(RCC->CR>>17));        //等待外部时钟就绪
    RCC->CFGR=0X00000400;         //APB1=DIV2;APB2=DIV1;AHB=DIV1;
    PLL-=2;                       //抵消2个单位
    RCC->CFGR|=PLL<<18;           //设置PLL倍频值 9倍
    RCC->CFGR|=1<<16;             //PLLSRC ON ，选择外部晶振做PLL输入
    FLASH->ACR|=0x32;             //使用72M时钟，FLASH 2个延时周期
    RCC->CR|=0x01000000;          //PLLON
    while(!(RCC->CR>>25));        //等待PLL锁定
    RCC->CFGR|=0x00000002;        //PLL作为系统时钟     
    while(temp2!=0x02)            //等待PLL作为系统时钟设置成功
    {   
        temp2=RCC->CFGR>>2;
        temp2&=0x03;
    }    
}    

