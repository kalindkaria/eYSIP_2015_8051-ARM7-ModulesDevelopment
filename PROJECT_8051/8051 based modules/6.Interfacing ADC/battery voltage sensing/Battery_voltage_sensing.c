/*
* Author: Bhumika Varshney, ERTS Lab, IIT Bombay
* File name: Battery_voltage_sensing.c
* Functions: ADC_conversion(unsigned char ADC_channel_number), forward(), backward(), right(),
*            left(), stop(), buzzer_operation(), lcd_operation(), motor_operation()
* Global variable: start_conv, output_enable, unsigned char i, unsigned char initial_battery_voltage,
*                  unsigned char final_battery_voltage, left_velocity, right_velocity, LF, LB, RF, RB
* Description: This experiment demonstrates use of battery voltage sensors to monitor the power consumption
*              of the robot during various activities such as locomotion, buzzer and LCD operation.
* Concepts covered: Sensors and ADC interfacing 
  
 Note: 
 1. Make sure that in the configuration options following settings are 
 done for proper operation of the code
  Microcontroller: P89V51RD2
 Frequency: 11.0592 MHz	
 2. This code displays ADC converted battery status. To calculate voltage from the ADC’s digital value we can use
 following formula:
     V Battery = 0.7V + (ADC value * 0.06149)						   
 3. Coding Standards followed(For further reference on Coding Standards go to portal link).
******************************************************************************************************************/
#include <intrins.h>
#include "p89v51rx2.H"
#include "lcd_display.h"

sbit start_conv = P3^6;		 //P3^6 is connected to Start and ALE pin of ADC0808
sbit output_enable = P3^7;	//P3^7 is connected to Output Enable Pin of ADC0808
//The appropriate bits have been defined as per the logic table for motion control
sbit left_velocity=P1^3;
sbit right_velocity=P1^4;
sbit LB=P1^0;
sbit LF=P1^1;
sbit RF=P1^2;
sbit RB=P3^4;

unsigned char i=0;  //used in main function during ADC conversion 
unsigned char initial_battery_voltage=0; //variable to store battery status
unsigned char final_battery_voltage=0;	 //variable to store battery status

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
/* Function name - backward
* Input - none 
* Output - moves the robot in backward direction
* Logic - set appropriate bits high to move backward as per the logic table
*/
void backward(void) //go backward
{
RF=0;
LF=0;
RB=1;
LB=1;
}
/* Function name - left
* Input - none 
* Output - turns the robot in left direction
* Logic - set appropriate bits high to turn left as per the logic table
*/
void left(void) //go left ( hard left)
{
RF=1;
LF=0;
RB=0;
LB=1;
}
/* Function name - right
* Input - none 
* Output - turns the robot in right direction
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
/*************************************************************
* Function name - buzzer_operation
* Input - none 
* Output - buzzer beeps
* Logic - blows buzzer for specified time delays
*************************************************************/	  
void buzzer_operation()
{
	while(i<3)
  {
	buzzer=0;
	delay_ms(1000);
	buzzer=1;
	delay_ms(500);
	i++;
  }	
}
/******************************************************************
* Function name - lcd_operation
* Input - none 
* Output- diplayed words on LCD for finite time delays
* Logic - displays few strings on LCD screen by calling various lcd
*         related functions. 
********************************************************************/

void lcd_operation()
{
	lcd_clear();
	lcd_string("LCD operation");
	delay_ms(1000);
	lcd_clear();
	lcd_cursor(1,4);
	lcd_string("Firebird V");
	lcd_cursor(2,5);
	lcd_string("E-yantra");
	delay_ms(2000);   
}
/******************************************************************
* Function name - motor_operation
* Input - none 
* Output- motion of robot
* Logic - calls functions to move robot forward, backward, right, 
*         left and stop. 
********************************************************************/
void motor_operation()
{
/*here maximum velocity has been set for both the motors but to vary it one can apply PWM signal on these 2 pins*/
left_velocity=1;      
right_velocity=1;	 
forward(); //go forward
delay_ms(2000); //delay of 2 seconds
stop(); //stop
backward(); //go backward
delay_ms(1000); //delay of 1 seconds
stop(); //stop
right(); //go left
delay_ms(3000); //delay of 3 seconds
stop(); //stop
left(); //go right
delay_ms(3000); //delay of 3 seconds
stop(); //stop
}
/*
* Function Name: main
* Input: None
* Output: int to inform the caller that the program exited correctly or incorrectly (C code standard)
* Logic: stores battery status at various levels and executes the desired operation to be performed.
* Example Call: Called automatically by the Operating System
*
*/
void main()
{
 unsigned char initial_battery_voltage=0;
 unsigned char final_battery_voltage=0;
 unsigned char i=0;
 output_enable = 0;
 start_conv = 0;    //de-assert all control signals to ADC

 initial_battery_voltage= ADC_conversion(1);
 lcd_init(); //Initialize LCD
 lcd_clear();
 lcd_cursor(1,1);
 lcd_string("Initial battery");
 lcd_cursor(2,1);
 lcd_string("Voltage=");
 lcd_print(2,10,initial_battery_voltage,3);
 delay_ms(2500);
 
 	motor_operation();
 
 final_battery_voltage= ADC_conversion(1);
 lcd_clear();														
 lcd_cursor(1,1);
 lcd_string("Final battery");
 lcd_cursor(2,1);
 lcd_string("Voltage=");
 lcd_print(2,10,final_battery_voltage,3);
 while(1);
}//main ends here