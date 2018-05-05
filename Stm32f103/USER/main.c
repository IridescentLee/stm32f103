#include <stdio.h>

#include "delay.h"
#include "sys.h"
#include "LED\led.h"
#include "KEY\key.h"
#include "USART\usart.h"

 int main( void)
 {
    BoardInit();

    while( 1)
    {		
        StateFlow();
    }	 

 }

