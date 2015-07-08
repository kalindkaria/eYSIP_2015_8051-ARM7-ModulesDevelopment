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
void delay_ms(unsigned int);
void Init_Led_Pin(void);
void Init_Peripherals(void);
void Init_Ports(void);

/***************************************************************************** 
* Function name - delay_ms
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
/***********************************************************
* Function     : Init_Ports
* Input        : none
* Output       : none
* Logic        : initializes ports 
* Example call : called by the main function
***********************************************************/
void Init_Ports(void)
{
Init_Led_Pin();
}
/***********************************************************
* Function     : Init_Peripherals
* Input        : none
* Output       : none
* Logic        : to initialize peripherals, in this case it 
*                is buzzer so it will call the function to 
*                initialize buzzer port 
* Example call : called by the main function
***********************************************************/
void Init_Peripherals(void)
{
Init_Ports();
}

/*********************************************************************
* Function Name	: main
* Input			: None
* Output		: int to inform the caller that the program exited 
                  correctly or incorrectly (C code standard)
* Logic			: glows and stops the LED for specified time delays 
* Example Call	: Called automatically by the Operating System
*
**********************************************************************/
int main(void)
{
PINSEL0 = 0x00000000; // Reset all pins as GPIO
PINSEL1 = 0x00000000;
PINSEL2 = 0x00000000;
Init_Peripherals(); // Init Port pins and Peripherals
while(1)
{
LED_ON(); //Turn ON LED
delay_ms(1000); //Wait
LED_OFF(); //Turn OFF LED
delay_ms(1000); //Wait
} 
} //main ends here
