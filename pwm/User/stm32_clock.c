#include "stm32f10x.h"
#include "stm32f10x.h"

//ϵͳʱ�ӳ�ʼ������
//pll:ѡ��ı�Ƶ������2��ʼ�����ֵΪ16    
void Stm32_Clock_Init(u8 PLL)
{
    unsigned char temp2=0; 
    RCC->APB1RSTR = 0x00000000;          
    RCC->APB2RSTR = 0x00000000;   //���踴λ
    RCC->CR|=0x00010000;          //�ⲿ����ʱ��ʹ��HSEON
    while(!(RCC->CR>>17));        //�ȴ��ⲿʱ�Ӿ���
    RCC->CFGR=0X00000400;         //APB1=DIV2;APB2=DIV1;AHB=DIV1;
    PLL-=2;                       //����2����λ
    RCC->CFGR|=PLL<<18;           //����PLL��Ƶֵ 9��
    RCC->CFGR|=1<<16;             //PLLSRC ON ��ѡ���ⲿ������PLL����
    FLASH->ACR|=0x32;             //ʹ��72Mʱ�ӣ�FLASH 2����ʱ����
    RCC->CR|=0x01000000;          //PLLON
    while(!(RCC->CR>>25));        //�ȴ�PLL����
    RCC->CFGR|=0x00000002;        //PLL��Ϊϵͳʱ��     
    while(temp2!=0x02)            //�ȴ�PLL��Ϊϵͳʱ�����óɹ�
    {   
        temp2=RCC->CFGR>>2;
        temp2&=0x03;
    }    
}    

