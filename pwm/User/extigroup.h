#ifndef __extigroup_H
#define __extigroup_H
#include <stm32f10x.h>
#include <misc.h>    
#define GPIO_A 0
#define GPIO_B 1
#define GPIO_C 2
#define GPIO_D 3
#define GPIO_E 4
#define GPIO_F 5
#define GPIO_G 6 
#define FTIR   1  //下降沿触发
#define RTIR   2  //上升沿触发
extern void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group);
extern void MY_NVIC_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group);
extern void Ex_NVIC_Config(u8 GPIOx,u8 BITx,u8 TRIM) ;
#endif


