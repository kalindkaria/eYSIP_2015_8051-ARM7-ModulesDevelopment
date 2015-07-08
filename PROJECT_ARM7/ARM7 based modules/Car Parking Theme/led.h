  /*
* Author: Bhumika Varshney, ERTS Lab, IIT Bombay
* File name: interfacing_led.c
* Description: This experiment demonstrates simple Input and Output operation. 
* Functions: delay_ms(unsigned int), Init_Led_Pin(void), Init_Peripherals(void), Init_Ports(void)
* Concepts covered: Input and Output operations
  
 Note: 
 
 1. Make sure that in the configuration options following settings are 
 done for proper operation of the code
  Microcontroller: LPC2148
 Frequency: 12 MHz								   
 2. Led is connected to the 4th pin of Port0 i.e P0.4 using expansion slot, to turn it 
    on we have to set it as output and give logic 1 to it.
 3. Coding Standards followed(For further reference on Coding Standards go to portal link).
**********************************************************************************************/
#include <lpc214x.h>
/**********Macros******************/
#define LED_OFF() IO0CLR=(1<<4) //Macro to turn OFF Led
#define LED_ON() IO0SET=(1<<4) //Macro to turn ON Led

/****Function Prototypes**********/

void Init_Led_Pin(void);

/************************************************************
Function : Init_Led_Pin
Return type : None
Parameters : None
Description : Initialises Led pin
************************************************************/
void Init_Led_Pin(void)
{
PINSEL0&=0xFFFFFCFF;
PINSEL0|=0x00000000; //Set P0.4 as GPIO
IO0DIR&=0xFFFFFFEF;
IO0DIR|= (1<<4); //Set P0.4 as Output
}
