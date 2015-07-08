#include <intrins.h>
#include "p89v51rx2.h"
//The appropriate bits have been defined as per the logic table for motion control
sbit left_velocity=P1^3;
sbit right_velocity=P1^4;
sbit LB=P1^0;
sbit LF=P1^1;
sbit RF=P1^2;
sbit RB=P3^4;

/****************function prototypes*************************/
void forward(void);
void backward(void);
void right(void);
void left(void);
void stop(void);
void soft_right(void);
void soft_left(void);

//These are the direction functions to move the motors in various directions
/* Function name - forward
* Input - none 
* Output - moves the robot in forward direction
* Logic - set appropriate bits high to move forward as per the logic table
*/
void forward(void) //go forward
{
RF=1;
LF=1;
RB=0;
LB=0;
}
/* Function name - backward
* Input - none 
* Output - moves the robot in backward direction
* Logic - set appropriate bits high to move backward as per the logic table
*/
void backward(void) //go backward
{
RF=0;
LF=0;
RB=1;
LB=1;
}
/* Function name - left
* Input - none 
* Output - turns the robot in left direction
* Logic - set appropriate bits high to turn left as per the logic table
*/
void left(void) //go left ( hard left)
{
RF=1;
LF=0;
RB=0;
LB=1;
}
/* Function name - right
* Input - none 
* Output - turns the robot in right direction
* Logic - set appropriate bits high to turn right as per the logic table
*/
void right(void) //go right ( hard right)
{
RF=0;
LF=1;
RB=1;
LB=0;
}
/* Function name - stop
* Input - none 
* Output - stops the robot
* Logic - set appropriate bits high to stop all motors as per the logic table
*/
void stop(void) //stop ( soft stop )
{
RF= 0;
LF= 0;
RB= 0;
LB= 0;
}

void soft_right(void)
{
stop();
LF=1;
}

void soft_left(void)
{
stop();
RF=1;
}