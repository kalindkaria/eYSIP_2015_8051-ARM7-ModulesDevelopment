/*
* Author: Bhumika Varshney, ERTS Lab, IIT Bombay
* File name: buzzertest.c
* Functions: delay_ms(unsigned int)
* Global variable: buzzer
* This experiment demonstrates simple Input and Output operation.
* Concepts covered: Input and Output operations
  
 Note: 
 
 1. Make sure that in the configuration options following settings are 
 done for proper operation of the code
  Microcontroller: P89V51RD2
 Frequency: 11.0592 MHz								   
 2. Buzzer is connected to the 8th pin of Port 2 i.e P2.7, to turn it on we have to set P2^7 to zero
3.  Coding Standards followed(For further reference on Coding Standards go to portal link).
*********************************************************************************/
#include <lpc214x.h>
/**********Macros******************/
#define BUZZER_OFF() IO0CLR=(1<<25) //Macro to turn OFF buzzer
#define BUZZER_ON() IO0SET=(1<<25) //Macro to turn ON buzzer
/***********************************/
/****Function Prototypes**********/
void Buzzer_Delay(unsigned int);
void Init_Buzzer_Pin(void);
void Init_Peripherals(void);
void Init_Ports(void);
/***********************************/
/************************************************************
Function : Buzzer_Delay
Return type : None
Parameters : None
Description : Provides small amount of delay between
buzzer toggles.
************************************************************/
void Buzzer_Delay(unsigned int ms)
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
void Init_Ports(void)
{
Init_Buzzer_Pin();
}
void Init_Peripherals(void)
{
Init_Ports();
}
/**************************************************************************** 
*Function name - delay_ms
* Input - time in milli seconds 
* Outut - gives delay for the spcified time before executing next instruction
* Logic - delay can be given in the program by introducing loops
******************************************************************************/
void delay_ms(unsigned int ms)
{
unsigned int i,j;

for(i=0;i<ms;i++)
for(j=0;j<53;j++);
}
int main(void)
{
PINSEL0 = 0x00000000; // Reset all pins as GPIO
PINSEL1 = 0x00000000;
PINSEL2 = 0x00000000;
Init_Peripherals(); // Init Port pins and Peripherals
while(1)
{
BUZZER_ON(); //Turn ON buzzer
Buzzer_Delay(1000); //Wait
BUZZER_OFF(); //Turn OFF Buzzer
Buzzer_Delay(1000); //Wait
}
}
