
#include <lpc214x.h>
/**********Macros******************/
#define BUZZER_OFF() IO0CLR=(1<<25) //Macro to turn OFF buzzer
#define BUZZER_ON() IO0SET=(1<<25) //Macro to turn ON buzzer

/****Function Prototypes**********/
void delay_ms(unsigned int);
void Init_Buzzer_Pin(void);
/***************************************************************************** 
* Function name - Buzer_Delay
* Input - time in milli seconds 
* Outut - gives delay for the spcified time before executing next instruction
* Logic - delay can be given in the program by introducing loops
******************************************************************************/
void delay_ms(unsigned int ms)
{
unsigned int i,j;
for(j=0;j<(ms/10);j++)
{
for(i=0; i<60000; i++);
}
}
/************************************************************
Function : Init_Buzzer_Pin
Return type : None
Parameters : None
Description : Initialises Buzzer pin
************************************************************/
void Init_Buzzer_Pin(void)
{
PINSEL1&=0xFFF3FFFF;
PINSEL1|=0x00000000; //Set P0.25 as GPIO
IO0DIR&=0xFDFFFFFF;
IO0DIR|= (1<<25); //Set P0.25 as Output
BUZZER_OFF(); //Initially turn OFF buzzer
}

