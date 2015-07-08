#include <intrins.h>

sbit buzzer=P2^7;

/******Function Prototypes**********/
void buzzer_on(void);
void buzzer_off(void);
/**************************************/

void buzzer_on(void)
{
	buzzer=0;
}

void buzzer_off(void)
{
	buzzer=1;
}