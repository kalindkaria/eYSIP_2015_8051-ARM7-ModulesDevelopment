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

#include <intrins.h>
#include "p89v51rx2.h"
sbit buzzer=P2^7;

/* Function name - delay_ms
* Input - time in milli seconds 
* Outut - gives delay for the spcified time before executing next instruction
* Logic - delay can be given in the program by introducing loops
*/
void delay_ms(unsigned int ms)
{
unsigned int i,j;
for(i=0; i<ms; i++)
for(j=0; j<53; j++);
}

/*
* Function Name: main
* Input: None
* Output: int to inform the caller that the program exited correctly or incorrectly (C code standard)
* Logic: blows and stops the buzzer for specified time delays 
* Example Call: Called automatically by the Operating System
*
*/
void main (void)
{
buzzer=0;// turn on the buzzer
delay_ms(1000); //give delay of 1000 milliseconds
buzzer=1; // turn off the buzzer
delay_ms(1000);
}