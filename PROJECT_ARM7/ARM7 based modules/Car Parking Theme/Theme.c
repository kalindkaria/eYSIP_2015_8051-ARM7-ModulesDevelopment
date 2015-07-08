#include<LPC214x.h>	
#include "Pos.h"
#include "LCD.h"										
#include "adc.h"
#include"spi.h"
#include "buzzer.h"
#include "led.h"
#include "xbee.h"

#define	SPI1_SLAVE_SELECT	0x00100000
#define SENSOR_OFF() IO1SET=(1<<16)		//Macro to turn OFF Sensors
#define SENSOR_ON() IO1CLR=(1<<16)
#define LED_OFF() IO0CLR=(1<<4) //Macro to turn OFF Led
#define LED_ON() IO0SET=(1<<4)
/***************Function Prototypes****************/
void velocity(unsigned int x,unsigned int y);
void Init_Peripherals(void);
void Init_Ports(void);
void node(void);
BYTE MEGA8_ADCRead(BYTE channel);
void destination(int x,int y, int z);
 void  __irq IRQ_UART1(void);
 void required (unsigned long int distance) ;
char L=0;
char R=0;


/***********Global variables**************************/
unsigned char flag=0;  //used in white line following logic
unsigned int left_white_line=0;
unsigned int center_white_line=0;
unsigned int right_white_line=0;
unsigned int start=0;
unsigned int left_sensor;
unsigned int right_sensor;
unsigned int n=0;
volatile int path=0;  
volatile int turn=0;
volatile int slot=0;
void velocity(unsigned int x,unsigned int y)
{
 	  PWMMR2 = x;
 	  PWMLER = 0x04;	//match register 2 enabled
	
 	  PWMMR5 = y;
 	  PWMLER = 0x20;	//match register 5 enabled

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
Init_Led_Pin();
 Init_Motion_Pin();
 Init_LCD_Pin();
 Init_ADC_Pin();
  Ext_INT_Setup();
  Init_UART1();
}

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

/***********************************************************
* Function     : Init_Peripherals
* Input        : none
* Output       : none
* Logic        : to initialize all peripherals
* Example call : called by the main function
***********************************************************/
void Init_Peripherals(void)
{
 Init_Ports();
 Init_PWM();
 Init_ADC0();
 Init_ADC1();
 SPI1_Init();
 Init_Sensor_Switch_Pin();
}


//This function sends a single character on the serial port
void UART1_SendByte(unsigned char data)
{  
   U1THR = data;				    
   while( (U1LSR&0x40)==0 );	    
}



void node(void)
{
while(1)
{
 	left_white_line=AD1_Conversion(3); 
	center_white_line=AD0_Conversion(1);
	right_white_line=AD0_Conversion(2);
	
	LCD_Print(1,1,left_white_line,3);
  	LCD_Print(1,6,center_white_line,3);
  	LCD_Print(1,11,right_white_line,3);
	LCD_Print(2,1,flag,1);

	flag=0;


	  if(center_white_line>30)            //Monitor center black line
    {
      flag=1;
      forward();
      velocity(450,450); 
    }

    if((left_white_line<30) && (flag==0) && (right_white_line>30))    //Monitor left black line
    {
      forward();
      velocity(450,370);
    }

    if((right_white_line<30) && (flag==0) && (left_white_line>30) )      //Monitor right black line
    {
      forward();
      velocity(370,450);
    }

    if(left_white_line<30 && right_white_line<30 &&(flag==0))    // Random motion to again follow black line
    {
      forward();
      velocity(420,420);     
    }

    if((left_white_line>30 && center_white_line>30)| (right_white_line>30 && center_white_line>30))
    {
	  break;
	}
  }
}

void right_turn(void)
{
	Forward_mm(70);
	stop();
	delay_ms(500);
	Right_Degrees(45);
	while(1)
	{
	left_white_line=AD1_Conversion(3); 
	center_white_line=AD0_Conversion(1);
	right_white_line=AD0_Conversion(2);
	
	LCD_Print(1,1,left_white_line,3);
  	LCD_Print(1,6,center_white_line,3);
  	LCD_Print(1,11,right_white_line,3);
	LCD_Print(2,1,flag,1);
	right();
		velocity(350,350);
	if(AD0_Conversion(1)>30 && AD1_Conversion(3)<30 && AD0_Conversion(2)<30)
	 {
	 stop();
	 break;
	  }
	}
		  

 }
 void left_turn(void)
{
	Forward_mm(70);
	stop();
	delay_ms(500);
	Left_Degrees(45);

	while(1)
	{
	left_white_line=AD1_Conversion(3); 
	center_white_line=AD0_Conversion(1);
	right_white_line=AD0_Conversion(2);
	
	LCD_Print(1,1,left_white_line,3);
  	LCD_Print(1,6,center_white_line,3);
  	LCD_Print(1,11,right_white_line,3);
	LCD_Print(2,1,flag,1);
	left();
	velocity(350,350);
	 if(AD0_Conversion(1)>30 && AD1_Conversion(3)<30 && AD0_Conversion(2)<30)
{
stop();
	 break;
 }
}
 }
	
void buzz(void)
{
  BUZZER_ON();
  DelaymSec(300);
  BUZZER_OFF();
}
 void Led(void)
 {
 Init_Led_Pin();
   LED_ON();
	DelaymSec(1000);
	LED_OFF(); 
 }

