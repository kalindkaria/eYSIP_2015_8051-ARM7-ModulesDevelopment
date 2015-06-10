/*
* Author: Bhumika Varshney, ERTS Lab, IIT Bombay
* File name: trace_square_clockwise.c
* Functions: delay_ms(unsigned int)
* Global variable: left_velocity, right_velocity, LB, LF, RB, RF
* This experiment demonstrates simple Motion control of robot.
* Concepts covered: Motor related operations
  
 Note: 
 1. Make sure that in the configuration options following settings are 
 done for proper operation of the code
  Microcontroller: P89V51RD2
 Frequency: 11.0592 MHz								   
2.  Coding Standards followed(For further reference on Coding Standards go to portal link).
*********************************************************************************/
#include <intrins.h>
#include "p89v51rx2.h"
//The appropriate bits have been defined as per the logic table for motion control
sbit left_velocity=P1^3;
sbit right_velocity=P1^4;
sbit LB=P1^0;
sbit LF=P1^1;
sbit RF=P1^2;
sbit RB=P3^4;
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
//These are the direction functions to move the motors in various directions
/* Function name - forward
* Input - none 
* Outut - moves the robot in forward direction
* Logic - set appropriate bits high to move forward as per the logic table
*/
void forward(void) //go forward
{
RF=1;
LF=1;
RB=0;
LB=0;
}
/* Function name - right
* Input - none 
* Outut - turns the robot in right direction
* Logic - set appropriate bits high to turn right as per the logic table
*/
void right(void) //go right ( hard right)
{
RF=0;
LF=1;
RB=1;
LB=0;
}
/* Function name - stop
* Input - none 
* Outut - stops the robot
* Logic - set appropriate bits high to stop all motors as per the logic table
*/
void stop(void) //stop ( soft stop )
{
RF= 0;
LF= 0;
RB= 0;
LB= 0;
}
/*
* Function Name: main
* Input: None
* Output: int to inform the caller that the program exited correctly or incorrectly (C code standard)
* Logic: calls the function forward and right to trace a square.
* Example Call: Called automatically by the Operating System
*
*/
void main (void)
{
int count=1;
/*here maximum velocity has been set for both the
* motors but to vary it one can apply PWM signal on these 2 pins*/
left_velocity=1;      
right_velocity=1;
while(count<5)
{	 
forward(); //go forward
delay_ms(2000); //delay of 2 seconds
stop(); //stop
right(); //take a right turn
delay_ms(1500); //delay of 1.5 seconds
stop(); //stop
count++;
}
while(1);
}// main ends here