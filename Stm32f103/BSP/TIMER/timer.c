#include "stdio.h"

#include "stm32f10x.h"

#include "TIMER\timer.h"
#include "LED\led.h"

#define TIMER_CNT_FCT 10    //按照定时器分频系数，计数10次为1ms

MyClockStrct clockStrct = {0};


/************************************************************************/
/*
FunctionName    :Timer3Init
Description     :定时器3初始化，并开启中断
para1           :period 计数周期
return          :None
Author/Ver/Date :LY/1.00/20180429
*/
/************************************************************************/

void Timer3Init( u32 period)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period        = period; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 (period/10) ms
	TIM_TimeBaseStructure.TIM_Prescaler     = (720-1);//设置用来作为TIMx时钟频率除数的预分频值1Mhz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig( TIM3, TIM_IT_Update|TIM_IT_Trigger, ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设

    printf("\r\nTimer3 configuration is ok, period is %dms.\r\n", period/10);
							 
}

void TIM3_IRQHandler(void)   //TIM3中断
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 

        clockStrct.clock1PrdCnt++;
        if( clockStrct.clock1PrdCnt >= TIMER_CNT_FCT)   //1ms
        {
            clockStrct.clock1PrdCnt = 0;
            clockStrct.clock1ms     = 1;
            clockStrct.clock1msCnt++;
        }
        if( clockStrct.clock1msCnt >= 10)   //10ms
        {
            clockStrct.clock1msCnt  = 0;
            clockStrct.clock10ms    = 1;
            clockStrct.clock10msCnt++;
        }
        if( clockStrct.clock10msCnt >= 10)  //100ms
        {
            clockStrct.clock10msCnt = 0;
            clockStrct.clock100ms   = 1;
            clockStrct.clock100msCnt++;
        }
        if( clockStrct.clock100msCnt >= 5) //500ms
        {
            clockStrct.clock100msCnt    = 0;
            clockStrct.clock500ms       = 1;
            clockStrct.clock500msCnt++;
        }
        if( clockStrct.clock500msCnt >= 2)  //1s
        {
            clockStrct.clock500msCnt    = 0;
            clockStrct.clock1s          = 1;
            clockStrct.clock1sCnt++;
        }
    }
}












