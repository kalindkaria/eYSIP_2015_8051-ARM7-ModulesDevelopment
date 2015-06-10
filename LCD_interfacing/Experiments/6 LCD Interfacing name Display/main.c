/*
* Author: Bhumika Varshney, ERTS Lab, IIT Bombay
* File name: lcdtextdisp.c
* Functions: lcd_init(), lcd_clear(), lcd_cursor(char row, char column), lcd_string(char *str)
* This experiment demonstrates simple LCD display operation.
* Concepts covered: LCD Interfacing
  
 Note: 
 1. Make sure that in the configuration options following settings are 
 done for proper operation of the code
  Microcontroller: P89V51RD2
 Frequency: 11.0592 MHz								   
2.  Coding Standards followed(For further reference on Coding Standards go to portal link).
*********************************************************************************/
#include<intrins.h>
#include"p89v51rx2.h"
#include"lcd_display.h"
/*						  
* Function Name: main
* Input: None
* Output: int to inform the caller that the program exited correctly or incorrectly (C code standard)
* Logic: calls various functions given in the header file of lcd to display a string on lcd.
* Example Call: Called automatically by the Operating System
*
*/
void main (void)
{
lcd_init();	  //initializes the LCD
lcd_clear();  //clears screen of LCD
lcd_cursor(1,5);  //places cursor at 5th column of 2nd row
 lcd_string("YOUR NAME");  //writes your name on the screen of LCD
 while(1);
} //main ends here

