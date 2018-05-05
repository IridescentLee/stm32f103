#ifndef __MY_CMD_H
#define __MY_CMD_H

#include "stm32f10x.h"

#define MY_CMD_BUFF_LEN 256  //接收缓存字节数

typedef struct
{
    u8      myCmdBuff[MY_CMD_BUFF_LEN];  //接收缓存
    u16     myCmdRxNum;                 //接收到的字符数量
    u8      myCmdAck;                   //接收完成，需要进行处理
}MyCmdStruct;

extern MyCmdStruct myCmdStrct;

void MyCommand( void);

#endif