void reach(void)
{
node(); 
if(start==0) left_turn();
else  right_turn();

 node();
 right_turn(); 
  }

 void detect(void)
 {
	 
	if(start==0)
 	{
 		L= 0x01  ;	
 		R=0x09  ;
 	}
 	else
 	{
		 L=	0x16 ;
		 R=	0x15  ;
	 } 
  	n=0;
  	while(n<5)
  	{
   		node();
   		Forward_mm(40);
   		stop();
			DelaymSec(200);
		left_sensor= MEGA8_ADCRead(7);
		DelaymSec(200);
		right_sensor= MEGA8_ADCRead(14);
		LCD_Print(2,5,left_sensor,3);
		LCD_Print(2,9,right_sensor,3);
		DelaymSec(200);

		if(left_sensor<60)
   		{
   	 		if(n==4 && start==1)	
  				UART1_SendByte(L+=0x06);
				
			else 
				 UART1_SendByte(L);
	
			buzz();
		 }
		 else	Led();
	
		DelaymSec(500);

		if(right_sensor<60)
   		{
	   	 	if(start==0)
			{
			if(n>0)		UART1_SendByte(R);
			else		UART1_SendByte(R+0x07);
			}
	
			if(start==1)	UART1_SendByte(R);
		 
	
			buzz();
		  } 
		  else	Led();

		  if(n==0 && start==0)
		  {
		  }
		 else R--;
		  n++;
		  L++;
	}  
	start++;
 }	 	
  

 void  __irq IRQ_UART1(void)
{  
 	Temp = U1RBR;
	
	 if(Temp == 0x31) 
	  {
	  path=1;
	  slot=1;
	  turn=1;
	   }
	  
	 if(Temp == 0x32) 
	  {
	  slot=2;
	  path=1;
	  turn=1;
	     }
	    
	 if(Temp == 0x33) 
	  {
	  slot=3;
	  path=1;
	  turn=1;
	   }
	  
	 if(Temp == 0x34) 
	  {
	  slot=4;
	  path=1;
	  turn=1;
	   }
	  
	 if(Temp == 0x35) 
	  {
	slot=5;
	  path=1;
	  turn=1;
	     }
	  	
	 if(Temp == 0x62) 
	  {
	  slot=1;
	  path=2;
	  turn=1;
	   }
	  
	 if(Temp == 0x63) 
	  {
	  slot=2;
	  path=2;
	  turn=1;
	   }
	  
	 if(Temp == 0x64) 
	  {
	   slot=3;
	  path=2;
	  turn=1;
	  }
	  
	 if(Temp == 0x65) 
	  {
	 slot=4;
	  path=2;
	  turn=1;
	    }
	  
	 if(Temp == 0x66) 
	  {
	 slot=5;
	  path=2;
	  turn=1;
	    }
	
	    
	 if(Temp == 0x61) 
	  {
	  slot=1;
	  path=1;
	  turn=2;
	   }
	   if(Temp == 0x39) 
	  {
	  slot=2;
	  path=1;
	  turn=2;
	   }
	
	   if(Temp == 0x38) 
	  {
	  slot=3;
	  path=1;
	  turn=2;
	   }
	
	   if(Temp == 0x37) 
	  {
	  slot=4;
	  path=1;
	  turn=2;
	   }
	
	   if(Temp == 0x36) 
	  {
	 slot=5;
	  path=1;
	  turn=2;
	    }
	
	   if(Temp == 0x67) 
	  {
	 slot=5;
	  path=2;
	  turn=2;
	   }
	
	   if(Temp == 0x68) 
	  {
	 slot=4;
	  path=2;
	  turn=2;
	   }
	
	   if(Temp == 0x69) 
	  {
	 slot=3;
	  path=2;
	  turn=2;
	   }
	
	   if(Temp == 0x6a) 
	  {
	  slot=2;
	  path=2;
	  turn=2;
	  }
	   
	   if(Temp == 0x6b) 
	  {
	  slot=1;
	  path=2;
	  turn=2;
	  }
	Led();     
	
	 VICVectAddr = 0x00;
	 VICIntEnable = (0<<7);
}		


	void park(void)
    {
	    while(slot>0)
		{
		  node();
		  slot--;
		  if(slot>0) Forward_mm(35);
		 }
		 stop();
		 buzz();
		 delay_ms(200);
		 Led();
		 if (turn==1)	left_turn();
		 else if(turn==2)	right_turn();
	
		  node();
		  stop();
		  Forward_mm(120);
		  stop();
		  buzz();
		  Led();
		  delay_ms(200);
		  buzz();
		  Led();

	  }


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
/*	
  while(1)
  {
  LCD_Print(1,1,MEGA8_ADCRead(7),3);
  LCD_Print(1,5,MEGA8_ADCRead(14),3);
  
  
  }*/	
   velocity(450,450);
 UART1_SendByte(0x41);
 
 velocity(450,450);
 reach();
 detect();
 reach();
 detect();
 node();
 right_turn();
 node();
 stop(); 
 while(1)
 {
   if(path==1)
   {
     Forward_mm(35);
	 node();
	 right_turn();
	 park();
	 break;
   	}
	else if(path==2)
	{
	  Forward_mm(35);
	  right_turn();
	  node();
	  left_turn();
	  park();
	  break;
	}
   
   }
 }


