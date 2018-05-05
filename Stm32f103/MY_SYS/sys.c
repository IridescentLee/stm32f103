#include "sys.h"
#include "delay.h"
#include "LED\led.h"
#include "USART\usart.h"
#include "TIMER\timer.h"
#include "my_cmd.h"
#include "LCD\lcd.h" 


void NVIC_Configuration( void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
}

void BoardInit( void)
{
	NVIC_Configuration();//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	Usart1Init( 9600);//串口初始化为9600
	delay_init( 72);
	LedInit();
    LCD_Init();

    Timer3Init( 10);
}

void Do1ms( void)
{}

void Do10ms( void)
{}

void Do100ms( void)
{
    MyCommand();
}

void Do500ms( void)
{
//    LED0 = ~LED0;
    LED1 = ~LED1;
}

void Do1s()
{
    static u8 x = 0;
    
    switch(x)
	{
		case 0:LCD_Clear(WHITE);break;
		case 1:LCD_Clear(BLACK);break;
		case 2:LCD_Clear(BLUE);break;
		case 3:LCD_Clear(RED);break;
		case 4:LCD_Clear(MAGENTA);break;
		case 5:LCD_Clear(GREEN);break;
		case 6:LCD_Clear(CYAN);break;
		case 7:LCD_Clear(YELLOW);break;
		case 8:LCD_Clear(BRRED);break;
		case 9:LCD_Clear(GRAY);break;
		case 10:LCD_Clear(LGRAY);break;
		case 11:LCD_Clear(BROWN);break;
	}
    
	POINT_COLOR=RED;
	LCD_ShowString(30,50,"Mini STM32 ^_^");
	LCD_ShowString(30,70,"2.4'/2.8' TFTLCD TEST");
	LCD_ShowString(30,90,"LY");
	LCD_ShowString(30,110,"2018/5/3");
    x++;
	if(x==12)x=0;
	LED0=!LED0;
}

void StateFlow( void)
{
    if( clockStrct.clock1ms == 1)
    {
        Do1ms();
        clockStrct.clock1ms = 0;
    }

    if( clockStrct.clock10ms == 1)
    {
        Do10ms();
        clockStrct.clock10ms = 0;
    }

    if( clockStrct.clock100ms == 1)
    {
        Do100ms();
        clockStrct.clock100ms = 0;
    }

    if( clockStrct.clock500ms == 1)
    {
        Do500ms();
        clockStrct.clock500ms = 0;
    }

    if( clockStrct.clock1s == 1)
    {
        Do1s();
        clockStrct.clock1s = 0;
    }
}

