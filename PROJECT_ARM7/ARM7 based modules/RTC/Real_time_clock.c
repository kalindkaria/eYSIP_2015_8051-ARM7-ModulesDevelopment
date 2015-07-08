//Program to Configure RTC
 #include<lpc214x.h>

 #include"LCD.h"


unsigned char flag=0;


void rtc_int(void)__irq{

ILR = 0X01;

flag = 1;

VICVectAddr = 0X00000000;

}



void init_rtc(){

ILR = 0X01;

CCR  = 0X13;

CCR =  0X11;

CIIR = 0X01;

SEC = 0X00;

HOUR =0X00;

MIN = 0X00;

VICIntEnable = 0x00002000;

VICVectCntl0 = 0x0000002D;

VICVectAddr0 = (unsigned)rtc_int;

}



int main(void)
{
 PINSEL0 = 0x00000000;		// Enable GPIO on all pins
 PINSEL1 = 0x00000000;
 PINSEL2 = 0x00000000;
 DelaymSec(40);
 Init_LCD_Pin();
 LCD_Init();
 LCD_Clear();
 DelaymSec(20);

init_rtc();

while(1){

if(flag){

LCD_Command(0x80);

flag=0;

LCD_Data(HOUR/10 + '0');

LCD_Data(HOUR%10 + '0'); 

LCD_Data(':') ;

LCD_Data(MIN/10 + '0');

LCD_Data(MIN%10 + '0');

LCD_Data(':') ;

LCD_Data(SEC/10 + '0');

LCD_Data(SEC%10 + '0');

}

  }

}
