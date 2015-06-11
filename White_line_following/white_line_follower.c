/*
* Author: Bhumika Varshney, ERTS Lab, IIT Bombay
* File name: white_line_follower.c
* Functions: ADC_conversion(unsigned char ADC_channel_number), left_motor_velocity(unsigned char vel),
*            right_motor_velocity(unsigned char vel), forward(), right(), left(), stop(), pca_init()
* Global variable: start_conv, output_enable, unsigned char i, left_velocity, right_velocity, LF, LB, RF, RB
*                  unsigned char flag
* This experiment demonstrates 	white line following by monitoring the white line sensor's values.
* Concepts covered: ADC and sensors interfacing 
  
 Note: 
 1. Make sure that in the configuration options following settings are 
 done for proper operation of the code
  Microcontroller: P89V51RD2
 Frequency: 11.0592 MHz
 2. This code displays ADC converted sensor readings. To calculate voltage from the ADC’s digital value we can use
 following formula:
     V Battery = 0.7V + (ADC value * 0.06149)								   
3.  Coding Standards followed(For further reference on Coding Standards go to portal link).
*******************************************************************************************************************/
#include <intrins.h>
#include "p89v51rx2.H"
#include "lcd_display.h"

//The appropriate bits have been defined as per the logic table for motion control
sbit left_velocity=P1^3;
sbit right_velocity=P1^4;
sbit LB=P1^0;
sbit LF=P1^1;
sbit RF=P1^2;
sbit RB=P3^4;

sbit start_conv = P3^6;		 //P3^6 is connected to Start and ALE pin of ADC0808
sbit output_enable = P3^7;		 //P3^7 is connected to Output Enable Pin of ADC0808

unsigned char i=0;  //used in main function during ADC conversion
unsigned char flag=0;  //used in white line following logic
 

/********************************************************************************
	Function: ADC_conversion
	Input: ADC_channel_number
	Output: Digital equivalent of analog data
	Description: This function performs ADC conversion of a channel number 
				 that is passed to it. The return value of this function contains 
 				 digital data of the selected channel.
		   
				 Analog Sensor Pin Mapping
  
            	 IN0-----> IR Proximity sensor 3 (front)
				 IN1-----> Batt. Voltage
				 IN2-----> Front Sharp IR Range sensor
				 IN3-----> IR Proximity sensor 2 (left side)
				 IN4-----> Left Whiteline
		 		 IN5-----> Center Whiteline
				 IN6-----> Right Whiteline
				 IN7-----> IR Proximity sensor 4 (right side)
********************************************************************************/
unsigned char ADC_conversion(unsigned char ADC_ch_no)
{
unsigned char ADC_data;

P0 = ADC_ch_no;		  // to select channel no. send address on P0

start_conv = 1; // ADC0808 will latch the address on L-H transition on the ALE pin
start_conv = 0;	// ADC0808 will start conversion on H-L transition on the start pin
delay_ms(2);	// conversion time for ADC0808

P0 =0xFF;	        // output enable and read
output_enable = 1;	//Enabling o/p of ADC0808 for to read the converted data
ADC_data = P0;		//storing adc_data
output_enable = 0;	//disabling ADC 0808 o/p
return ADC_data;	//returning adc_data
}

/**************************************************************************************************
Function name - left_motor_vel
* Input - compare velocity in hexadecimal
* Output - none
* Logic - changes contents of the capture register in order to change velocity
*          ,set left motor with velocity vel(compare value)
* Note - 0x00 will give full (100% duty cycle) velocity, while 0xFF will give zero (0% duty cycle)
*        velocity. Any value in between 0x00 and 0xFF will give intermediate velocity.
****************************************************************************************************/
void left_motor_velocity(unsigned char vel)
{
CCAP0H=vel;
}

/**************************************************************************************************
Function name - right_motor_vel
* Input - compare velocity in hexadecimal
* Output - none
* Logic - changes contents of the capture register in order to change velocity
*          ,set right motor with velocity vel(compare value)
* Note - 0x00 will give full (100% duty cycle) velocity, while 0xFF will give zero (0% duty cycle)
*        velocity. Any value in between 0x00 and 0xFF will give intermediate velocity.
****************************************************************************************************/
void right_motor_velocity(unsigned char vel)
{
CCAP1H=vel;
}

