/*
* Author: Bhumika Varshney, ERTS Lab, IIT Bombay
* File name: display_special_charater.c
* Functions: character_heart(), character_smiley(), character_sad_smiley(), character_music_note(), character_big_smiley(),
*            character_shaock_smiley(), character_disgust_smiley(), character_diamond()
* This experiment demonstrates simple LCD display operation of some special characters.
* Concepts covered: LCD Interfacing
  
 Note: 
 1. Make sure that in the configuration options following settings are 
 done for proper operation of the code
  Microcontroller: P89V51RD2
 Frequency: 11.0592 MHz								   
2.  Coding Standards followed(For further reference on Coding Standards go to portal link).
******************************************************************************************************************************/
#include<intrins.h>
#include"p89v51rx2.h"
#include"lcd_display.h"
/***********************************************************************
* Function: character_heart()
* Input: none
* Output: heart shape character 
* Description: This function stores special character to be displayed.
*************************************************************************/
void character_heart()
{
	
	commandsend(64);  //Address where values of the first custom character is stored
	datasend(10);	 //sum of all column addresses which are required to glow in first row (8+2=10)
	datasend(21);	 //sum of all column addresses which are required to glow in second row(16+4+1=21)
	datasend(21);
	datasend(10);
	datasend(4);
	datasend(0);
	datasend(0);
	datasend(0);	
}
/***********************************************************************
* Function: character_smiley()
* Input: none
* Output: smiley face shape character 
* Description: This function stores special character to be displayed.
*************************************************************************/
void character_smiley()
{
	
	commandsend(72);  //Address where values of the first custom character is stored
	datasend(0);
	datasend(10);
	datasend(0);
	datasend(4);
	datasend(17);
	datasend(14);
	datasend(0);
	datasend(0);	
}
/***********************************************************************
* Function: character_sad_smiley()
* Input: none
* Output: sad face shape character 
* Description: This function stores special character to be displayed.
*************************************************************************/
void character_sad_smiley()
{
	
	commandsend(80);  //Address where values of the first custom character is stored
	datasend(0);
	datasend(10);
	datasend(0);
	datasend(4);
	datasend(0);
	datasend(14);
	datasend(17);
	datasend(0);	
}
/***********************************************************************
* Function: character_music_note()
* Input: none
* Output: music note shape character 
* Description: This function stores special character to be displayed.
*************************************************************************/
void character_music_note()
{
	
	commandsend(88);  //Address where values of the first custom character is stored
	datasend(1);
	datasend(3);
	datasend(5);
	datasend(9);
	datasend(9);
	datasend(11);
	datasend(27);
	datasend(27);	
}
/***********************************************************************
* Function: character_big_smiley()
* Input: none
* Output: happy face shape character 
* Description: This function stores special character to be displayed.
*************************************************************************/
void character_big_smiley()
{
	
	commandsend(96);  //Address where values of the first custom character is stored
	datasend(0);
	datasend(10);
	datasend(0);
	datasend(31);
	datasend(17);
	datasend(14);
	datasend(0);
	datasend(0);	
}
/***********************************************************************
* Function: character_shock_smiley()
* Input: none
* Output: shocking face shape character 
* Description: This function stores special character to be displayed.
*************************************************************************/
void character_shock_smiley()
{
	
	commandsend(104);  //Address where values of the first custom character is stored
	datasend(10);
	datasend(0);
	datasend(4);
	datasend(0);
	datasend(14);
	datasend(17);
	datasend(17);
	datasend(14);	
}
/***********************************************************************
* Function: character_disgust_smiley()
* Input: none
* Output: disgust face shape character 
* Description: This function stores special character to be displayed.
*************************************************************************/
void character_disgust_smiley()
{
	
	commandsend(112);  //Address where values of the first custom character is stored
	datasend(0);	  //sum of all column addresses which are required to glow in first row
	datasend(17);	  //sum of all column addresses required to glow in secong row(16+1=17)
	datasend(10);
	datasend(17);
	datasend(4);
	datasend(0);
	datasend(14);
	datasend(17);	
}
/***********************************************************************
* Function: character_diamond()
* Input: none
* Output: diamond shape character 
* Description: This function stores special character to be displayed.
*************************************************************************/
void character_diamond()
{
	
	commandsend(120);  //Address where values of the first custom character is stored
	datasend(0);
	datasend(4);
	datasend(10);
	datasend(17);
	datasend(10);
	datasend(4);
	datasend(0);
	datasend(0);	
}
/*						  
* Function Name: main
* Input: None
* Output: int to inform the caller that the program exited correctly or incorrectly (C code standard)
* Logic: calls various functions to display special chracters on the LCD.
* Example Call: Called automatically by the Operating System
*
*/
void main()
{
	lcd_init();	 //initializes the LCD
	lcd_clear();  //clears the LCD
	character_heart();	 //stores heart shape character
	lcd_cursor(1,1);	 //sets location where the character has to be displayed
	/*different refrence numbers are used for different addresses in memory, for example-
	0 for address 64, 1 for address 72, 2 for address 80 and so on... */   
	datasend(0);	   //displays custom character stored on location 64 on the screen of LCD
	
	/*Similarly more characters can be displayed*/ 
	character_smiley();
	lcd_cursor(1,3);
	datasend(1);
	character_sad_smiley();
	lcd_cursor(1,5);
	datasend(2);
	character_music_note();
	lcd_cursor(1,7);
	datasend(3);
	character_big_smiley();
	lcd_cursor(2,2);
	datasend(4);
	character_shock_smiley();
	lcd_cursor(2,4);
	datasend(5);
	character_disgust_smiley();
	lcd_cursor(2,6);
	datasend(6);
	character_diamond();
	lcd_cursor(2,8);
	datasend(7);
	while(1);
}	//main ends here