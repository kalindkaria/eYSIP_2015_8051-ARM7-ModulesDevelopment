/*
* Author: Bhumika Varshney, ERTS Lab, IIT Bombay
* File name: lcd_display.h
* Functions: delay_ms(unsigned int ms), ready(void), char swap(unsigned char tempdata), commandsend(unsigned char command),
*            datasend(unsigned char command), lcd_init(void), lcd_cursor(char row, char column), lcd_string(char *str), lcd_clear()
* Global variables: buzzer, RS, RW, E, BUSY, A
* This is a header file which contains all the useful functions for LCD operations.
* Concepts covered: LCD Interfacing
  
 Note: 
 1. Make sure that in the configuration options following settings are 
 done for proper operation of the code
  Microcontroller: P89V51RD2
 Frequency: 11.0592 MHz								   
2.  Coding Standards followed(For further reference on Coding Standards go to portal link).
*********************************************************************************/
//program specific declarations
sbit buzzer=P2^7;  //buzzer = 1; buzzer off, buzzer = 0; buzzer on,
sbit RS = P2^6;    //P2^6 is connected to the RS line of LCD 
sbit RW = P2^5;    //P2^5 is connected to the RW line of LCD
sbit E = P2^4;	   //P2^4 is connected to the EN line of LCD
sbit BUSY = P2^3;  //P2^3 is connected to the DB3 pin of LCD

/*void delay_ms(unsigned int ms);
void ready(void);
unsigned char swap(unsigned char tempdata);
commandsend(unsigned char command);
datasend(unsigned char command);
lcd_init(void);
lcd_cursor(char row, char column);
lcd_string(char *str);
lcd_clear();
void lcd_print(char row,char column,unsigned int value,int digits);*/

unsigned char A=0; //used in the swap(unsigned char tempdata) function
unsigned int unit;
unsigned int tens;
unsigned int hundred;
unsigned int thousand;
unsigned int million;
unsigned int temp;

/**************************************************************************** 
*Function name - delay_ms
* Input - time in milli seconds 
* Outut - gives delay for the spcified time before executing next instruction
* Logic - delay can be given in the program by introducing loops
******************************************************************************/
void delay_ms(unsigned int ms)
{
unsigned int i,j;

for(i=0;i<ms;i++)
for(j=0;j<53;j++);
}
 /**************************************************************************************************************** 
* Function name - ready
* Input - none 
* Outut - gives status of the busy bit(D7)
* Logic - subroutines required to check if LCD has finished executing previous command and is ready to accept next one
*****************************************************************************************************************/ 
 void ready(void) 
 {
  bit readybit;
  unsigned char buzzer_status = 0;
  buzzer_status = P2 & 0x80; //storing original buzzer status
    
  E = 0;
  RS = 0;			 //  to select comand register
  RW = 1;			 // to select read mode

  while(readybit == 1)	 // keep checking MSB bit till busy goes low
  {
   E = 1;		 // high to low transition on E pin requird to clock in data
   P2 = 0x7F | buzzer_status;		 // to set this pin in read mode to read busy signal from MSB (D7) of the LCD
   readybit = P2^3;
   E = 0;
  }
 }

/******************************************************************************************************
* Function: swap(unsigned char tempdata)
* Input: data to be swapped
* Output: data after swapping
* Description: This function swaps the data which is passed to it in such away that higher nibble is pushed in place of
				 lower nibble because lower nibble of microcontroller is being used for data transfer.
*******************************************************************************************************/

unsigned char swap(unsigned char tempdata)
{
A = tempdata;
A = _cror_(A,4);	// this uses an inbuilt rotate function to get MSB's in LSB's
tempdata = A & 0x0F;
return tempdata;
}	

/****************************************************************
* Function: commandsend(unsigned char command)
* Input: command to be send
* Output: command execution
* Description: This function sends data to LCD as command by
*				 selecting the instruction register
****************************************************************/

void commandsend(unsigned char command)
{ 
unsigned char temp;
unsigned char buzzer_status = 0;
  
buzzer_status = P2 & 0x80; //storing original buzzer status

ready();       // checking for busy bit to send next nibble
temp = swap(command);
P2 = temp | buzzer_status; // to get higher nibble
RW = 0;        // to select write mode
RS = 0;        // to select command register
E = 1;         // to get the enable high to low transition
E = 0;         // in order to gate in the data

P2 = (command & 0x0F) | buzzer_status; // to get the lower nibble
RW = 0;
RS = 0;        // repeat again all above steps    

E = 1;
E = 0;
delay_ms(2);  //delay of 2 ms
}

/***********************************************************
* Function: datasend(unsigned char command)
* Input: data to be send
* Output: data written on LCD 
* Description: This function sends data to LCD as data by
*				 selecting the data register
***********************************************************/

void datasend(unsigned char lcddata)
{
unsigned char temp;
unsigned char buzzer_status = 0;

buzzer_status = P2 & 0x80; //storing original buzzer status

temp = swap(lcddata);
ready();	// checking for busy bit to send next nibble
P2 = temp | buzzer_status;  // to get higher nibble
RW = 0;	 // to select write mode
RS = 1;  // to select data register
         
E = 1;
E = 0;


P2 = (lcddata & 0x0F)  | buzzer_status;
RW = 0;
RS = 1;
E = 1;
E = 0;
delay_ms(2);	 //delay of 2 ms
}

