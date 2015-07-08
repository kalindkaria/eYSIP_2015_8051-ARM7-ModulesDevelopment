#include<intrins.h>
#include"p89v51rx2.h"
sbit en=P0^0;
void delay_ms(unsigned int ms)
{
unsigned int i,j;
for(i=0; i<ms; i++)
for(j=0; j<53; j++);
}
void main()
{
while(1)
{
   en=1;
   delay_ms(1000);
   en=0;
   delay_ms(1000);
}
}