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
 
//sbit buzzer=P2^7;
/**************************************************************************
	Function: uart_setup()
	
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
    TMOD = 0x20;		// configure timer1 for Mode 2 operation for the correct baud rate 
    TH1 = 0xFD;     	// 9600 bps for 11.0592 MHz clock 
    TCON = 0x40;    	// Start timer 1 by setting TR1 = 1 
    SCON = 0x50;  		// Set Serial IO to receive and normal mode 
	RI=0;               //Receive Interrupt Flag is cleared
	TI=0;  				//Tx interrupt flag is cleared
}		                 

/***************************************************************************
	Function: void char RxData()
	
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
void process()
{
if (rx_data== 0x38)
buzzer=0; 
if (rx_data == 0x39)
buzzer = 1;
if(rx_data==0x30)
forward();
if(rx_data==0x31)
backward();
if(rx_data==0x32)
right();
if(rx_data==0x33)
left();
if(rx_data==0x34)
stop();			
}
void main(void)
{
 	
	buzzer=1;
 	uart_setup(); // calling the UART setup function
 	lcd_init();
	lcd_clear();
 	while(1) //loop continuously
 	{
  		if (RI==1)   // if any data is present store it and transmit it to LCD 
  		{
   		rx_data = RxData();  //get the received data
   		datasend(rx_data);
		process();
		RI = 0;  //Clear receive interrupt. Must be cleared by the user.
   		TI = 0;	//Clear transmit interrupt. Must be cleared by the user.
  		}
 	}
}

