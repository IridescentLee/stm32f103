#ifndef __MY_CMD_H
#define __MY_CMD_H

#include "stm32f10x.h"

#define MY_CMD_BUFF_LEN 256  //���ջ����ֽ���

typedef struct
{
    u8      myCmdBuff[MY_CMD_BUFF_LEN];  //���ջ���
    u16     myCmdRxNum;                 //���յ����ַ�����
    u8      myCmdAck;                   //������ɣ���Ҫ���д���
}MyCmdStruct;

extern MyCmdStruct myCmdStrct;

void MyCommand( void);

#endif