/* Function name - forward
* Input - none 
* Output - moves the robot in forward direction
* Logic - set appropriate bits high to move backward as per the logic table
*/
void forward(void) //go forward
{
RF=1;
LF=1;
RB=0;
LB=0;
}
/* Function name - left
* Input - none 
* Outut - turns the robot in left direction
* Logic - set appropriate bits high to turn left as per the logic table
*/
void left(void) //go left ( hard left)
{
RF=1;
LF=1;
RB=0;
LB=0;
 left_motor_velocity(0x5F);	//decrease  left motor velocity
 right_motor_velocity(0x2F);	//increase right motor velocity

}
/* Function name - right
* Input - none 
* Outut - turns the robot in right direction
* Logic - set appropriate bits high to turn right as per the logic table
*/
void right(void) //go right ( hard right)
{
RF=1;
LF=1;
RB=0;
LB=0;
 left_motor_velocity(0x3F);	//increase left motor velocity 
 right_motor_velocity(0x5F);  //decrease  right motor velocity
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
 CMOD=0x80;	   //80 sets PCA counter to run at Fosc/6; 
 CCON=0x40;	   //start PCA counter
 						
 CCAP0L=0x00;   
 CCAP0H=0xFF;  //Left motor duty cycle register
  
 CCAP1L=0x00;				  
 CCAP1H=0xFF;  //Right motor duty cycle register

 CCAPM0=0x42;  //enable PWM mode and ECOM bits for left motor
 CCAPM1=0x42;  //enable PWM mode and ECOM bits for right motor
}

/*
* Function Name: main
* Input: None
* Output: int to inform the caller that the program exited correctly or
* incorrectly (C code standard)
* Logic: constantly monitors the white line sensors value and accordingly adjusts its path.
* Example Call: Called automatically by the Operating System
*
*/
void main()
{
	unsigned int left_sensor_value=0;
	unsigned int middle_sensor_value=0;
	unsigned int right_sensor_value=0;
	
	unsigned char i=0;
 	output_enable = 0;
 	start_conv = 0;    //de-assert all control signals to ADC
 
  	lcd_init(); //Initialize LCD
 	pca_init();
 	lcd_clear();
  	
	forward(); //moves forward
	left_motor_velocity(0x4F);  /*0x00 will give full (100% duty cycle) velocity, 
while 0xFF will give zero (0% duty cycle) velocity. Any value in between 0x00 and 0xFF will give intermediate velocity.*/			
	right_motor_velocity(0x4F); /*0x00 will give full (100% duty cycle) velocity, 
while 0xFF will give zero (0% duty cycle) velocity. Any value in between 0x00 and 0xFF will give intermediate velocity.*/
 
 while(1)
 {
 	left_sensor_value=ADC_conversion(4); 
	middle_sensor_value=ADC_conversion(5);
	right_sensor_value=ADC_conversion(6);
	
	lcd_print(1,1,left_sensor_value,3);
  	lcd_print(1,6,middle_sensor_value,3);
  	lcd_print(1,11,right_sensor_value,3);
	lcd_print(2,1,flag,1);
	flag=0;		 //reset the flag to 0

	if(left_sensor_value>50 && middle_sensor_value<20 && right_sensor_value>50 && flag==0) //checks if middle sensor is on white line and rest two on black
	{
		flag=1;	//set the flag to 1 so that further white line sensor comparision is disabled
		forward();  //left and right motor is at same velocity
	}
	if(left_sensor_value<50 && right_sensor_value>50 && flag==0) //if it deviates towards right
	{
		flag=1;	//set the flag to 1 so that further white line sensor comparision is disabled
		left();	//soft left turn by varying velocity
		while(left_sensor_value>50);
	}
	if(left_sensor_value>50 && right_sensor_value<50 && flag==0) //if it deviates towards left
	{
		flag=1;	//set the flag to 1 so that further white line sensor comparision is disabled
		right(); //soft right turn by varying velocity
		while(right_sensor_value>50);
	}
	if(left_sensor_value>50 && middle_sensor_value>50 && right_sensor_value>50 && flag==0) //if all sensors are on black
		stop();	 //stops
	}
}//main ends here