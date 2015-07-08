/*
* Author: Bhumika Varshney, ERTS Lab, IIT Bombay
* File name: Obstacle_detection_using_sharpIR_sensor.c
* Functions: Init_Peripherals(void), Init_Ports(void), Init_Buzzer_Pin(void), Init_ADC_Pin(void), Init_ADC1(void),
*            unsigned int AD1_Conversion(unsigned char channel), Init_motion_pin(), forward(), right(), stop()
* Global variables: unsigned int sharpIR_value
* This experiment demonstrates use of sharpIR sensor in detecting an obstacle.
* Concepts covered: Sensors and ADC interfacing 
  
  Note: 
 1. Make sure that in the configuration options following settings are 
 done for proper operation of the code
  Microcontroller: LPC2148
 Frequency: 12 MHz								   
 
 2.  Coding Standards followed(For further reference on Coding Standards go to portal link).
 
 3. Make sure that you copy the lcd.c file in your folder
 
 4. Distance calculation for Sharp GP2D12 (10cm-80cm) IR Range sensor can be done using formula - 
     Distance in mm = (int)(10.00 * (2799.6 * (1.00 / (float)((double)(ADC_Value)^(double)(1.1546)))))

 5. ADC Connection: Sharp IR range sensor 3 is connected to ADC Channel No. AD1.0(P0.6)

 ********************************************************************************************************/

#include <lpc214x.h>
#include "LCD.h"		//This header files defines LCD related prototype functions
#include <math.h>

/**********Macros******************/
#define BUZZER_OFF() IO0CLR=(1<<25) //Macro to turn OFF buzzer
#define BUZZER_ON() IO0SET=(1<<25) //Macro to turn ON buzzer

/********Global Variables*******/
unsigned int sharpIR_value=0;		   //variable to store sharp IR sensor value

/*********Function Prototypes********/

void Init_Peripherals(void);
void Init_Ports(void);
void Init_Buzzer_Pin(void);
void Init_motion_pin(void);
void Init_ADC_Pin(void);
void forward(void);
void right(void);
void stop(void);
void Init_ADC1(void);
unsigned int AD1_Conversion(unsigned char channel);

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
 AD1CR=0x00200E00;	// SEL = 1 	ADC0 Channel 1
					// CLKDIV = Fpclk / 1000000 - 1 ;1MHz
					// BURST = 0 
					// CLKS = 0 
 					// PDN = 1 
 					// START = 1
  					// EDGE = 0 (CAP/MAT)
} 
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

/************************************************************
Function : Init_Buzzer_Pin
Return type : None
Parameters : None
Description : Initialises Buzzer pin
************************************************************/
void Init_Buzzer_Pin(void)
{
PINSEL1&=0xFFF3FFFF;
PINSEL1|=0x00000000; //Set P0.25 as GPIO
IO0DIR&=0xFDFFFFFF;
IO0DIR|= (1<<25); //Set P0.25 as Output
BUZZER_OFF(); //Initially turn OFF buzzer
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
 Init_LCD_Pin();
 Init_ADC_Pin();
 Init_motion_pin();
 Init_Buzzer_Pin();
}
/***********************************************************
* Function     : Init_Peripherals
* Input        : none
* Output       : none
* Logic        : to initialize peripherals 
* Example call : called by the main function
***********************************************************/
void Init_Peripherals(void)
{
 Init_Ports();
 Init_ADC1();
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

/*********************************************************************
* Function Name	: main
* Input			: None
* Output		: int to inform the caller that the program exited 
                  correctly or incorrectly (C code standard)
* Logic			: displays all sensors value on LCD after converting it to digital 
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
 BUZZER_OFF();
   
 while(1)	
 {
 	forward();
	sharpIR_value=AD1_Conversion(0);
	LCD_Print(1,1,sharpIR_value,3);
	if(sharpIR_value>50) //obstacle detected
	{
	stop();
	DelaymSec(500);
	BUZZER_ON();	//buzzer on
	DelaymSec(300);	 
	BUZZER_OFF();	//buzzer off
	right();		//right turn
	DelaymSec(400);
	}
 }			
 
} //main ends here