/***********************************************************************
* Function: lcd_init(void)
* Input: none
* Output: initializes the LCD by giving proper commands
* Description: This function initialises LCD display in 4-bit mode.
*              The intialisation functions are 8-bit wide, so we write
*			  each inst into two seperate nibbles.
*			  Following instructions are used for Lcd init.
*			  1.>  0x30 8-bit mode
*			  2.>  0x28 4-bit mode and 5x8 dot character font
*			  3.>  0x0E Turn ON lcd and cursor
*			  4.>  0x06 Autoincrement cursor position
*			  5.>  0x01 Clear Lcd display	
*			  6.>  0x80 for setting cursor position
*************************************************************************/

void lcd_init(void)
{						
unsigned char buzzer_status = 0;

buzzer_status = P2 & 0x80; //storing original buzzer status


RS = 0;	 //  to select comand register
RW = 0;	 // to select write mode
delay_ms(40);  // on power ON we must allow a delay of 40ms for VCC to settle
 
E = 0;
P2 = 0x03 | buzzer_status; // send instruction to set FUNCTION SET to 8 bit mode though we have only connected 4 bit data bus because by default on power ON LCD is in 8 bit mode
E =  1;
E = 0;

delay_ms(4);  // delay of 4ms
  
P2 = 0x03 | buzzer_status;	  // send the same instruction two times again. This way the LCD controller knows that we are operating in 4 bit mode
E =  1;
E = 0;
delay_ms(2);	   // delay of  2ms
P2 = 0x03  | buzzer_status;
E =  1;
E = 0;
delay_ms(2); // delay of  2ms

P2 = 0x02  | buzzer_status;		 // send FUNCTION SET instruction for 4 bit mode
E =  1;
E = 0;
ready();
P2 = 0x02  | buzzer_status; // again we send FUNCTION SET higher bits to keep 4 bit mode operation but main aim is to set the parameters in lower bits

E =  1;
E = 0;
P2 = 0x08  | buzzer_status; // these are the lower bits of FUNCTION SET. They set LCD to 1 display line and set font size to 5 x 8 dots

E =  1;
E = 0;
ready();
P2 = 0x00  | buzzer_status; // higher bits of DISPLAY OFF instruction
E =  1;
E = 0;
P2 = 0x08  | buzzer_status;  // lower bits of DISPLAY OFF instruction
E =  1;
E = 0;
ready();
P2 = 0x00  | buzzer_status;  // higher bits of CLEAR instruction
E =  1;
E = 0;
P2 = 0x01  | buzzer_status; // lower bits of CLEAR instruction
E =  1;
E = 0;
ready();
P2 = 0x00  | buzzer_status;// higher bits of ENTRY MODE instruction
E =  1;
E = 0;

P2 = 0x06  | buzzer_status;// lower bits of ENTRY MODE instruction to set increment i.e display to right side mode and no shift
E =  1;
E = 0;
ready();

commandsend(0x0F); // set LCD ON; CURSOR OFF; BLINK ON.
commandsend(0x01); // clear display.
commandsend(0x06); // to set cursor auto increment
commandsend(0x80); //Set cursor position to first line first character
} 
 /*************************************************************************
* Function name - lcd_cursor
* Input - desired row and column 
* Outut - places cursor at defined row and column
* Logic - sends appropriate commands to place cursor at the given location
**************************************************************************/ 
void lcd_cursor(char row, char column)
{
	switch (row) {
		case 1: commandsend(0x80 + column - 1); break;
		case 2: commandsend(0xc0 + column - 1); break;
		case 3: commandsend(0x94 + column - 1); break;
		case 4: commandsend(0xd4 + column - 1); break;
		default: break;
	}
}
/*************************************************************************
* Function name - lcd_string
* Input - string to be displayed 
* Outut - displayed string on screen
* Logic - sends appropriate commands and calls functions to display 
*          string at the given location
**************************************************************************/
void lcd_string(char *str)
{
	while(*str != '\0')
	{
		datasend(*str);
		str++;
	}
	commandsend(0x0C);
}
/*************************************************************************
* Function name - lcd_clear
* Input - none 
* Outut - clears LCD screen
* Logic - sends appropriate commands to clear screen of LCD.
**************************************************************************/
void lcd_clear()
{
	commandsend(0x01);
	commandsend(0x80);
}
/********************************************************************************
* Function name - lcd_print
* Input - a)row in which data is to be written
*         b)column in which data is to be written
*         c)input value to be displayed on LCD screen
*         d)desired number of digits to be displayed 
* Output- displayed value on the screen of LCD
* Description: Function To Print Any input value upto the desired digit on LCD
* Logic - recognizes the input and gives commands accordingly and also
*         transforms the input value in desired number of digits.
**********************************************************************************/
void lcd_print (char row, char coloumn, unsigned int value, int digits)
{
	unsigned char flag=0;
	if(row==0||coloumn==0)
	{
		commandsend(0x80);
	}
	else
	{
		lcd_cursor(row,coloumn);
	}
	if(digits==5 || flag==1)
	{
		million=value/10000+48;
		datasend(million);
		flag=1;
	}
	if(digits==4 || flag==1)
	{
		temp = value/1000;
		thousand = temp%10 + 48;
		datasend(thousand);
		flag=1;
	}
	if(digits==3 || flag==1)
	{
		temp = value/100;
		hundred = temp%10 + 48;
		datasend(hundred);
		flag=1;
	}
	if(digits==2 || flag==1)
	{
		temp = value/10;
		tens = temp%10 + 48;
		datasend(tens);
		flag=1;
	}
	if(digits==1 || flag==1)
	{
		unit = value%10 + 48;
		datasend(unit);
	}
	if(digits>5)
	{
		lcd_string("ERROR");
	}
}

