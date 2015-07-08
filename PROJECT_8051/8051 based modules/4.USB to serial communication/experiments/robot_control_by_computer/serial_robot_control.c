/*
* Author: Bhumika Varshney, ERTS Lab, IIT Bombay
* File name: serial_robot_control.c
* Functions: uart_setup(), RxData(), forward(), backward(), right(), left(), stop()
* Global variable: unsigned char rx_data, left_velocity, right_velocity, LB, LF, RF, RB 
* This experiment demonstrates wired serial communication between computer and the robot.
* Concepts covered: USB to serial communication 
  
 Note: 
 1. Make sure that in the configuration options following settings are 
 done for proper operation of the code
  Microcontroller: P89V51RD2
 Frequency: 11.0592 MHz
 2. Make sure that jumpers are properly connected to select between RS232 and Wireless communication mode.								   
2.  Coding Standards followed(For further reference on Coding Standards go to portal link).
*********************************************************************************/
#include <intrins.h>
#include "p89v51rx2.H"
#include"lcd_display.h"

unsigned char rx_data;
sbit left_velocity=P1^3;
sbit right_velocity=P1^4;
sbit LB=P1^0;
sbit LF=P1^1;
sbit RF=P1^2;
sbit RB=P3^4;

/**************************************************************************
	Function: uart_setup()
	Input: none
	Output: desired baud rate 
	Description: This function configures Timer1 for generating the Baudrate.
			     The TH1 register should be written with following hex numbers
		         for desired baud rate. After setting the baudrate, SCON register
			     for enabling Rx and setting up the frame format.
 	
	       		 9600 ----->  0xFD
			     4800 ----->  0xFA
			     2400 ----->  0xF4
			     1200 ----->  0xE8

**************************************************************************/
void uart_setup()
{
	IEN0=0x90;			//enable serial interrupt
    TMOD = 0x20;		// configure timer1 for Mode 2 operation for the correct baud rate 
    TH1 = 0xFD;     	// 9600 bps for 11.0592 MHz clock 
    TCON = 0x40;    	// Start timer 1 by setting TR1 = 1 
    SCON = 0x50;  		// Set Serial IO to receive and normal mode 
	RI=0;               //Receive Interrupt Flag is cleared
	TI=0;  				//Tx interrupt flag is cleared
}		                 

/***************************************************************************
	Function: void char RxData()
	Input: none
	output: recieved data byte
	Description: This function copies any data that is present in the SBUF 
			     register and returns it to the calling function.
***************************************************************************/

 unsigned char RxData()
{
    unsigned char rcv_data;
	rcv_data = (unsigned char)SBUF;  //if any data is available copy it from SBUF
	return rcv_data;      //return the received data
}
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
/* Function name - backward
* Input - none 
* Outut - moves the robot in backward direction
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
* Outut - turns the robot in left direction
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
/***************************************************************************
* Function: Interrupt Service Routine
* Input: none
* Output: none
* Logic: whenever serial interrupt occurs it executes this routine
*        and after this it returns to the main function
*        serial interrupt occurs whenever data is recieved or transmitted.
****************************************************************************/
void serial_int_isr(void)interrupt 4
{
	rx_data = RxData();  //get the received data
  	datasend(rx_data);
	RI = 0;  //Clear receive interrupt. Must be cleared by the user.
  	TI = 0;	//Clear transmit interrupt. Must be cleared by the user.
	if (rx_data== 0x38)	
	buzzer=0; 		//buzzer on
	if (rx_data == 0x39)
	buzzer = 1;		//buzzer off
	if(rx_data==0x30)
	forward();	   //move forward
	if(rx_data==0x31)
	backward();		//move backward
	if(rx_data==0x32)
	right();	  //turn right
	if(rx_data==0x33)
	left();		  //turn left
	if(rx_data==0x34)
	stop();		//stop	
}
/*
* Function Name: main
* Input: None
* Output: int to inform the caller that the program exited correctly or incorrectly (C code standard)
* Logic: initializes UART communication
* Example Call: Called automatically by the Operating System
*
*/
void main(void)
{
 	
	buzzer=1;
 	uart_setup(); // calling the UART setup function
 	lcd_init();	//initializes LCD
	lcd_clear();
 	while(1); 
} //main ends here