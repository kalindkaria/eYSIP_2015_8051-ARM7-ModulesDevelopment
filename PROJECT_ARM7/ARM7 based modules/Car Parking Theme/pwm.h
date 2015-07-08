/*
* Author: Bhumika Varshney, ERTS Lab, IIT Bombay
* File name: velocity_control_pwm.c
* Functions: forward(), backward(), right(), left(), stop(), Init_PWM(void),
*            UpdateLeftPWM(unsigned int vel), UpdateRightPWM(unsigned int vel), Init_Motion_Pin(void)
* This experiment demonstrates velocity control operation by PWM technique.
* Concepts covered: PWM Technique 
  
 Note: 
 1. Make sure that in the configuration options following settings are 
 done for proper operation of the code
  Microcontroller: LPC2148
 Frequency: 12 MHz								   
2.  Coding Standards followed(For further reference on Coding Standards go to portal link).
*********************************************************************************/
#include  <lpc214x.h>

/*****Function Prototypes*********************/

void forward(void);
void backward(void);
void left(void);
void right(void);
void soft_right(void);
void soft_left(void);

void stop(void);
void Init_PWM(void);
void UpdateLeftPWM(unsigned int vel);
void UpdateRightPWM(unsigned int vel);
void Init_Motion_Pin(void);	

/************************************************************
	Function 		: Init_Motion_Pin
	Input 			: None
	Output			: None
	Description 	: Initialises Motion control pins
************************************************************/

void Init_Motion_Pin(void)
{
 PINSEL0&=0xFF0F3FFF;		
 PINSEL0|=0x00000000;		//Set Port pins P0.7, P0.10, P0.11 as GPIO
 PINSEL1&=0xFFFFF0FF;
 PINSEL1|=0x00000000;		//Set Port pins P0.21 and 0.22 as GPIO
 IO0DIR&=0xFF9FF37F;
 IO0DIR|= (1<<10) | (1<<11) | (1<<21) | (1<<22) | (1<<7); 	//Set Port pins P0.10, P0.11, P0.21, P0.22, P0.7 as Output pins
 IO1DIR&=0xFFDFFFFF;
 IO1DIR|= (1<<21);		// Set P1.21 as output pin
 stop();				// Stop both the motors on start up
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
/* Function name - backward
* Input - none 
* Output - moves the robot in backward direction
* Logic - set appropriate bits high to move backward as per the logic table
*/
void backward()
{
	IO0CLR|= (1<<10);
	IO1CLR|=(1<<21);
	IO0SET|=(1<<22)|(1<<11);
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
/* Function name - left
* Input - none 
* Output - turns the robot in left direction
* Logic - set appropriate bits high to turn left as per the logic table
*/
void left()
{
	IO0SET|= (1<<10);
	IO0CLR|=(1<<11);
	IO1CLR|=(1<<21);
	IO0SET|=(1<<22);
}

void soft_right()
{	
 stop();
 IO1SET|=(1<<21);

}


void soft_left()
{	
 stop();
 IO0SET|=(1<<10);

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

/************************************************************
   	Function 		: Init_PWM
	Input			: None
	Output			: None
	Description 	: Initialises PWM module for motion control
************************************************************/

void Init_PWM(void)
{
 PINSEL0&=0xFFFF3FFF;
 PINSEL0|=0x00008000;	//Enabling P0.7 as PWM2
 PINSEL1&=0xFFFFF3FF;		
 PINSEL1|=0x00000400;	//Enabling P0.22 as PWM5

 PWMPR	= 30;	//PWM Prescaler PCLK/30 = 500KHz
 PWMPC	= 0;	//PWMPC increments on every PCLK
 PWMTC	= 0;	//PWMTC increments on every PWMPC=PWMPR
 PWMMR0 = 500;	//PWM base frequency 500KHz/500=1KHz	 
 PWMMR2 = 0;
 PWMMR5 = 0;
 PWMMCR = 0x00000002; //Reset on PWMMR0: the PWMTC will be reset if PWMMR0 matches it
 PWMPCR	= 0x2400;	  //The PWM2 and PWM5 output enabled.
 PWMLER	= 0x24;		  //2nd and 5th match registers are enabled
 PWMTCR = 0x01;		  //the PWM Timer Counter and PWM Prescale Counter are enabled for counting.
}  
/**************************************************************************************************
* Function name - UpdateLeftPWM
* Input - desired velocity in decimal
* Output - none
* Logic - changes contents of the match register in order to change velocity
*          ,set left motor with velocity vel and enable the PWM2
* Note - 500 will give full (100% duty cycle) velocity, while 0 will give zero (0% duty cycle)
*        velocity. Any value in between 0 and 500 will give intermediate velocity.
****************************************************************************************************/

void UpdateLeftPWM(unsigned int vel)
{
 PWMMR2 = vel;
 PWMLER = 0x04;	//match register 2 enabled
}
/**************************************************************************************************
* Function name - UpdateRightPWM
* Input - desired velocity in decimal
* Output - none
* Logic - changes contents of the match register in order to change velocity
*          ,set right motor with velocity vel and enable the PWM5
* Note - 500 will give full (100% duty cycle) velocity, while 0 will give zero (0% duty cycle)
*        velocity. Any value in between 0 and 500 will give intermediate velocity.
****************************************************************************************************/

void UpdateRightPWM(unsigned int vel)
{
 PWMMR5 = vel;
 PWMLER = 0x20;	//match register 5 enabled
}

