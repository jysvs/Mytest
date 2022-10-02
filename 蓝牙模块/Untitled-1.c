
#include "lanya.h"
 
u8 res;
void lanya_Init(void)
{
        
    GPIO_InitTypeDef GPIO_InitStrue;  
    USART_InitTypeDef USART_InitStrue;  
    NVIC_InitTypeDef NVIC_InitStrue;  
      
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);// 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//   
     
    GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;  
    GPIO_InitStrue.GPIO_Pin=GPIO_Pin_2;  
    GPIO_InitStrue.GPIO_Speed=GPIO_Speed_10MHz;  
    GPIO_Init(GPIOA,&GPIO_InitStrue); 
	
		
    GPIO_InitStrue.GPIO_Mode=GPIO_Mode_IN_FLOATING;  
    GPIO_InitStrue.GPIO_Pin=GPIO_Pin_3;  
    GPIO_InitStrue.GPIO_Speed=GPIO_Speed_10MHz;  
    GPIO_Init(GPIOA,&GPIO_InitStrue);
	
    USART_InitStrue.USART_BaudRate=9600;  //波特率设置9600，串口设置
    USART_InitStrue.USART_HardwareFlowControl=USART_HardwareFlowControl_None;  
    USART_InitStrue.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;  
    USART_InitStrue.USART_Parity=USART_Parity_No;  
    USART_InitStrue.USART_StopBits=USART_StopBits_1;  
    USART_InitStrue.USART_WordLength=USART_WordLength_8b;  
      
    USART_Init(USART2,&USART_InitStrue);
      
    USART_Cmd(USART2,ENABLE);     //
      
    USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//
      
    NVIC_InitStrue.NVIC_IRQChannel=USART2_IRQn;  
    NVIC_InitStrue.NVIC_IRQChannelCmd=ENABLE;  
    NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority=0;  
    NVIC_InitStrue.NVIC_IRQChannelSubPriority=1;  
    NVIC_Init(&NVIC_InitStrue);  
      
}  
  
void USART2_IRQHandler(void)  
{  
 
     if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)  
 {  
     res= USART_ReceiveData(USART2);   //接收数据给res
//     USART_SendData(USART2,res);    
  }  
}
//这里主要还是初始化串口，即蓝牙初始化，后面又开启串口2中断接受数据吗，
//这里因为我采用了串口2，所以并没有开启发送到电脑的串口助手上，我们仅通过与手机软件的互联，这也是我们通过手机端来控制LED的亮灭，进而实现其他高阶操作。

//在这里RXD接单片机上串口的TXD，TXD接单片机上串口的RXD，VCC和GND就不用多说了吧哈哈

//下面代码里面我接的是串口2，即PA2（TXD）和PA3（RXD)


//主函数部分

#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "lanya.h"
 
extern u8 res;         
 int main(void)
 {   
 int i;
 char a[]="abcdefg";
 
 delay_init();
 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
 uart_init(115200);        
 LED_Init();            
 lanya_Init();
  while(1)
 {
  if(res=='0')
  {
   LED0=0;
   delay_ms(500);    
   for(i=0;i<7;i++)
   {
    USART_SendData(USART2, a[i]); 
  while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
		 
   }
   res=0;
  }
  if(res=='1')
  {
   LED1=0;
   delay_ms(50);
   res=0;
  }
	if(res=='2')
	{
		LED0=1;
        delay_ms(50);
		res=0;
	}
		if(res=='3')
	{
		LED1=1;
        delay_ms(50);
		res=0;
	}
 
 }  
}
////////////////////////////////////////////////////////////////////////////////


//STM32f103c8t6控制蓝牙模块控制led灯实现亮灭
#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"

extern u8 res;										//设置外部全局变量
 int main(void)
 {		 
	int i;
	char a[]="abcdefg";
	char b[7]={'1','2','3','4','5','6','7'};
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 							//串口初始化为115200
	My_USART2_Init();
 	LED_Init();			     						//LED端口初始化
 	while(1)
	{
		if(res=='1')
		{
			GPIO_ResetBits(GPIOA,GPIO_Pin_1);		//PA.1指示灯亮
			delay_ms(500);							//假如要用延时函数，最好就在上面初始化延时函数，不然程序很容易就跑飞
			for(i=0;i<7;i++)
			{
				USART_SendData(USART2, a[i]);			//向串口2发送数据给蓝牙模块
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
			}
			res=0;
		}
		if(res=='2')
		{
			GPIO_SetBits(GPIOA,GPIO_Pin_1);			//PA.1指示灯灭
			res=0;
		}
	}	 
 }

u8 res;  				  //设置全局变量
void My_USART2_Init(void)  
{  
    GPIO_InitTypeDef GPIO_InitStrue;  
    USART_InitTypeDef USART_InitStrue;  
    NVIC_InitTypeDef NVIC_InitStrue;  
      
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//GPIO端口使能  
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//串口端口使能  
      
    GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;  
    GPIO_InitStrue.GPIO_Pin=GPIO_Pin_2;  
    GPIO_InitStrue.GPIO_Speed=GPIO_Speed_10MHz;  
    GPIO_Init(GPIOA,&GPIO_InitStrue);  
      
    GPIO_InitStrue.GPIO_Mode=GPIO_Mode_IN_FLOATING;  
    GPIO_InitStrue.GPIO_Pin=GPIO_Pin_3;  
    GPIO_InitStrue.GPIO_Speed=GPIO_Speed_10MHz;  
    GPIO_Init(GPIOA,&GPIO_InitStrue);  
      
    USART_InitStrue.USART_BaudRate=9600;  
    USART_InitStrue.USART_HardwareFlowControl=USART_HardwareFlowControl_None;  
    USART_InitStrue.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;  
    USART_InitStrue.USART_Parity=USART_Parity_No;  
    USART_InitStrue.USART_StopBits=USART_StopBits_1;  
    USART_InitStrue.USART_WordLength=USART_WordLength_8b;  
      
    USART_Init(USART2,&USART_InitStrue);
      
    USART_Cmd(USART2,ENABLE);					//使能串口2  
      
    USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//开启接收中断  
      
    NVIC_InitStrue.NVIC_IRQChannel=USART2_IRQn;  
    NVIC_InitStrue.NVIC_IRQChannelCmd=ENABLE;  
    NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority=0;  
    NVIC_InitStrue.NVIC_IRQChannelSubPriority=1;  
    NVIC_Init(&NVIC_InitStrue);  
      
}  
  
void USART2_IRQHandler(void)  
{  

     if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)  
 {  
     res= USART_ReceiveData(USART2); 	 
//     USART_SendData(USART2,res);     //串口2发送数据给蓝牙模块接收，也就是手机app接收到的数据
  }  
}

void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PA端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				 //LED-->PA.1 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.1
}

//版权声明：本文为CSDN博主「i土豆」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
//原文链接：https://blog.csdn.net/weixin_44069765/article/details/109820055
/////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
