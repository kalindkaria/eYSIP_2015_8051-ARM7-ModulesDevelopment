/*
* Author: Bhumika Varshney, ERTS Lab, IIT Bombay
* File name: ADC_sensor_value_on_LCD.c
* Functions: Init_Peripherals(void), Init_Ports(void), Init_Buzzer_Pin(void), Init_ADC_Pin(void), Init_ADC0(void),
*            Init_ADC1(void), unsigned int AD0_Conversion(unsigned char channel), unsigned int AD1_Conversion(unsigned char channel)
* This experiment demonstrates 	conversion of analog values to digital values,
* ADC captures the analog sensor values and displayes it on the LCD.
* Concepts covered: ADC interfacing 
  
  Note: 
 1. Make sure that in the configuration options following settings are 
 done for proper operation of the code
  Microcontroller: LPC2148
 Frequency: 12 MHz								   
 
 2.  Coding Standards followed(For further reference on Coding Standards go to portal link).
 
 3. Make sure that you copy the lcd.c file in your folder

 4. This code displays ADC converted sensor readings. To calculate voltage from the ADC’s digital value we can use
 following formula:
     V Battery = 0.7V + (ADC value * 0.05226)
 
 5. Distance calculation for Sharp GP2D12 (10cm-80cm) IR Range sensor can be done using formula - 
     Distance in mm = (int)(10.00 * (2799.6 * (1.00 / (float)((double)(ADC_Value)^(double)(1.1546)))))


 LCD Connections:
 			  LCD	  Microcontroller Pins
 			  RS  --> P1.19
			  RW  --> P1.18
			  EN  --> P1.17
			  DB7 --> P1.25
			  DB6 --> P1.24
			  DB5 --> P1.23
			  DB4 --> P1.22

 ADC Connection:
 			  	Sensor									  ADC Channel No.

			  	Battery Voltage									AD1.4(P0.13)
			  	White line sensor 1								AD1.3(P0.12)
			  	White line sensor 2								AD0.1(P0.28)
			  	White line sensor 3								AD0.2(P0.29)
			  	IR Proximity analog sensor 2*****				AD0.6(P0.4)
			  	IR Proximity analog sensor 4*****				AD0.7(P0.5)
			  	Sharp IR range sensor 2							AD0.6(P0.4)
			  	Sharp IR range sensor 3							AD1.0(P0.6)
			  	Sharp IR range sensor 4							AD0.7(P0.5)
			  	

 ***** For using Analog IR proximity (2 and 4) sensors ensure that OE resistors are soldered and remove the respective sharp sensors. 
 	    
 
 LCD Display interpretation:
 ****************************************************************************
 *BATTERY VOLTAGE	IR PROX.SENSOR 2	FRONT SHARP 2 DIS 	IR.PROX.SENSOR 4*
 *LEFT WL SENSOR			  CENTER WL SENSOR				RIGHT WL SENSOR	*
 ****************************************************************************/

#include <lpc214x.h>
#include "LCD.h"		//This header files defines LCD related prototype functions
#include <math.h>

/*********Function Prototypes********/

void Init_Peripherals(void);
void Init_Ports(void);
void Init_Buzzer_Pin(void);
void Init_ADC_Pin(void);
void Init_ADC0(void);
void Init_ADC1(void);
unsigned int AD0_Conversion(unsigned char channel);
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
 AD0CR=0x00200E00;			// SEL = 1
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
 AD1CR=0x00200E00;			// SEL = 1
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
 Init_LCD_Pin();
 Init_ADC_Pin();
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
 Init_ADC0();
 Init_ADC1();
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
   
 while(1)	
 { 
  LCD_Print(1,1, AD0_Conversion(6),3);	//Left IR 
  LCD_Print(1,5,AD1_Conversion(0),3);	//Front Sharp
  LCD_Print(1,9,AD0_Conversion(7),3);	//Right IR	  
  LCD_Print(1,13,AD1_Conversion(4),4); 	//Battery		  
  LCD_Print(2,1,AD1_Conversion(3),3);   //whiteline Left 
  LCD_Print(2,7,AD0_Conversion(1),3);   //whiteline Center	 
  LCD_Print(2,13,AD0_Conversion(2),3);	//whiteline Right
  DelaymSec(200);
 }
 
} //main ends here
