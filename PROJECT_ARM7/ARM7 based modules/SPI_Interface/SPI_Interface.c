#include <LPC214x.h>
#include"spi.h"
#include"LCD.h"

#define	SPI1_SLAVE_SELECT	0x00100000

#define SENSOR_OFF() IO1SET=(1<<16)		//Macro to turn OFF Sensors
#define SENSOR_ON() IO1CLR=(1<<16)		//Macro to turn ON Sensors

/************************************************************

	Function 		: Init_Sensor_Switch_Pin
	Return type		: None
	Parameters		: None
	Description 	: Initialises Sensor switching pin
************************************************************/
                               
void Init_Sensor_Switch_Pin(void)
{
 IO1DIR&=0xFFFEFFFF;
 IO1DIR|=(1<<16);   //Set P1.16 direction as Output
 SENSOR_OFF();		   //Initially turn OFF Sensor
}

BYTE MEGA8_ADCRead(BYTE channel)
{
	BYTE	adcVal = 0;
	DWORD i = 0;

	IOCLR0 |= SPI1_SLAVE_SELECT;		// slave select enable
	SPI1_SendByte(channel);

	//Delay for settling down  (80 uS)
	for (i=0; i<1000; i++);

	adcVal = SPI1_ReceiveByte();
	IOSET0 |= SPI1_SLAVE_SELECT;		// slave select disable

	//Delay for settling down  (80 uS)
	for (i=0; i<1000; i++);

	return adcVal;

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

void Init_Ports(void)
{
 Init_ADC_Pin();
 Init_LCD_Pin();
}


void Init_Peripherals(void)
{
 Init_Ports();
 Init_Sensor_Switch_Pin();
 Init_ADC0();
 Init_ADC1();
 SPI1_Init();
}

int main()
{
 PINSEL0 = 0x00000000;
 PINSEL1 = 0x00000000;
 PINSEL2 = 0x00000000;
 Init_Peripherals();
 DelaymSec(40);
 LCD_4Bit_Mode();
 LCD_Init();
 LCD_Clear();
 DelaymSec(20);
 while(1)
 {
 LCD_Print(1,1,MEGA8_ADCRead(7),3);
 LCD_Print(1,5,AD1_Conversion(0),3);	//Front Sharp
 LCD_Print(1,7,MEGA8_ADCRead(13),3);
 LCD_Print(1,14,MEGA8_ADCRead(9),3);
 LCD_Print(2,4,MEGA8_ADCRead(8),3);
 LCD_Print(2,10,MEGA8_ADCRead(15),3);
 DelaymSec(200);
 }
}
