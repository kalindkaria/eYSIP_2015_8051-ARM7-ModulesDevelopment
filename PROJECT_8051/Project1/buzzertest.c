		 //Buzzer is connected to the 8th pin of Port 2 i.e P2.7
// To turn it on we have to set P2^7 to zero
#include <intrins.h>
#include "p89v51rx2.h"
sbit buzzer=P2^7;
// function for giving a delay of ms milliseconds
void delay_ms(unsigned int ms)
{
unsigned int i,j;
for(i=0; i<ms; i++)
for(j=0; j<53; j++);
}
// main begins here
void main()
{
while(1)
{
buzzer=0;// turn on the buzzer
delay_ms(1000); //give delay of 200 milliseconds
buzzer=1; // turn off the buzzer
delay_ms(1000);
}
}