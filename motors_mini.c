//gcc foo.c -lwiringPi


#include <stdlib.h>
#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <string.h>
#include <unistd.h>

int enablePin = 7;		//GPIO4, pin enable engine 1
int m1 = 0;			//GPIO17, input 1 engine 1
int m2 = 3;			//GPIO22, input 2 engine 1
int enablePin2 = 6;		//GPIO25, pin enable engine 2
int m3 = 4;			//GPIO23, input 1 engine 2
int m4 = 5;			//GPIO24, input 2 engine 2
char s[20]="";
char* data;

int
main (int argc, char **argv)
{

	printf("Content-Length: 500\n");
        printf("Content-Type: text/plain\n");
        printf("Date: Mon, 13 Jun 2016 15:48:49 GMT\n");
        printf("\n");
	printf("Motor is staring...");
	
	data = getenv("QUERY_STRING");
	sscanf(data,"s=\%s",s);
	printf("\%s\n",s);

  //init wiringPi lib
  if (wiringPiSetup () == -1)
    {
      printf ("error!\n");
      return 1;
    }
  else
    {
      //init GPIO output engine 1
      pinMode (enablePin, OUTPUT);
      pinMode (m1, OUTPUT);
      pinMode (m2, OUTPUT);
      //init GPIO output engine 2
      pinMode (enablePin2, OUTPUT);
      pinMode (m3, OUTPUT);
      pinMode (m4, OUTPUT);
    }


//MOVE BACKWARD
if (!strcmp(s,"backward"))
{
  //start engine 1 forward
  digitalWrite (enablePin, 1);	//moteur ON: GPIOC=HIGH
  digitalWrite (m1, 1);		//GPIOA=HIGH
  digitalWrite (m2, 0);		//GPIOB=LOW

  //start engine 2 forward
  digitalWrite (enablePin2, 1);	//moteur ON: GPIOC=HIGH
  digitalWrite (m3, 1);		//GPIOA=HIGH
  digitalWrite (m4, 0);		//GPIOB=LOW

  //pause 1s
  printf ("engines 1 and 2, go backward!\n");
  sleep (1);

  //stop engine 1
  digitalWrite (enablePin, 0);

  //stop engine 2
  digitalWrite (enablePin2, 0);

  printf ("engines 1 and 2, stop!\n");

}


//MOVE FORWARD 
if (!strcmp(s,"forward"))
{
  //start engine 1 backward
  digitalWrite (enablePin, 1);	//start engine
  digitalWrite (m1, 0);		//GPIOA=LOW
  digitalWrite (m2, 1);		//GPIOB=HIGH

  //start engine 2 backward
  digitalWrite (enablePin2, 1);	//start engine
  digitalWrite (m3, 0);		//GPIOA=LOW
  digitalWrite (m4, 1);		//GPIOB=HIGH

  //pause 1s
  printf ("engines 1 and 2, go forward!\n");
  sleep (1);

  //stop engine 1
  digitalWrite (enablePin, 0);

  //stop engine 2
  digitalWrite (enablePin2, 0);

  printf ("engines 1 and 2, stop!\n");
}

//TURN LEFT
if (!strcmp(s,"turnleft"))
{
  //start engine 1 backward
  digitalWrite (enablePin, 1);  //start engine
  digitalWrite (m1, 0);         //GPIOA=LOW
  digitalWrite (m2, 1);         //GPIOB=HIGH

  //pause 1s
  printf("Engine 1, turns left\n");
  sleep (1);

  //stop engine 1
  digitalWrite (enablePin, 0);

}


//TURN RIGHT
if (!strcmp(s,"turnright"))
{

  //start engine 2 backward
  digitalWrite (enablePin2, 1); //start engine
  digitalWrite (m3, 0);         //GPIOA=LOW
  digitalWrite (m4, 1);         //GPIOB=HIGH

  //pause 1s
  printf("Engine 2, turns right\n");
  sleep (1);

  //stop engine 2
  digitalWrite (enablePin2, 0);

}

/*
  //stop engine 1
  digitalWrite (enablePin, 0);

  //stop engine 2
  digitalWrite (enablePin2, 0);

  printf ("engines 1 and 2, stop!\n");
*/

  return 0;
}
