#include<intrins.h>
#include "p89v51rx2.h"

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

//function to change velocity of both motors
void velocity(int left_vel, int right_vel)
{
	left_motor_vel(left_vel);
	right_motor_vel(right_vel);	
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
