#include<intrins.h>
#include"p89v51rx2.h"
#include"lcd_display.h"
#include"adc.h"
#include"pwm.h"
#include"position_encoder.h"
#include"buzzer.h"

#define Sharp_sensor_threshold 60

unsigned int node_count;
unsigned int left_white_line=0;
unsigned int center_white_line=0;
unsigned int right_white_line=0;
unsigned char flag=0;

/***********Function Prototypes****************/
int on_node(void);

int on_node(void)
{
	if((ADC_conversion(6)>0x28&&ADC_conversion(5)>0x28)||(ADC_conversion(4)>0x28&&ADC_conversion(5)>0x28)||(ADC_conversion(6)>0x28&&ADC_conversion(5)>0x28&&ADC_conversion(4)>0x28))
	return 1;
	else
	return 0;	
}

int slot_used(void)
{
	if(ADC_conversion(2)>Sharp_sensor_threshold)
	return 1;
	else
	return 0;		
}
int slot_empty(void)
{
	if(ADC_conversion(2)<Sharp_sensor_threshold)
	return 1;
	else
	return 0;
}

void follow_black_line()
{
	while(1)
{
 	left_white_line=ADC_conversion(4); 
	center_white_line=ADC_conversion(5);
	right_white_line=ADC_conversion(6);
	
	lcd_print(1,1,left_white_line,3);
  	lcd_print(1,6,center_white_line,3);
  	lcd_print(1,11,right_white_line,3);
	lcd_print(2,1,flag,1);

	flag=0;


	  if(center_white_line>30)            //Monitor center black line
    {
      flag=1;
      forward();
      velocity(250,250); 
    }  

    if((left_white_line<30) && (flag==0) && (right_white_line>30))    //Monitor left black line
    {
	//	flag=1;
      forward();
      velocity(250,190);
    }

    if((right_white_line<30) && (flag==0) && (left_white_line>30) )      //Monitor right black line
    {
      forward();
      velocity(190,250);
    }

    if(left_white_line<30 && right_white_line<30 &&(flag==0))    // Random motion to again follow black line
    {
      forward();
      velocity(250,250);     
    }

    if((left_white_line>30 && center_white_line>30)| (right_white_line>30 && center_white_line>30))
    {
      stop();
	  delay_ms(500);
	  break;
	}
  }
}

void right_turn(void)
{
	move_forward(70);
	stop();
	delay_ms(500);
	right_degrees(45);
	while(1)
	{
	left_white_line=ADC_conversion(4); 
	center_white_line=ADC_conversion(5);
	right_white_line=ADC_conversion(6);
	
	lcd_print(1,1,left_white_line,3);
  	lcd_print(1,6,center_white_line,3);
  	lcd_print(1,11,right_white_line,3);
	lcd_print(2,1,flag,1);
	right();
		velocity(200,200);
	if(ADC_conversion(1)>30 && ADC_conversion(3)<30 && ADC_conversion(2)<30)
	 {
	 stop();
	 break;
	  }
	}
		  

 }
 void left_turn(void)
{
	move_forward(70);
	stop();
	delay_ms(500);
	left_degrees(45);

	while(1)
	{
	left_white_line=ADC_conversion(3); 
	center_white_line=ADC_conversion(1);
	right_white_line=ADC_conversion(2);
	
	lcd_print(1,1,left_white_line,3);
  	lcd_print(1,6,center_white_line,3);
  	lcd_print(1,11,right_white_line,3);
	lcd_print(2,1,flag,1);
	left();
	velocity(200,200);
	 if(ADC_conversion(1)>30 && ADC_conversion(3)<30 && ADC_conversion(2)<30)
	 {
	 stop();
	 break;
	} 
	}
}
int main(void)
{
	lcd_init();
	lcd_clear();
	pca_init();
	left_white_line=ADC_conversion(4); 
	center_white_line=ADC_conversion(5);
	right_white_line=ADC_conversion(6);
	
	lcd_print(1,1,left_white_line,3);
  	lcd_print(1,6,center_white_line,3);
  	lcd_print(1,11,right_white_line,3);
	lcd_print(2,1,ADC_conversion(2),3);
	delay_ms(1000);
follow_black_line();
	if(slot_used())
	{
		stop();
		buzzer_on();
		delay_ms(1000);
		buzzer_off();
		right_turn();
	}
	else
	left_turn(); 
	/*node_count=0;
	while(task_completed())
	{
	while(!on_node())
	{
		follow_black_line();
	}
	node_count++;
	stop();
	lcd_print(1,1,node_count,2);
	if(node_count==1)
	 {
	 	left_turn();
	 }
	if(node_count==2 || node_count==8 || node_count==9 || node_count==15)
	 {
	 	right_turn();
	 }
	else
	forward();
	} */ 
		
}
