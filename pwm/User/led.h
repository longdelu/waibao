#ifndef __led_H
#define __led_H

// PE5, ����LED1;
#define LED1 PEout(5)     

// PB5, ����LED0;
#define LED0 PBout(5)    

//LED��ʼ��
void LED_Init(void);

#endif
