/*
* Author: Bhumika Varshney, ERTS Lab, IIT Bombay
* File name: white_line_follower.c
* Functions: void Init_ADC_Pin(void), Init_ADC0(void), Init_ADC1(void), unsigned int AD0_Conversion(unsigned char channel),
*            unsigned int AD1_Conversion(unsigned char channel), forward(), right(), left(), stop(), Init_PWM(void), 
*            UpdateLeftPWM(unsigned int vel), UpdateRightPWM(unsigned int vel), Init_Motion_Pin(void),
*            Init_Peripherals(void), Init_Ports(void)
* Global variables: unsigned char flag, unsigned int left_sensor_value=0, unsigned int middle_sensor_value=0,
*                   unsigned int right_sensor_value=0
* This experiment demonstrates	white line following by monitoring the white line sensor's values.
* Concepts covered: ADC and sensors interfacing 

 Note: 
 1. Make sure that in the configuration options following settings are 
 done for proper operation of the code
  Microcontroller: LPC2148
 Frequency: 12 MHz								   
2.  Coding Standards followed(For further reference on Coding Standards go to portal link).
*********************************************************************************/
#include  <lpc214x.h>
#include"LCD.h"

/*****Function Prototypes*********************/

void Init_ADC_Pin(void);
void Init_ADC0(void);
void Init_ADC1(void);
unsigned int AD0_Conversion(unsigned char channel);
unsigned int AD1_Conversion(unsigned char channel);
void forward(void);
void left(void);
void right(void);
void stop(void);
void Init_PWM(void);
void UpdateLeftPWM(unsigned int vel);
void UpdateRightPWM(unsigned int vel);
void Init_Motion_Pin(void);
void Init_Peripherals(void);
void Init_Ports(void);

