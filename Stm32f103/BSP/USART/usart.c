#include <stdio.h>

#include "stm32f10x.h"
#include "sys.h"
#include "USART\usart.h"
#include "my_cmd.h"


//加入以下代码,支持printf函数,而不需要选择use MicroLIB
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

/*使用microLib的方法*/
 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/

void Usart1Init(u32 bound)
{
    //GPIO端口设置
    GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
     //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

   //Usart1 NVIC 配置

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器USART1
  
   //USART 初始化设置
   
	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART1, &USART_InitStructure);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
   
    USART_Cmd(USART1, ENABLE);                    //使能串口

    printf("\r\nUsart1 configuration is ok, boundrate is %d.\r\n", bound);

}

void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 res;
	
	if(USART_GetITStatus( USART1, USART_IT_RXNE) != RESET)
	{
		res = USART_ReceiveData( USART1);//(USART1->DR);	//读取接收到的数据

		if( res == '\r')
        {
            Usart1PutChar('\n');
            Usart1PutChar('\r');          
            myCmdStrct.myCmdBuff[myCmdStrct.myCmdRxNum] = '\0';
            myCmdStrct.myCmdAck = 1;

            return;
        }

		if( res == '\b') /* Backspace */
        {
			if( myCmdStrct.myCmdRxNum > 0)
			{
				Usart1PutChar('\b');
				Usart1PutChar(' ');
				Usart1PutChar('\b');
				myCmdStrct.myCmdRxNum--;
			}
			return;          
        }

		if( myCmdStrct.myCmdRxNum >= MY_CMD_BUFF_LEN)
        {
			printf("Command string size overflow\r\n");
			myCmdStrct.myCmdRxNum = 0;
			return;
        }

		if ( res >= 0x20 && res <= 0x7E)
        {
			myCmdStrct.myCmdBuff[myCmdStrct.myCmdRxNum++] = res;
			Usart1PutChar( res);
        }   		 
	} 
} 

unsigned char Usart1PutChar( unsigned char ch)
{
	USART_SendData( USART1, (uint8_t) ch); /*发送一个字符函数*/ 

	/* Loop until the end of transmission */
	while (USART_GetFlagStatus( USART1, USART_FLAG_TC) == RESET)/*等待发送完成*/
	{}

	return ch;
}
