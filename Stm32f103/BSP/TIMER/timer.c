#include "stdio.h"

#include "stm32f10x.h"

#include "TIMER\timer.h"
#include "LED\led.h"

#define TIMER_CNT_FCT 10    //���ն�ʱ����Ƶϵ��������10��Ϊ1ms

MyClockStrct clockStrct = {0};


/************************************************************************/
/*
FunctionName    :Timer3Init
Description     :��ʱ��3��ʼ�����������ж�
para1           :period ��������
return          :None
Author/Ver/Date :LY/1.00/20180429
*/
/************************************************************************/

void Timer3Init( u32 period)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period        = period; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 (period/10) ms
	TIM_TimeBaseStructure.TIM_Prescaler     = (720-1);//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ1Mhz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig( TIM3, TIM_IT_Update|TIM_IT_Trigger, ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����

    printf("\r\nTimer3 configuration is ok, period is %dms.\r\n", period/10);
							 
}

void TIM3_IRQHandler(void)   //TIM3�ж�
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 

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