/***********Global variables**************************/
unsigned char flag=0;  //used in white line following logic
unsigned int left_sensor_value=0;
unsigned int middle_sensor_value=0;
unsigned int right_sensor_value=0;

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
	 UpdateLeftPWM(300);		//Set Velocity here. Smaller the value lesser will be the velocity.
	 UpdateRightPWM(300);		//Try different values between 0 to 500

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
	 UpdateLeftPWM(400);		//Set Velocity here. Smaller the value lesser will be the velocity.
	 UpdateRightPWM(200);		//Try different values between 0 to 500

	IO0SET|= (1<<10);
	IO1SET|=(1<<21);
	IO0CLR|=(1<<22)|(1<<11);

}
/* Function name - left
* Input - none 
* Output - turns the robot in left direction
* Logic - set appropriate bits high to turn left as per the logic table
*/
void left()
{
	 UpdateLeftPWM(200);		//Set Velocity here. Smaller the value lesser will be the velocity.
	 UpdateRightPWM(400);		//Try different values between 0 to 500

	IO0SET|= (1<<10);
	IO1SET|=(1<<21);
	IO0CLR|=(1<<22)|(1<<11);

}
/* Function name - stop
* Input - none 
* Output - stops the robot
* Logic - set appropriate bits high to stop all motors as per the logic table
*/
void stop(void)
{
	 UpdateLeftPWM(0);		//Set Velocity here. Smaller the value lesser will be the velocity.
	 UpdateRightPWM(0);		//Try different values between 0 to 500

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

/************************************************************
	Function 		: Init_ADC_Pin
	Input			: None
	Output			: None
	Description 	: Initialises the required GPIOs as ADC pins
************************************************************/

void Init_ADC_Pin(void)
{
 PINSEL0&= 0xF0FFC0FF;
 PINSEL0|= 0x0F003F00;		//Set pins P0.4, P0.5, P0.6, P0.12, P0.13 as ADC pins
 PINSEL1&= 0xF0FFFFFF;		
 PINSEL1|= 0x05000000;	    //Set pins P0.28, P0.29 as ADC pins
}

/************************************************************

	Function 		: ADC0_Init
	Return type		: None
	Parameters		: None
	Description 	: This fuction initialises ADC 0
					  module of LPC2148 microcontroller. It also 
					  configures the required I/o pins to be used as 
					  ADC pins. 
************************************************************/
void Init_ADC0(void)
{
 AD0CR=0x00200E00;	// SEL = 1 	ADC0 channel 1	Channel 1
					// CLKDIV = Fpclk / 1000000 - 1 ;1MHz
					// BURST = 0 
					// CLKS = 0 
 					// PDN = 1 
 					// START = 1
  					// EDGE = 0 (CAP/MAT)
} 
 
/************************************************************

	Function 		: ADC1_Init
	Return type		: None
	Parameters		: None
	Description 	: This fuction initialises ADC 1
					  module of LPC2148 microcontroller. It also 
					  configures the required I/o pins to be used as 
					  ADC pins. 
************************************************************/
void Init_ADC1(void)
{
 AD1CR=0x00200E00;	// SEL = 1 	ADC0 channel 1	Channel 1
					// CLKDIV = Fpclk / 1000000 - 1 ;1MHz
					// BURST = 0 
					// CLKS = 0 
 					// PDN = 1 
 					// START = 1
  					// EDGE = 0 (CAP/MAT)
} 
                           
/************************************************************
	Function 		: AD0_Conversion
	Input			: ADC channel number
	Output			: digital converted value
	Description 	: This function converts ADC0 channels.
	 Channel number is passed to this function as integer.
************************************************************/
unsigned int AD0_Conversion(unsigned char channel)
{
 unsigned int Temp;
 if(channel!=0)
 {
  AD0CR = (AD0CR & 0xFFFFFF00) | (1<<channel);
 }
 else
 {
  AD0CR = (AD0CR & 0xFFFFFF00) | 0x01;
 }
 AD0CR|=(1 << 24);
 while((AD0GDR&0x80000000)==0);
 Temp = AD0GDR;						
 Temp = (Temp>>8) & 0xFF;
 return Temp;
}

/************************************************************
	Function 		: AD1_Conversion
	Input			: ADC channel number
	Output			: digital converted value
	Description 	: This function converts ADC1 channels.
	 Channel number is passed to this function as integer.
************************************************************/

unsigned int AD1_Conversion(unsigned char channel)
{
 unsigned int Temp;
 if(channel!=0)
 {
  AD1CR = (AD1CR & 0xFFFFFF00) | (1<<channel);
 }
 else
 {
  AD1CR = (AD1CR & 0xFFFFFF00) | 0x01;
 }
 AD1CR|=(1 << 24);
 while((AD1GDR&0x80000000)==0);
 Temp = AD1GDR;						
 Temp = (Temp>>8) & 0xFF;
 return Temp;
}

/***********************************************************
* Function     : Init_Ports
* Input        : none
* Output       : none
* Logic        : initializes ports 
* Example call : called by the main function
***********************************************************/
void Init_Ports(void)
{
 Init_Motion_Pin();
 Init_LCD_Pin();
 Init_ADC_Pin();
}
/***********************************************************
* Function     : Init_Peripherals
* Input        : none
* Output       : none
* Logic        : to initialize all peripherals
* Example call : called by the main function
***********************************************************/
void Init_Peripherals(void)
{
 Init_Ports();
 Init_PWM();
 Init_ADC0();
 Init_ADC1();
}
/*********************************************************************
* Function Name	: main
* Input			: None
* Output		: int to inform the caller that the program exited 
                  correctly or incorrectly (C code standard)
* Logic			: constantly monitors the white line sensors value 
*                 and accordingly adjusts its path. 
* Example Call	: Called automatically by the Operating System
*
**********************************************************************/
int main(void)
{ 
 PINSEL0 = 0x00000000;		// Enable GPIO on all pins
 PINSEL1 = 0x00000000;
 PINSEL2 = 0x00000000;
 DelaymSec(40);
 Init_Peripherals();
 LCD_Init();
 LCD_Clear();
 DelaymSec(20);

 while(1)
 {
 	left_sensor_value=AD1_Conversion(3); 
	middle_sensor_value=AD0_Conversion(1);
	right_sensor_value=AD0_Conversion(2);
	
	LCD_Print(1,1,left_sensor_value,3);
  	LCD_Print(1,6,middle_sensor_value,3);
  	LCD_Print(1,11,right_sensor_value,3);
	LCD_Print(2,1,flag,1);
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
}
