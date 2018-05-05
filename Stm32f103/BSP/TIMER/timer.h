#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

typedef struct
{
    u8 clock1PrdCnt;
    
    u8 clock1msCnt;
    u8 clock10msCnt;
    u8 clock100msCnt;
    u8 clock500msCnt;
    u32 clock1sCnt;
    
    u8 clock1ms;
    u8 clock10ms;
    u8 clock100ms;
    u8 clock500ms;
    u8 clock1s;
}MyClockStrct;

extern MyClockStrct clockStrct;

void Timer3Init( u32 period);

#endif

