/*
* Author: Bhumika Varshney, ERTS Lab, IIT Bombay
* File name: velocity_control.c
* Functions: left_motor_vel(unsigned int), right_motor_vel(unsigned int), pca_init(), forward()
* Global variable: LF, LB, RF, RB
* This experiment demonstrates simple velocity control operation using PWM.
* Concepts covered: PWM Technique 
  
 Note: 
 1. Make sure that in the configuration options following settings are 
 done for proper operation of the code
  Microcontroller: P89V51RD2
 Frequency: 11.0592 MHz								   
2.  Coding Standards followed(For further reference on Coding Standards go to portal link).
*********************************************************************************/
#include<intrins.h>
#include "p89v51rx2.h"
sbit LB=P1^0; // Left Back
sbit LF=P1^1; // Left Forward
sbit RF=P1^2; // Right Forward
sbit RB=P3^4; // Right Back
/**************************************************************************************************
Function name - left_motor_vel
* Input - compare velocity in hexadecimal
* Output - none
* Logic - changes contents of the capture register in order to change velocity
*          ,set left motor with velocity vel(compare value)
* Note - 0x00 will give full (100% duty cycle) velocity, while 0xFF will give zero (0% duty cycle)
*        velocity. Any value in between 0x00 and 0xFF will give intermediate velocity.
****************************************************************************************************/
void left_motor_vel(unsigned int vel)
{
	CCAP0H=vel;
}
/**************************************************************************************************
*Function name - right_motor_vel
* Input - compare velocity in hexadecimal
* Output - none
* Logic - changes contents of the capture register in order to change velocity
*          ,set right motor with velocity vel(compare value)
* Note - 0x00 will give full (100% duty cycle) velocity, while 0xFF will give zero (0% duty cycle)
*        velocity. Any value in between 0x00 and 0xFF will give intermediate velocity.
****************************************************************************************************/
void right_motor_vel(unsigned int vel)
{
	CCAP1H=vel;
}
/*************************************************************************************
* Function name - pca_init
* Input - none
* Output - none
* Logic - initialise programmable counter array module to generate pwm. 
*         The CCAPnH register holds the reload value. CCAPnL register increments,
*         and is loaded with this value everytime it overflows,thus the value in
*         CCAPnH can be changed to change the motor velocity
***************************************************************************************/
void pca_init(void)
{
//80 sets PCA counter to run at Fosc/6
CMOD=0x80;
//start PCA counter
CCON=0x40;
//Left motor duty cycle
CCAP0L=0x00;
CCAP0H=0xFF;
//Right motor duty cycle
CCAP1L=0x00;
CCAP1H=0xFF;
//enable PWM and ECOM bits for left motor
CCAPM0=0x42;
//enable PWM and ECOM bits for right motor
CCAPM1=0x42;
}
/* Function name - forward
* Input - none 
* Outut - moves the robot in forward direction
* Logic - set appropriate bits high to move forward as per the logic table
*/
void forward()
{
	LF=1;
	RF=1;
	LB=0;
	RB=0;
}
/*
* Function Name: main
* Input: None
* Output: int to inform the caller that the program exited correctly or
* incorrectly (C code standard)
* Logic: generates pwm signal by calling above defined functions and setting the desired velocity
* Example Call: Called automatically by the Operating System
*
*/
void main()
{
	pca_init();
/*set the motor velocities over here. Use only values
in the range 0x00 – 0xFF */
	left_motor_vel(0x1F);
	right_motor_vel(0x1F);
	forward();
	while(1);
}