//头文件调用
#include <stdio.h>
#include "stm32f10x_usart.h"
#include "usart.h"

//加入以下代码,支持printf函数,而不需要选择use MicroLIB      
#if 1
#pragma import(__use_no_semihosting)  

//标准库需要的支持函数                 
struct __FILE 
{ 
    int handle; 
    /* Whatever you require here. If the only file you are using is */ 
    /* standard output using printf() for debugging, no file handling */ 
    /* is required. */ 
}; 

/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout;   

//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
    x = x; 
} 

//重定义fputc函数 
int fputc(int Data, FILE *f)
{   
    while(!USART_GetFlagStatus(USART1,USART_FLAG_TXE));      //USART_GetFlagStatus：得到发送状态位
                                                             //USART_FLAG_TXE:发送寄存器为空 1：为空；0：忙状态
    USART_SendData(USART1,Data);                             //发送一个字符
       
    return Data;                                             //返回一个值
}
#endif 


//串口1发送一个字节函数
void USART1_Send_Byte(uint16_t Data)
{ 
    while(!USART_GetFlagStatus(USART1,USART_FLAG_TXE));      //USART_GetFlagStatus：得到发送状态位
                                                             //USART_FLAG_TXE:发送寄存器为空 1：为空；0：忙状态
    USART_SendData(USART1,Data);                             //发送一个字符
}

//串口1接收一个字节函数
uint8_t USART1_Receive_Byte(void)
{ 
    while(!(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)));   //USART_GetFlagStatus：得到接收状态位
                                                             //USART_FLAG_RXNE:接收数据寄存器非空标志位 
                                                             //1：忙状态  0：空闲(没收到数据，等待。。。)
    return USART_ReceiveData(USART1);                        //接收一个字符
}

//串口1初始化函数
void Init_Usart(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;                     //定义一个GPIO结构体变量
    
    //使能端口A的时钟及串口1的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1,ENABLE);    
                                                            

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                //配置串口接收端口挂接到9端口
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;          //复用功能输出开漏
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;        //配置端口速度为50M
    GPIO_Init(GPIOA, &GPIO_InitStructure);                   //根据参数初始化GPIOA寄存器    

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;    //浮空输入(复位状态);                       
    GPIO_Init(GPIOA, &GPIO_InitStructure);                   //根据参数初始化GPIOA寄存器    
}

//串口配置函数
void Usart_Configuration(uint32_t BaudRate)
{
    USART_InitTypeDef USART_InitStructure;                                             //定义一个串口结构体

    USART_InitStructure.USART_BaudRate            = BaudRate ;                         //波特率115200
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;               //传输过程中使用8位数据
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;                  //在帧结尾传输1位停止位
    USART_InitStructure.USART_Parity              = USART_Parity_No ;                  //奇偶校验禁能
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;    //硬件流控禁能
    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;     //接收和发送模式
    USART_Init(USART1, &USART_InitStructure);                                          //根据参数初始化串口寄存器
    USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);                                      //不使能串口中断接收
    USART_Cmd(USART1, ENABLE);                                                         //使能串口外设
}


