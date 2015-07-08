/*
* Author: Bhumika Varshney, ERTS Lab, IIT Bombay
* File name: buzzer_on_switch.c
* Description: This experiment demonstrates simple Input and Output operation. 
* Functions: Init_Buzzer_Pin(void), Init_Peripherals(void), Init_Ports(void), Init_Switch_Pin(void)
* Concepts covered: Input and Output operations
  
 Note: 
 
 1. Make sure that in the configuration options following settings are 
 done for proper operation of the code
  Microcontroller: LPC2148
 Frequency: 12 MHz								   
 2. Buzzer is connected to the 25th pin of Port0 i.e P0.25, to turn it 
    on we have to set it as output and give logic 1 to it.
 3. Switch is connected to P0.14.
 4. Coding Standards followed(For further reference on Coding Standards go to portal link).
*********************************************************************************/
#include  <lpc214x.h>

/**********Macros******************/

#define BUZZER_OFF() IO0CLR=(1<<25)		   //Macro to turn OFF buzzer
#define BUZZER_ON() IO0SET=(1<<25)		   //Macro to turn ON buzzer
#define SW1 (IO0PIN & 0x00004000)		   //Macro to read switch status

/**********************************/

/****Function Prototypes**********/
                               
void Init_Buzzer_Pin(void);
void Init_Switch_Pin(void);
void Init_Peripherals(void);
void Init_Ports(void);

/*********************************/


/************************************************************

	Function 		: Init_Buzzer_Pin
	Return type		: None
	Parameters		: None
	Description 	: Initialises Buzzer pin
************************************************************/
void Init_Buzzer_Pin(void)
{
 PINSEL1&=0xFFF3FFFF;	//Set P0.25 as GPIO
 IO0DIR&=0xFDFFFFFF; 
 IO0DIR|= (1<<25);  	//Set P0.25 as Output
 BUZZER_OFF();			//Initially turn OFF buzzer
}

/************************************************************

	Function 		: Init_Switch_Pin
	Return type		: None
	Parameters		: None
	Description 	: Initialises Switch pin
************************************************************/
void Init_Switch_Pin(void)
{ 
 PINSEL0&=0xCFFFFFFF;	//Set P0.14 as GPIO, P0.14 is Input only pin so no need to set direction
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
 Init_Buzzer_Pin();
 Init_Switch_Pin();
}
/***********************************************************
* Function     : Init_Peripherals
* Input        : none
* Output       : none
* Logic        : to initialize peripherals, in this case it 
*                is buzzer so it will call the function to 
*                initialize buzzer port 
* Example call : called by the main function
***********************************************************/
void Init_Peripherals(void)
{
 Init_Ports();
}
/******************************************************************
* Function Name	: main
* Input        	: None
* Output		: int to inform the caller that the program exited 
*         		  correctly or incorrectly (C code standard)
* Logic			: blows the buzzer when boot switch is pressed 
* Example Call	: Called automatically by the Operating System
********************************************************************/
int  main(void)
{  
 PINSEL0 = 0x00000000;		// Reset all pins as GPIO
 PINSEL1 = 0x00000000;
 PINSEL2 = 0x00000000;
 Init_Peripherals();
 while(1)	
 { 
  if(!SW1) {BUZZER_ON();}	 //If switch is pressed i.e. P0.14 is at logic '0' then turn on Buzzer
  else 	   {BUZZER_OFF();}	 //else turn OFF buzzer
 }
}//main ends here

