/*
* Author: Bhumika Varshney, ERTS Lab, IIT Bombay
* File name: buzzer_beep_using_timer_interrupt.c
* Description: This experiment demonstrates use of timer match interrupt to do tasks in a peirodic way.
*              In this example timer 0's Match0 interrupt is used to turn on and off 
*              buzzer with the time peirod of 1 second.
* Functions:  __irq IRQ_Timer0(void), Init_Buzzer_Pin(void), Init_Peripherals(void),
*             Init_Ports(void), Timer0_Init(void)
* Global Variables: Temp, Toggle
* Concepts covered: Interrupts 
  
 Note: 
 1. Make sure that in the configuration options following settings are done for proper operation of the code
  Microcontroller: LPC2148
 Frequency: 12 MHz
		   
2.  Coding Standards followed(For further reference on Coding Standards go to portal link).
*******************************************************************************************************************/
#include <LPC214x.h>

/**********Macros******************/

#define BUZZER_OFF() IO0CLR=(1<<25)		   //Macro to turn OFF buzzer
#define BUZZER_ON() IO0SET=(1<<25)		   //Macro to turn ON buzzer


/****Function Prototypes**********/

void __irq IRQ_Timer0(void);
void Init_Buzzer_Pin(void);
void Init_Peripherals(void);
void Init_Ports(void);
void Timer0_Init(void);

/********************************/

/*********Global Variables*******/

unsigned char Temp=0;
unsigned char Toggle=0;

/************************************************************
  	Function 		: Init_Buzzer_Pin
	Input			: None
	Output			: None
	Description 	: Initialises Buzzer pin
************************************************************/

void Init_Buzzer_Pin(void)
{
 PINSEL1&=0xFFF3FFFF;		
 PINSEL1|=0x00000000; 		//Set P0.25 as GPIO
 IO0DIR = (1<<25);  		//Set P0.25 as Output
 BUZZER_OFF();				//Initially turn OFF buzzer
}
/****************************************** 
* Function		: Timer0_Init
* Input			: none
* Output		: none
* Description 	: initializes the timer0
*******************************************/
void Timer0_Init(void)
{
 T0TCR = 0x02;		//Stop Timer
 T0CTCR = 0x00;		//Select Timer Mode
 T0PR = 15000;		//Timer Clock is running at 1KHz PCLK/T0PR = 1Khz
 T0TC = 0;			//Reset Timer Counting register to 0
 T0PC = 0;			//Reset Prescale counter register to 0
 T0MR0 = 1000;		//Match Register0 triggers interrupt at every 500mSec 1Khz/T0MRO=2Hz=500msec
 T0MCR = 0x0001;	//To generate an interrupt when MR0 matches the value in the TC.

 VICIntSelect = 0x00000000;		// Setting Timer 0 interrupt as IRQ(Vectored)
 VICVectCntl0 = 0x20|4;			// Assigning Highest Priority Slot to Timer 0 and enabling this slot
 VICVectAddr0 = (int)IRQ_Timer0; // Storing vector address Timer0 Interrupt
 VICIntEnable = (1<<4);			// Enabling Timer 0 interrupt

 T0TCR = 0x01;  				//Start Timer
}
 
/****************************************************************
* Function: Interrupt Service Routine
* Input: none
* Output: none
* Logic: whenever interrupt0 occurs it executes this routine
*        and after this it returns to the main function
*****************************************************************/
void __irq IRQ_Timer0(void)	//ISR for Timer 0 interrupt
{ 
  Temp=T0IR;					 //read status of all pending interrupts
  if(Temp & 0x01==0x01)			//check if interrupt0 request is pending
  {
   Toggle=~Toggle;				//used to toggle, it will on the buzzer if it is not and vice versa
   if(Toggle)
   {
    BUZZER_ON();				//buzzer on till next interrupt occurs
   }							
   else
   {
    BUZZER_OFF();			   //buzzer off till next interrupt occurs
   }
   T0TC=0;					   //reset the timer
   T0IR=0x01;				   //clears interrupt
  } 
  VICVectAddr = 0x00;   		//Acknowledge Interrupt
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
}
/*****************************************************************
* Function     : Init_Peripherals
* Input        : none
* Output       : none
* Logic        : to initialize peripherals, in this case it 
*                is buzzer so it will call the function to 
*                initialize buzzer port and also initialize timer
* Example call : called by the main function
*******************************************************************/
void Init_Peripherals(void)
{
 Init_Ports();
 Timer0_Init();
}
/*********************************************************************
* Function Name	: main
* Input			: None
* Output		: int to inform the caller that the program exited 
                  correctly or incorrectly (C code standard)
* Logic			: initialize all pheripherals 
* Example Call	: Called automatically by the Operating System
*
**********************************************************************/
int main()
{
 PINSEL0 = 0x00000000; //Reset all pins in GPIO mode
 PINSEL1 = 0x00000000;
 PINSEL2 = 0x00000000;
 Init_Peripherals();
 while(1);
} //main ends here


