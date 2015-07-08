/*
* Author: Bhumika Varshney, ERTS Lab, IIT Bombay
* File name: obstacle_detection_using_proximity_sensor.c
* Functions: ADC_conversion(unsigned char ADC_channel_number), forward(), stop(), 
*            left_mtor_velocity(unsigned char vel), right_mtor_velocity(unsigned char vel), pca_init()
* Global variable: start_conv, output_enable, unsigned char i, left_velocity, right_velocity, LF, LB, RF, RB
* Description: This experiment demonstrates use of proximity sensor in detecting an obstacle.
* Concepts covered: Sensors and ADC interfacing 
  
 Note: 
 1. Make sure that in the configuration options following settings are 
 done for proper operation of the code
  Microcontroller: P89V51RD2
 Frequency: 11.0592 MHz							   
 2. Coding Standards followed(For further reference on Coding Standards go to portal link).
******************************************************************************************************************/
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
sbit output_enable = P3^7;	//P3^7 is connected to Output Enable Pin of ADC0808

unsigned char i=0;         //used in main function during ADC conversion 

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

	P0 = ADC_ch_no;		// to select channel no. send address on P0
							
	start_conv = 1; 	// ADC0808 will latch the address on L-H transition on the ALE pin
	start_conv = 0;		// ADC0808 will start conversion on H-L transition on the start pin
	delay_ms(2);		// conversion time for ADC0808

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
/* Function name - stop
* Input - none 
* Output - stops the robot
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
* Logic: constantly monitors the sensor value and blows buzzer above a defined threshold value. 
* Example Call: Called automatically by the Operating System
*
*/
void main()
{
	unsigned int sensor2_value=0; //variable to store proximity sensor2 value
 	unsigned int sensor3_value=0; //variable to store proximity sensor3 value
 	unsigned int sensor4_value=0; //variable to store proximity sensor4 value
 	unsigned char i=0;
 	output_enable = 0;
 	start_conv = 0;    //de-assert all control signals to ADC
 
 	buzzer = 1; 	//buzzer off
 	lcd_init(); 	//Initialize LCD
 	pca_init();		//initialize PCA for velocity control
 	lcd_clear();  	//clears LCD
 	forward();
 	left_motor_velocity(0x2F);			
	right_motor_velocity(0x2F);
 while(1)
 {
	sensor2_value=ADC_conversion(3);
	sensor3_value=ADC_conversion(0);
	sensor4_value=ADC_conversion(7);
	
	lcd_print(1,1,sensor2_value,3);
	lcd_print(1,5,sensor3_value,3);
	lcd_print(1,9,sensor4_value,3);
	
	if(sensor2_value<50 || sensor3_value<50 || sensor4_value<50) //obstacle detected
	{
	stop();
	delay_ms(1000);
	buzzer=0;	//buzzer on
	delay_ms(2000);	 
	buzzer=1;	//buzzer off	
	break; 
	}			
 }
} //main ends here