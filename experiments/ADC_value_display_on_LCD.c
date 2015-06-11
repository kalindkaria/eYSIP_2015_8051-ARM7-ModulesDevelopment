/*
* Author: Bhumika Varshney, ERTS Lab, IIT Bombay
* File name: ADC_value_display.c
* Functions: ADC_conversion(unsigned char ADC_channel_number)
* Global variable: start_conv, output_enable, unsigned char i
* This experiment demonstrates 	conversion of analog values to digital values.
* Concepts covered: ADC interfacing 
  
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

sbit start_conv = P3^6;		 //P3^6 is connected to Start and ALE pin of ADC0808
sbit output_enable = P3^7;	 //P3^7 is connected to Output Enable Pin of ADC0808

unsigned char i=0;  //used in main function during ADC conversion 

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

/*************************************************************************
* Function Name: main
* Input: None
* Output: int to inform the caller that the program exited correctly or
* incorrectly (C code standard)
* Logic: displays various sensor's values on LCD after converting it 
*        to its digital equivalent.  
* Example Call: Called automatically by the Operating System
*
**************************************************************************/
void main()
{
 unsigned char i=0;
 output_enable = 0;
 start_conv = 0;    //de-assert all control signals to ADC
 
 lcd_init(); //Initialize LCD
 lcd_clear();
  lcd_print(1,1,ADC_conversion(1),3);  //battery voltage value
  lcd_print(1,5,ADC_conversion(3),3);  //IR proximity sensor 2
  lcd_print(1,9,ADC_conversion(0),3);  //IR proximity sensor 3
  lcd_print(1,13,ADC_conversion(7),3); //IR proximity sensor 4
  lcd_print(2,1,ADC_conversion(2),3);  //sharp IR sensor value
  lcd_print(2,5,ADC_conversion(4),3);  //left white line sensor
  lcd_print(2,9,ADC_conversion(5),3);  //center white line sensor
  lcd_print(2,13,ADC_conversion(6),3); //right white line sensor
  while(1);	//to avoid effect of fluctuation in values being measured
}	//main ends here