							  

#include <intrins.h>
#include "p89v51rx2.H"
#include "lcd_display.h"

//sbit buzzer=P2^7;               //buzzer = 1; buzzer off, buzzer = 0; buzzer on,
sbit LB=P1^0;	                //LEFT BACK Pin
sbit LF=P1^1;	        	//LEFT FRONT Pin
sbit RF=P1^2;	        	//RIGHT FRONT Pin
sbit RB=P3^4;	        	//RIGHT BACK Pin
sbit PORT3_3=P3^3;	 
sbit TCON_2=TCON^2;
sbit IEN0_7=IEN0^7;
sbit IEN0_3=IEN0^2;
sbit left_velocity=P1^3;          //Left velocity control pin. 
sbit right_velocity=P1^4;         //Right velocity control pin.
 
sbit start_conv = P3^6;		 //P3^6 is connected to Start and ALE pin of ADC0808
sbit output_enable = P3^7;	 //P3^7 is connected to Output Enable Pin of ADC0808

unsigned char left_motor_velocity = 0x00;
unsigned char right_motor_velocity = 0x00;

unsigned char ser_data_upper_nibbel = 0x00;
unsigned char ser_data_lower_nibbel = 0x00;

unsigned int left_motor_pulse_count = 0x00;
unsigned int left_motor_pulse_count_locked =0x00;

unsigned char data_array[8];    //initialize an array of 8 elements to display onto the LCD screen
unsigned char i=0;              //used in main function during ADC conversion 



void lcd_display(void)
{
int i=0;
unsigned char lcd_data1[16]={"  FBV GUI MODE  "};  // While defining the character array make sure that it is less than 16 characters(for each line of LCD) & long enough to hold all characters in the string 
unsigned char lcd_data2[16]={"NEX Robotics IND"};

 commandsend(0x01); // clear display.
 commandsend(0x06);  // auto increment cursor position

for(i=0;i<16;i++)
 {
  datasend(lcd_data1[i]);
 }

commandsend(0xC0);         //80h+40h. 80h for setting cursor position
			   //40h is the address for the first character on thesecon line
for(i=0;i<16;i++)
 {
  datasend(lcd_data2[i]);
 }
}

unsigned char ADC_conversion(unsigned char ADC_ch_no)
{
unsigned char ADC_data;

P0 = ADC_ch_no;		  // to select channel no. send address on P0
start_conv = 1;           // ADC0808 will latch the address on L-H transition on the ALE pin
start_conv = 0;	          // ADC0808 will start conversion on H-L transition on the start pin
delay_ms(2);	          // conversion time for ADC0808
P0 =0xFF;	          // output enable and read
output_enable = 1;	  //Enabling o/p of ADC0808 for to read the converted data
ADC_data = P0;		  //storing adc_data
output_enable = 0;	  //disabling ADC 0808 o/p
return ADC_data;	  //returning adc_data
}


 //initialization routine for Interrupt INT1 setup.
void int1_setup() //Initalisation of the Int 1 should be done in the same sequence as given in this function.
{
TCON_2 = 1;       //Set Interrupt 1 trigger type to falling edge trigger.
IEN0_7=1;	  //IEN0 = 0x84; //External Interrupt 1 Enable and Global Interrupt Enable. 
IEN0_3=1;
PORT3_3=1;        //set P3.3 (INT 1) as input port.
}

void pca_left_init(void)
{
 CMOD=0x80;	  //80 sets PCA counter to run at Fosc/6; 
 CCON=0x40;	  //start PCA counter
 						
 CCAP0L=0x00;   
 CCAP0H=0xFF;     //Left motor duty cycle register
  
 
 CCAPM0=0x42;     //enable PWM mode and ECOM bits for left motor
 CCAPM1=0x42;     //enable PWM mode and ECOM bits for right motor
}

void pca_right_init(void)
{
 CMOD=0x80;	  //80 sets PCA counter to run at Fosc/6; 
 CCON=0x40;	  //start PCA counter
 						
   
 CCAP1L=0x00;				  
 CCAP1H=0xFF;    //Right motor duty cycle register

 CCAPM0=0x42;    //enable PWM mode and ECOM bits for left motor
 CCAPM1=0x42;    //enable PWM mode and ECOM bits for right motor
}

void uart_setup()
{
    TMOD = 0x20;		// configure timer1 for Mode 2 operation for the correct baud rate 
    TH1 = 0xFD;     	        // 9600 bps for 11.0592 MHz clock 
    TCON = 0x40;    	        // Start timer 1 by setting TR1 = 1 
    SCON = 0x50;  		// Set Serial IO to receive and normal mode 
	RI=0;                   //Receive Interrupt Flag is cleared
	TI=0;  		        //Tx interrupt flag is cleared
}	

 void TxData(unsigned char tx_data)
{
 	

	SBUF = tx_data;      //Transmit data that is passed to this function
	while(TI != 1);	     //wait while data is being transmitted
}


  unsigned char RxData()
{
    unsigned char rcv_data;
	rcv_data = SBUF;     //if any data is available copy it from SBUF
	return rcv_data;     //return the received data
}

