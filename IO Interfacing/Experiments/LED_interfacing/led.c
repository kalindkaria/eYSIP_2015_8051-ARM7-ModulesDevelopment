/*
* Author: Bhumika Varshney, ERTS Lab, IIT Bombay
* File name: led.c
* Functions: delay_ms(unsigned int)
* Global variable: led
* This experiment demonstrates simple Input and Output operation. 
*LED is connected to 2nd pin of port 3 hence to glow it, we have to give logic 1 to it.
* Concepts covered: Input and Output operations
  
 Note: 								    
 1. Make sure that in the configuration options following settings are 
 done for proper operation of the code
  Microcontroller: P89V51RD2
 Frequency: 11.0592 MHz								   
 2.  Coding Standards followed(For further reference on Coding Standards go to portal link).
*********************************************************************************/
#include<intrins.h>
#include"p89v51rx2.h"
sbit led=P3^1;
/* Function name - delay_ms
* Input - time in milli seconds 
* Outut - gives delay for the spcified time before executing next instruction
* Logic - delay can be given in the program by introducing loops
*/
void delay_ms(unsigned int ms)
{
unsigned int i,j;

for(i=0;i<ms;i++)
for(j=0;j<53;j++);

}
/*
* Function Name: main
* Input: None
* Output: int to inform the caller that the program exited correctly or incorrectly (C code standard)
* Logic: glows led for specified time delay than turns it off 
* Example Call: Called automatically by the Operating System
*
*/
void main()
{
	led=1;	   //led is on
	delay_ms(1000);
	led=0;	   //led is off
	while(1);
}