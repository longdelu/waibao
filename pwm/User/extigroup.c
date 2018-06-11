#include<extigroup.h>

//NVIC_Group:NVIC分组 0~4 总共5组            
void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group)     
{ 
    u32 temp,temp1;      
    temp1=(~NVIC_Group)&0x07;//取后三位
    temp1<<=8;
    temp=SCB->AIRCR;  //读取先前的设置
    temp&=0X0000F8FF; //清空先前分组
    temp|=0X05FA0000; //写入钥匙
    temp|=temp1;       
    SCB->AIRCR=temp;  //设置分组                                 
}

//设置NVIC 
//NVIC_PreemptionPriority:抢占优先级
//NVIC_SubPriority       :响应优先级
//NVIC_Channel           :中断编号
//NVIC_Group             :中断分组 0~4
//注意优先级不能超过设定的组的范围!否则会有意想不到的错误
//组划分:
//组0:0位抢占优先级,4位响应优先级
//组1:1位抢占优先级,3位响应优先级
//组2:2位抢占优先级,2位响应优先级
//组3:3位抢占优先级,1位响应优先级
//组4:4位抢占优先级,0位响应优先级
//NVIC_SubPriority和NVIC_PreemptionPriority的原则是,数值越小,越优先       
void MY_NVIC_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group)     
{ 
    u32 temp;    
    u8 IPRADDR=NVIC_Channel/4;               //每组只能存4个,得到组地址 
    u8 IPROFFSET=NVIC_Channel%4;             //在组内的偏移
    IPROFFSET=IPROFFSET*8+4;                 //得到偏移的确切位置
    MY_NVIC_PriorityGroupConfig(NVIC_Group); //设置分组
    temp=NVIC_PreemptionPriority<<(4-NVIC_Group);      
    temp|=NVIC_SubPriority&(0x0f>>NVIC_Group);
    temp&=0xf;//取低四位

    if(NVIC_Channel<32)NVIC->ISER[0]|=1<<NVIC_Channel; //使能中断位(要清除的话,相反操作就OK)
    else NVIC->ISER[1]|=1<<(NVIC_Channel-32);    
    NVIC->IP[IPRADDR]|=temp<<IPROFFSET;                //设置响应优先级和抢断优先级                                    
}

//外部中断配置函数
//只针对GPIOA~G;不包括PVD,RTC和USB唤醒这三个
//参数:
//GPIOx:0~6,代表GPIOA~G
//BITx:需要使能的位;
//TRIM:触发模式,1,下升沿;2,上降沿;3，任意电平触发
//该函数一次只能配置1个IO口,多个IO口,需多次调用
//该函数会自动开启对应中断,以及屏蔽线           
void Ex_NVIC_Config(u8 GPIOx,u8 BITx,u8 TRIM) 
{
    u8 EXTADDR;
    u8 EXTOFFSET;
    EXTADDR=BITx/4;       //得到中断寄存器组的编号以级所对应的中断线
    EXTOFFSET=(BITx%4)*4; //GPIOx_pin偏移设置
                           
    RCC->APB2ENR|=0x01;   //使能io复用时钟

    AFIO->EXTICR[EXTADDR]&=~(0x000F<<EXTOFFSET); //清除原来中断控制寄存器中相应中断线x上的设置
    AFIO->EXTICR[EXTADDR]|=GPIOx<<EXTOFFSET;     //映射到相对应的GPIOx的中断线
    
    //自动设置
    EXTI->IMR|=1<<BITx;               //开启line BITx上的中断
    //EXTI->EMR|=1<<BITx;             //不屏蔽line BITx上的事件 (如果不屏蔽这句,在硬件上是可以的,但是在软件仿真的时候无法进入中断!)
     if(TRIM&0x01)EXTI->FTSR|=1<<BITx;//line BITx上事件下降沿触发
    if(TRIM&0x02)EXTI->RTSR|=1<<BITx; //line BITx上事件上升降沿触发
} 

