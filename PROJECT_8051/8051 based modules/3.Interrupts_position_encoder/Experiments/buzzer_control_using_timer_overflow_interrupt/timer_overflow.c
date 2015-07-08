/*
* Author: Bhumika Varshney, ERTS Lab, IIT Bombay
* File name: timer_overflow.c
* Functions: timer0_setup()
* Global variable: buzzer, unsigned int count
* This experiment demonstrates simple timer overflow interrupt operation.
* Concepts covered: Interrupts Control
  
 Note: 
 1. Make sure that in the configuration options following settings are 
 done for proper operation of the code
  Microcontroller: P89V51RD2
 Frequency: 11.0592 MHz								   
2.  Coding Standards followed(For further reference on Coding Standards go to portal link).
********************************************************************************************/
#include <intrins.h>
#include "p89v51rx2.h"
sbit buzzer=P2^7;  //buzzer = 1; buzzer off, buzzer = 0; buzzer on,
unsigned int count=1;  //count used in ISR
/****************************************************************
* Function: timer0_setup
* Input: none
* Output: none
* Logic: initializes the interrupt registers to set 
*        required count for the counter
*****************************************************************/
void  timer0_setup(void)   
{
 TMOD=0x01; // Timer 0 in 16 bit external counter mode
 TH0=0x4b; // reset counter value to 0       
 TL0=0xFC;	// reset counter value to 0  
}
/****************************************************************
* Function: Interrupt Service Routine
* Input: none
* Output: none
* Logic: whenever interrupt0 occurs it executes this routine
*        and after this it returns to the main function
*****************************************************************/
void int1_isr(void)interrupt 1
 {
 	if(count==40)//to create a delay of 2 seconds(50x40= 2000ms= 2second)
	{
 	buzzer=~buzzer;	//if buzzer is on it will stop and vicevirsa
	TR0=0; //stop the timer
    TF0=0; //clear timer overflow flag
    TH0=0x4b; // reset counter value to 0       
    TL0=0xFC;	// reset counter value to 0
	count=1; //reset the count value to 1 
    TR0=1; //start the timer
	
	}
	else
	count++; 
 }
 /*
* Function Name: main
* Input: None
* Output: int to inform the caller that the program exited correctly or incorrectly (C code standard)
* Logic: initializes the timer and enable all interrupts 
* Example Call: Called automatically by the Operating System
*
*/ 
void main (void)
 {
   timer0_setup(); //initialize the timer
   IEN0=0x82;	   //enable interrupts
   TR0=1; 		 //start timer
   while(1);
 } //main ends here
