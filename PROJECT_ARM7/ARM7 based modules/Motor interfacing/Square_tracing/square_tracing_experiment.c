/*
* Author: Bhumika Varshney, ERTS Lab, IIT Bombay
* File name: square_tracing_experiment.c
* Functions: delay_ms(unsigned int), forward(), right(), stop()
* This experiment demonstrates simple motion control operation.
* Concepts covered: Motor interfacing 
  
 Note: 
 1. Make sure that in the configuration options following settings are 
 done for proper operation of the code
  Microcontroller: LPC2148
 Frequency: 12 MHz								   
2.  Coding Standards followed(For further reference on Coding Standards go to portal link).
*********************************************************************************/
#include<lpc214x.h>
/************************************************************
	Function 		: Init_Motion_Pin
	Input			: None
	Output			: None
	Description 	: Initialises Motion control pins
	Example call	: called in main function
************************************************************/

void Init_motion_pin(void)
{
 PINSEL0&=0xFF0F3FFF;	//Set Port pins P0.7, P0.10, P0.11 as GPIO
 PINSEL1&=0xFFFFF0FF;	//Set Port pins P0.21 and 0.22 as GPIO
 IO0DIR&=0xFF9FF37F;
 IO0DIR|= (1<<10) | (1<<11) | (1<<21) | (1<<22) | (1<<7); 	//Set Port pins P0.10, P0.11, P0.21, P0.22, P0.7 as Output pins
 IO1DIR&=0xFFDFFFFF;
 IO1DIR|= (1<<21);		// Set P1.21 as output pin
 IO0SET = 0x00200080;	// Set PWM pins P0.7/PWM2 and P0.21/PWM5 to logic 1
}
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
//These are the direction functions to move the motors in various directions
/* Function name - forward
* Input - none 
* Output - moves the robot in forward direction
* Logic - set appropriate bits high to move forward as per the logic table
*/
void forward()
{
	IO0SET|= (1<<10);
	IO1SET|=(1<<21);
	IO0CLR|=(1<<22)|(1<<11);
}
/* Function name - right
* Input - none 
* Output - turns the robot in right direction
* Logic - set appropriate bits high to turn right as per the logic table
*/

void right()
{
	IO0CLR|= (1<<10);
	IO1SET|=(1<<21);
	IO0CLR|=(1<<22);
	IO0SET|=(1<<11);
}
/* Function name - stop
* Input - none 
* Output - stops the robot
* Logic - set appropriate bits high to stop all motors as per the logic table
*/
void stop(void)
{
	IO0CLR|=(1<<10)|(1<<11)|(1<<22);
	IO1CLR|=(1<<21);
}
/**************************************************************
* Function Name: main
* Input: None
* Output: int to inform the caller that the program exited 
*         correctly or incorrectly (C code standard)
* Logic: calls the diection functions and delay function to 
*        trace square in clockwise direction.
* Example Call: Called automatically by the Operating System
*
****************************************************************/
int main()
{
	int count=1;
	Init_motion_pin();	//initialize motor pins
  	while(count<5)
	{	 
		forward(); //go forward
		delay_ms(2000); //delay of 2 seconds
		stop(); //stop
		right(); //take a right turn
		delay_ms(900); //delay of 0.9 second
		stop(); //stop
		count++;
	}
	while(1);
}//main ends here
