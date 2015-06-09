/*
* Author: Bhumika Varshney, ERTS Lab, IIT Bombay
* File name: buzzer_on_reset_switch.c
* Functions: delay_ms(unsigned int)
* Global variable: buzzer, switch1
* This experiment demonstrates simple Input and Output operation.It takes input from a pin and gives output to another.
* Concepts covered: Input and Output operations
  
 Note: 
 
 1. Make sure that in the configuration options following settings are 
 done for proper operation of the code
  Microcontroller: P89V51RD2
 Frequency: 11.0592 MHz								   
3.  Coding Standards followed(For further reference on Coding Standards go to portal link).
*********************************************************************************/

#include <intrins.h>
#include "p89v51rx2.h"
sbit buzzer=P2^7;
sbit switch1=P3^3;
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
	switch1=1;	 //setting P3^3 as an input pin
	   while(1)
		{  		
			if(switch1==0)
			{
	   		buzzer=0;  //buzzer blows
			}
			else
			buzzer=1;	//buzzer stops
			
		}
}