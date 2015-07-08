#include <intrins.h>
#include"motion.h"

unsigned int left_shaft_count=0;//initialize a variable to store the count pulses of the left position encoder.
unsigned int reqd_shaft_count_int=0; //initialize a variable to store required number of count pulses to move for desired distance
unsigned int distance=0;  //initialize a variable to store desired distance
unsigned int degrees=0;

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

P3=0x08; //set P3.3 (INT 1) as input port.
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

/***************************************************************************************
* Function - move_forward
* Input - desired distance to be moved
* Output - motion of robot
* Logic - counts the required shaft count and checks for the left shaft count if it has 
*         moved the desired ditance or not
****************************************************************************************/
void move_forward(unsigned int distance)
{
	left_shaft_count=0;
	reqd_shaft_count_int=(unsigned int)(distance/5.338);  //This equation will calculate the count required for distance to be travelled. 
  	int1_setup();//external linterrupt 1 initialization.
  	  					
 	left_velocity = 1;	 //setting motor enable pins to 1
 	right_velocity = 1; //setting motor enable pins to 1
	
	forward();	  //go forward
  	while(left_shaft_count<reqd_shaft_count_int)
	{
			lcd_print(1,1,reqd_shaft_count_int,2);
  		   	lcd_print(1,5,left_shaft_count,2);
	}; //wait till desired distance is covered.                          

 	stop();	 //stop motor	  		
 	
}

/***************************************************************************************
* Function - right_turn
* Input - none
* Output - turns the robot 90 degree right
* Logic - counts the required shaft count and checks for the left shaft count if it has 
*         moved the desired angle or not
****************************************************************************************/
void right_degrees(unsigned int degrees)
{
	left_shaft_count=0;
	reqd_shaft_count_int=(unsigned int)(degrees/4.090); //This equation will calculate the count required for distance to be travelled.
   					
 	left_velocity = 1;	 //setting motor enable pins to 1
 	right_velocity = 1; //setting motor enable pins to 1
 	
	soft_right();	  //go forward                            
 	while(left_shaft_count<reqd_shaft_count_int)
 	{
 		lcd_print(1,1,reqd_shaft_count_int,2);
 		lcd_print(1,5,left_shaft_count,2);
 	}; //wait till desired distance is covered.
 
 	stop();
}

void left_degrees(unsigned int degrees)
{
	  left_shaft_count=0;
	reqd_shaft_count_int=(unsigned int)(degrees/4.090); //This equation will calculate the count required for distance to be travelled.
   					
 	left_velocity = 1;	 //setting motor enable pins to 1
 	right_velocity = 1; //setting motor enable pins to 1
 	
	soft_left();	  //go forward                            
 	while(left_shaft_count<reqd_shaft_count_int)
 	{
 		lcd_print(1,1,reqd_shaft_count_int,2);
 		lcd_print(1,5,left_shaft_count,2);
 	}; //wait till desired distance is covered.
 
 	stop();
}
