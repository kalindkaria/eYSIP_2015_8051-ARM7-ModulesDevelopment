/*
* Author: Bhumika Varshney, ERTS Lab, IIT Bombay
* File name: 90_degree_right_turn_using_position_encoder.c
* Functions: int1_setup(), right(), stop()
* Global variable: left_velocity, right_velocity, LF, LB, RF, RB, left_shaft_count, reqd_shaft_count, distance
* This experiment demonstrates 90 degree right turn experiment using position encoders.
* Concepts covered: Position Encoders 
  
 Note: 
 1. Make sure that in the configuration options following settings are 
 done for proper operation of the code
  Microcontroller: P89V51RD2
 Frequency: 11.0592 MHz								   
2.  Coding Standards followed(For further reference on Coding Standards go to portal link).
******************************************************************************************************************/
#include <intrins.h>
#include "p89v51rx2.H"
#include"lcd_display.h"
//program specific declarations
sbit LB=P1^0;	    //LEFT BACK Pin.
sbit LF=P1^1;		//LEFT FRONT Pin.
sbit RF=P1^2;		//RIGHT FRONT Pin.
sbit RB=P3^4;		//RIGHT BACK Pin.

sbit left_velocity=P1^3;  //Left velocity control pin. 
sbit right_velocity=P1^4; //Right velocity control pin. 

unsigned int left_shaft_count=0;//initialize a variable to store the count pulses of the left position encoder.
 unsigned int reqd_shaft_count_int=0;

/****************************************************************************
* Function: int1_setup
* Input: none
* Output: none
* Logic: initialization routine for Interrupt 1 setup, initalisation of the
*        Int 1 should be done in the same sequence as given in this function.
*******************************************************************************/
void int1_setup() 
{
TCON = 0x04; //Set Interrupt 1 trigger type to falling edge trigger.
IEN0 = 0x84; //External Interrupt 1 Enable and Global Interrupt Enable. 

P3 = 0x08; //set P3.3 (INT 1) as input port.
		   // Can also be written as INT1 = 0; (refer to p89v51rx2.H).
}
/************************************************************************
* Function: Interrupt Service Routine
* Input: none
* Output: none
* Logic: This is an Interrupt Service Routine for external interrut1, 
*        whenever interrupt1 occurs it executes this routine and 
*        after this it returns to the main function
***************************************************************************/
void int1_isr(void)interrupt 2     //ISR Routine for External Interrupt 1.
								   //Refer to chapter 4 in the software manual (table 4.1)for syntax explanation.
{
 left_shaft_count++;//when the slotted disc passes through the encoder pulses will be generated,each pulse will generate an interrupt and the right shaft count value will be incremented.
 IE1 = 0; // When interrupt is processed it is cleared automatically by the hardware  or it can also be cleared by the software.
 		  // Clearing IE1 flag to 0.	
}
/* Function name - right
* Input - none 
* Outut - turns the robot right
* Logic - set appropriate bits high to turn right as per the logic table
*/ 
void right(void)
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

void stop(void)
{
RF=0;
LF=0;
RB=0;
LB=0;
}
/*
* Function Name: main
* Input: None
* Output: int to inform the caller that the program exited correctly or incorrectly (C code standard)
* Logic: initializes and enable the interrupts and calls functions to turn the robot for a desired
*        angle accordingly 
* Example Call: Called automatically by the Operating System
*
*/
void main()
{ 
	lcd_init();
	lcd_clear();
  int1_setup();//external linterrupt 1 initialization.    
 
 reqd_shaft_count_int=(unsigned int)(90/4.090); //This equation will calculate the count required for distance to be travelled.
 lcd_print(1,1,reqd_shaft_count_int,2);
 
  					
 left_velocity = 1;	 //setting motor enable pins to 1
 right_velocity = 1; //setting motor enable pins to 1
 right();	  //go forward                            
 while(left_shaft_count<reqd_shaft_count_int); //wait till desired distance is covered.
 lcd_print(1,5,left_shaft_count,2);
 stop();	 //stop motor	  		
 while(1);
}//main ends                

