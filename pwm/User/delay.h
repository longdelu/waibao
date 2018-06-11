#ifndef __DELAY_H
#define __DELAY_H                
      
extern void delay_init(u8 SYSCLK);
extern void delay_ms(u16 nms);
extern void delay_us(u32 nus);
//extern static u32  fac_us=0;//us延时倍乘数               
//extern static u32 fac_ms=0;//ms延时倍乘数
 
#endif





