//ISR for left position encoder

void int1_isr(void)interrupt 2
{

 left_motor_pulse_count++;
 IE1=0;
}

void serial_int_isr(void)interrupt 4
{
unsigned char rx_data;

 rx_data = RxData();
 
//________________ IR PROXIMITY STATUS COMMANDS _____________________
 if(rx_data==0xc2)			      //IR left
  {
  TxData(data_array[3]);
  }

 if(rx_data==0xc3)			      //IR center
  {							   

  TxData(data_array[0]);
  }

 if(rx_data==0xc4)			       //IR Right
  {
  TxData(data_array[7]);
  }
 //________ BATT VALUE + WHITE LINE SENSORS + SHARP SENSORS _________
  if(rx_data==0x60)			     //batt_voltage
  {
  TxData(data_array[1]);
  }

 if(rx_data==0x64)			     //white line left
  {
  TxData(data_array[4]);
  }

 if(rx_data==0x65)			     //white line center
  {
  TxData(data_array[5]);
  }

 if(rx_data==0x66)			     //white line right
  {
  TxData(data_array[6]);
  }

 if(rx_data==0x62)			      //sharp sensor
  {
  TxData(data_array[2]);
  }

//________________ Buzzer status _____________________
 if(rx_data==0x69)			      //buzzer on
  {
  buzzer=0;					 
  }

 if(rx_data==0x6A)			      //buzzer off
  {
 buzzer=1;
  }

//__________________________Motion Control_____________
 if(rx_data==0x51)	    		      //forward
  {
   left_velocity=1; 
   right_velocity=1;
   RF=1;
   LF=1;
   RB=0;
   LB=0;
  }

 if(rx_data==0x52)		    	      //backward
  {
   left_velocity=1; 
   right_velocity=1;
   RF=0;
   LF=0;
   RB=1;
   LB=1;
  }

if(rx_data==0x54)			      //left
  {
   left_velocity=1; 
   right_velocity=1;
   RF=1;
   LF=0;
   RB=0;
   LB=1;
  }
							 
  if(rx_data==0x53)	      	              //right
  {
   left_velocity=1; 
   right_velocity=1;
   RF=0;
   LF=1;
   RB=1;
   LB=0;
  }

  if(rx_data==0x59)			      //stop
  {
   left_velocity=1; 
   right_velocity=1;
   RF=0;
   LF=0;
   RB=0;
   LB=0;
  }

//__________________________Locomotoion setting commands_____________ 
 if((rx_data & 0xF0) == 0x10)
 {
 pca_left_init();
 ser_data_lower_nibbel = rx_data & 0x0F;
 left_motor_velocity = ser_data_lower_nibbel;
 } 

 if((rx_data & 0xF0)==0x20)
  { 
   
   ser_data_lower_nibbel = rx_data & 0x0F;
   left_motor_velocity = ((ser_data_lower_nibbel * 16) + left_motor_velocity);
   CCAP0H=(0xFF - left_motor_velocity);
  }
 
  if((rx_data & 0xF0) == 0x30)
 {
  pca_right_init();
  ser_data_lower_nibbel = rx_data & 0x0F;
  right_motor_velocity = ser_data_lower_nibbel;
 }	 

 if((rx_data & 0xF0)==0x40)
  { 
   ser_data_lower_nibbel = rx_data & 0x0F;
   left_motor_velocity = ((ser_data_lower_nibbel * 16) + right_motor_velocity);
   CCAP1H= ( 0xFF - left_motor_velocity);
  }
/*************************************************************************************************************/
 if(rx_data==0x72)				 //Lower_byte number of pulse counted
  {
   left_motor_pulse_count_locked=left_motor_pulse_count;
   TxData(left_motor_pulse_count_locked % 256);
  }  

 if(rx_data==0x73)				 //Upper_byte number of pulse counted
  {
   TxData(left_motor_pulse_count_locked / 256);
  }  
 RI=0;
 TI=0;
 }

 //-------------------------------------------------------------------------------
//Main Function
//--------------------------------------------------------------------------------
void main()
{

 unsigned char i=0;
 output_enable = 0;
 start_conv = 0;    //de-assert all control signals to ADC
 IEN0 = 0x90;
 buzzer = 1;        //buzzer off
 lcd_init();        //Initialize LCD
 uart_setup();	    //Initialize Uart 
 int1_setup();	    //Initialize hardware Interrupt INT1 
 lcd_display(); 
 while(1)
 {

  for(i=0;i<8;i++)
  {
   data_array[i] = ADC_conversion(i);
  }
 }
}