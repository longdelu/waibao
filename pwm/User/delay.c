#include "stm32f10x.h"

static u32  fac_us=0;  //us��ʱ������               
static u32 fac_ms=0;  //ms��ʱ������
 
//��ʼ���ӳٺ���
//SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
//SYSCLK:ϵͳʱ��
void delay_init(u32 SYSCLK)
{
    SysTick->CTRL&=~(1<<2);  //SYSTICKʹ���ⲿʱ��Դ     
    fac_us=SYSCLK/8;         //����us�µļ���
    fac_ms=fac_us*1000;      //����ms�µļ���ֵ                           
}                                    

//��ʱnus
//nusΪҪ��ʱ��us��.                                               
void delay_us(u32 nus)
{        
    u32 count;             
    SysTick->LOAD=nus*fac_us; //ʱ�����               
    SysTick->VAL=0x00;        //��ռ�����
    SysTick->CTRL=0x01 ;      //��ʼ����      
    do
    {
        count=SysTick->CTRL;
    }
    while((count&0x01)&&!(count &(1<<16))); //�ȴ�ʱ�䵽��   
    SysTick->CTRL=0x00;                     //�رռ�����
    SysTick->VAL =0X00;                     //��ռ�����     
}

//��ʱnms
//ע��nms�ķ�Χ
//SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK��λΪHz,nms��λΪms
//��72M������,nms<=1864 
void delay_ms(u32 nms)
{                     
    u32 temp1;           
    SysTick->LOAD=nms*fac_ms;     //ʱ�����(SysTick->LOADΪ24bit)
    SysTick->VAL =0x00;           //��ռ�����
    SysTick->CTRL=0x01 ;          //��ʼ����  
    do
    {
        temp1=SysTick->CTRL;
    }
    while((temp1&0x01)&&!(temp1&(1<<16))); //�ȴ�ʱ�䵽��   
    SysTick->CTRL=0x00;                    //�رռ�����
    SysTick->VAL =0X00;                    //��ռ�����              
} 

             

























