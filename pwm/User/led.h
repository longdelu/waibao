#ifndef __led_H
#define __led_H

// PE5, 代表LED1;
#define LED1 PEout(5)     

// PB5, 代表LED0;
#define LED0 PBout(5)    

//LED初始化
void LED_Init(void);

#endif
