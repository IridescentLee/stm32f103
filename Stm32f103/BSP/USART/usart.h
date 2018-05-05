#ifndef __USART_H
#define __USART_H
#include "stdio.h"

//#define USART_BUFF_LEN 256  //接收缓存字节数
//
//typedef struct
//{
//    u8      usartBuff[USART_BUFF_LEN];  //接收缓存
//    u16     usartRxNum;                 //接收到的字符数量
//    u8      usartAck;                   //接收完成，需要进行处理
//}UsartStruct;


void Usart1Init(u32 bound);
unsigned char Usart1PutChar( unsigned char ch);

#endif
