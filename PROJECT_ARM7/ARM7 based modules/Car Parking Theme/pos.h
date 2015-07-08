#include  <lpc214x.h>
#include"pwm.h"


/*********Function Prototypes************/

void Angle_Rotate(unsigned int Degrees);
void Linear_Distance_mm(unsigned int DistanceInMM);
void Forward_mm(unsigned int DistanceInMM);
void Back_mm(unsigned int DistanceInMM);
void Left_Degrees(unsigned int Degrees);
void Soft_Left_Degrees(unsigned int Degrees);
void Right_Degrees(unsigned int Degrees);
void Soft_Right_Degrees(unsigned int Degrees);
void  __irq IRQ_Eint0(void);
void  __irq IRQ_Eint3(void);
void Ext_INT_Setup(void);



/*************Global Variables*************/

volatile unsigned int Left_Shaft_Count=0;
volatile unsigned int Right_Shaft_Count=0;
/******************************************/

//Function used for turning robot by specified degrees
void Angle_Rotate(unsigned int Degrees)
{
 float ReqdShaftCount = 0;
 unsigned int ReqdShaftCountInt = 0;

 ReqdShaftCount = (float) Degrees/ 4.090; // division by resolution to get shaft count
 ReqdShaftCountInt = (unsigned int) ReqdShaftCount;
 Left_Shaft_Count = 0; 
 Right_Shaft_Count = 0; 
 while (1)
 {
  if((Right_Shaft_Count >= ReqdShaftCountInt) | (Left_Shaft_Count >= ReqdShaftCountInt))
  break;
 }
 stop(); //Stop robot
}

//Function used for moving robot forward by specified distance
void Linear_Distance_mm(unsigned int DistanceInMM)
{
 float ReqdShaftCount = 0;
 unsigned int ReqdShaftCountInt = 0;

 ReqdShaftCount = DistanceInMM / 5.338; // division by resolution to get shaft count
 ReqdShaftCountInt = (unsigned int) ReqdShaftCount;
  
 Right_Shaft_Count = 0;
 while(1)
 {
  if(Right_Shaft_Count > ReqdShaftCountInt)
  {
  	break;
  }
 } 
 stop(); //Stop robot
}

void Forward_mm(unsigned int DistanceInMM)
{
 forward();
 Linear_Distance_mm(DistanceInMM);
}

void Back_mm(unsigned int DistanceInMM)
{
 backward();
 Linear_Distance_mm(DistanceInMM);
}

void Left_Degrees(unsigned int Degrees) 
{
// 88 pulses for 360 degrees rotation 4.090 degrees per count
 left(); //Turn left
 Angle_Rotate(Degrees);
}

void Right_Degrees(unsigned int Degrees)
{
// 88 pulses for 360 degrees rotation 4.090 degrees per count
 right(); //Turn right
 Angle_Rotate(Degrees);
}

void Soft_Left_Degrees(unsigned int Degrees)
{
 // 176 pulses for 360 degrees rotation 2.045 degrees per count
 soft_left(); //Turn soft left
 Degrees=Degrees*2;
 Angle_Rotate(Degrees);
}

void Soft_Right_Degrees(unsigned int Degrees)
{
 // 176 pulses for 360 degrees rotation 2.045 degrees per count
 soft_right();  //Turn soft right
 Degrees=Degrees*2;
 Angle_Rotate(Degrees);
}

void Ext_INT_Setup(void)
{
 //PINSEL0&= 0x3FFFFFFF;
 //PINSEL0|= 0x80000000; //Enabling P0.15 as EINT2
 PINSEL1&= 0xCFFFFFFC;
 PINSEL1|= 0x20000001; //Enabling P0.16 as EINT0  and P0.30 as EINT3


 EXTMODE = 0x09;	// EINT3 and EINT0 is edge sensitive
 EXTPOLAR = 0x00;					// EINT3 and EINT0 in triggered on falling edge

 VICIntSelect = 0x00000000;		// Setting EINT2 and EINt0 as IRQ(Vectored)
 VICVectCntl0 = 0x20|17;		// Assigning Highest Priority Slot to EINT3 and enabling this slot
 VICVectAddr0 = (int)IRQ_Eint3; // Storing vector address of EINT2

 VICVectCntl1 = 0x20|14;		// Assigning second Highest Priority Slot to EINT0 and enabling this slot
 VICVectAddr1 = (int)IRQ_Eint0; // Storing vector address of EINT0
 EXTINT = 0x09;	//Setting EINT2 & EINT0 interrupt flag
 	
 VICIntEnable = (1<<17) | (1<<14);	// Enable EINT3	& EINT0 flags
}


//ISR for EINT0
void  __irq IRQ_Eint0(void)
{  
   Right_Shaft_Count++;
   EXTINT = 0x01;				// Clear EINT0 flag
   VICVectAddr = 0x00;   		//Acknowledge Interrupt
}	


//ISR for EINT2
void  __irq IRQ_Eint3(void)
{  
   Left_Shaft_Count++;
   EXTINT = 0x08;				// Clear EINT2 flag
   VICVectAddr = 0x00;   		//Acknowledge Interrupt
}
