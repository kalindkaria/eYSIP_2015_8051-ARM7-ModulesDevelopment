
#include  <lpc214x.h>

/***************Macros*********************/ 
#define Fosc            12000000                    //10MHz~25MHz
#define Fcclk           (Fosc * 5)                  //Fosc(1~32)<=60MHZ
#define Fcco            (Fcclk * 4)                 //CCO Fcclk 2񅦰�16�156MHz~320MHz
#define Fpclk           (Fcclk / 4) * 1             //VPB(Fcclk / 4) 1񄿔
#define UART_BPS	9600 		//Change Baud Rate Setting here


/******************************************/

/*****Function Prototypes*********************/

void Delay(unsigned char j);

void Init_Peripherals(void);
void Init_Ports(void);
void  __irq IRQ_UART1(void);
void DelaymSec(unsigned int Delay);
void Init_UART1(void);
void UART1_SendByte(unsigned char data);
void UART1_SendStr(const unsigned char *str);

/**********************************************/

/******************Global Variables****************/

unsigned char Temp=0;
/**************************************************/

//This function is UART0 Receive ISR. This functions is called whenever UART0 receives any data


/************************************************************

	Function 		: Init_UART1
	Return type		: None
	Parameters		: None
	Description 	: Initialises UART1 module. 
************************************************************/
void Init_UART1(void)
{  
   unsigned int Baud16;
   PINSEL0&=0xFFF0FFFF;
   PINSEL0|=0x00050000;

   U1LCR = 0x83;		            // DLAB = 1
   Baud16 = (Fpclk / 16) / UART_BPS;  
   U1DLM = Baud16 / 256;							
   U1DLL = Baud16 % 256;						
   U1LCR = 0x03;
   U1IER = 0x00000001;		//Enable Rx interrupts

   VICIntSelect = 0x00000000;		// IRQ
   VICVectCntl0 = 0x20|7;			// UART1
   VICVectAddr0 = (int)IRQ_UART1; 	//UART1 Vector Address
   VICIntEnable = (1<<7);	// Enable UART1 Rx interrupt

}
				



//This function sends a string of characters on the serial port
void UART1_SendStr(const unsigned char *str)
{  
   while(1)
   {  
      if( *str == '\0' ) break;
      UART1_SendByte(*str++);	    
   }
}


