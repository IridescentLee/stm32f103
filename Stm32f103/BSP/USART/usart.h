#ifndef __USART_H
#define __USART_H
#include "stdio.h"

//#define USART_BUFF_LEN 256  //���ջ����ֽ���
//
//typedef struct
//{
//    u8      usartBuff[USART_BUFF_LEN];  //���ջ���
//    u16     usartRxNum;                 //���յ����ַ�����
//    u8      usartAck;                   //������ɣ���Ҫ���д���
//}UsartStruct;


void Usart1Init(u32 bound);
unsigned char Usart1PutChar( unsigned char ch);

#endif
