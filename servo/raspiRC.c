/*
gcc foo.c -o foo -lwiringPi -lpthread
sleep( 1 );
delay( 1000 );
usleep( 1000000 );
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>		//sleep()
#include <wiringPi.h>
#include <softPwm.h>

#define DEVICE "/dev/input/js0"

#define NBBUTTONS 17
#define TRIANGLE 0
#define CIRCLE 1
#define CROSS 2
#define SQUARE 3
#define TRIGGERLEFT1 4
#define TRIGGERRIGHT1 5
#define TRIGGERLEFT2 6
#define TRIGGERRIGHT2 7
#define SELECT 8
#define START 9
#define STICKLEFT 10
#define STICKRIGHT 11
#define UP 12
#define RIGHT 13
#define DOWN 14
#define LEFT 15
#define P3 16


char commands[NBBUTTONS][17] = { { "TRIANGLE" }, { "CIRCLE" }, { "CROSS" }, { "SQUARE" }, { "TRIGGERLEFT1" }, { "TRIGGERRIGHT1" }, { "TRIGGERLEFT2" }, { "TRIGGERRIGHT2" }, { "SELECT" }, { "START" }, { "STICKLEFT" }, { "STICKRIGHT" }, { "UP" }, { "RIGHT" }, { "DOWN" }, { "LEFT" }, { "P3" } };


int state1=0, state2=0;
int buttons[NBBUTTONS];
int loop=1;
int enablePin=7;		//GPIO4, broche enable du moteur 1
int m1=0;			//GPIO17, entree 1 du moteur 1
int m2=3;			//GPIO22, entree 2 du moteur 1
int enablePin2=6;		//GPIO25, broche enable du moteur 2
int m3=4;			//GPIO23, entree 1 du moteur 2
int m4=5;			//GPIO24, entree 2 du moteur 2


int setupGPIO() {
	if(wiringPiSetup()==-1)
  		return -1;

	//on definit les GPIO en sortie pour le moteur 1
	pinMode(enablePin,OUTPUT);
	pinMode(m1,OUTPUT);
	pinMode(m2,OUTPUT);

	//on definit les GPIO en sortie pour le moteur 2
	pinMode(enablePin2,OUTPUT);
	pinMode(m3,OUTPUT);
	pinMode(m4,OUTPUT);

	return 1;
}


void go1() {
	//le moteur 1 tourne dans le sens positif
  	digitalWrite(enablePin,1);//active moteur : GPIOC=HIGH
  	digitalWrite(m1,1);		//GPIOA=HIGH
  	digitalWrite(m2,0);		//GPIOB=LOW
	state1 = 1;
}


void go2() {
	//le moteur 2 tourne dans le sens positif
  	digitalWrite(enablePin2,1);//active moteur : GPIOC=HIGH
  	digitalWrite(m3,1);		//GPIOA=HIGH
  	digitalWrite(m4,0);		//GPIOB=LOW
	state2 = 1;
}


void stop1() {
 	digitalWrite(enablePin,0);	//extinction du moteur 1
	state1 = 0;
}


void stop2() {
 	digitalWrite(enablePin2,0);	//extinction du moteur 2
	state2 = 0;
}


void back1() {
	//Le moteur 1 tourne dans l'autre sens
  	digitalWrite(enablePin,1);//activation du moteur
  	digitalWrite(m1,0);		//GPIOA=LOW
  	digitalWrite(m2,1);		//GPIOB=HIGH
	state1 = -1;
}


void back2() {
	//Le moteur 2 tourne dans l'autre sens
  	digitalWrite(enablePin2,1);//activation du moteur
  	digitalWrite(m3,0);		//GPIOA=LOW
  	digitalWrite(m4,1);		//GPIOB=HIGH
	state2 = -1;
}


void print_usage() {
	printf( "\nTRIANGLE to run both motors\n" );
	printf( "SQUARE to stop both motors\n" );
	printf( "CROSS to run reverse both motors\n" );
	printf( "UP to go on\n" );
	printf( "DOWN to go back\n" );
	printf( "LEFT to turn on the left\n" );
	printf( "RIGHT to turn on the right\n" );
	printf( "TRIGGERLEFT1 to run/stop motor 1\n" );
	printf( "TRIGGERLEFT2 to run reverse/stop motor 1\n" );
	printf( "TRIGGERRIGHT1 to run/stop motor 2\n" );
	printf( "TRIGGERRIGHT2 to run reverse/stop motor 2\n" );
	printf( "\nSELECT to quit the program\n" );
	printf( "P3 to halt the system\n" );
	printf( "\npress START to begin\n" );
}


void update_buttons() {
	int i;
	char temp;
	FILE *input;

	input = fopen( DEVICE, "rt" );
	for( i=0; i<8*NBBUTTONS; i++ ) {
		fscanf( input, "%c", &temp );
		if( i%8 == 4 ) 
			buttons[(i-4)/8] = (int)temp;
	}
	fclose( input );
}


void wait_start() {
	do {
		update_buttons();
	} while( buttons[START] != 1 );
}


void display_buttons() {
	int i;

	for( i=0; i<NBBUTTONS; i++ )
		printf( "%d ", buttons[i] );
	printf( "\n" );
/*
	int i=0;

	for( i=0; i<NBBUTTONS; i++ ) {
		if( buttons[i] == 1 ) {
			printf( "%s ", commands[i] );
			i = 1;
		}
	}
	if( i==1 )
		printf( "\n" );
*/
}


void handle_commands() {
	if( buttons[SELECT] ) loop = 0;
	if( buttons[P3] ) system( "sudo halt" );
	if( buttons[TRIANGLE] || buttons[UP] ) go1(), go2();
	if( buttons[SQUARE] ) stop1(), stop2();
	if( buttons[CROSS] || buttons[DOWN] ) back1(), back2();
	if( buttons[CIRCLE] );
	if( buttons[LEFT] ) system( "sudo echo 2=+9 > /dev/servoblaster\n" );
	if( buttons[RIGHT] ) system( "sudo echo 2=-9 > /dev/servoblaster\n" );
	if( buttons[UP] ) system( "sudo echo 3=-9 > /dev/servoblaster\n" );
	if( buttons[DOWN] ) system( "sudo echo 3=+9 > /dev/servoblaster\n" );
	if( buttons[TRIGGERLEFT1] )
		switch( state1 ) {
			case -1: go1(); break;
			case 0:	go1(); break;
			case 1:	stop1(); break;
			default: printf( "problem 1 1\n" );
		}
	if( buttons[TRIGGERLEFT2] )
		switch( state1 ) {
			case -1: stop1(); break;
			case 0:	back1(); break;
			case 1:	back1(); break;
			default: printf( "problem 1 2\n" );
		}
	if( buttons[TRIGGERRIGHT1] )
		switch( state2 ) {
			case -1: go2(); break;
			case 0:	go2(); break;
			case 1:	stop2(); break;
			default: printf( "problem 2 1\n" );
		}
	if( buttons[TRIGGERRIGHT2] )
		switch( state2 ) {
			case -1: stop2(); break;
			case 0:	back2(); break;
			case 1:	back2(); break;
			default: printf( "problem 2 2\n" );
		}
}


void rumble() {
	go1();
	back2();
	usleep( 20000 );
	stop1();
	stop2();
}


int main( int argc, char** argv ) {
	if(setupGPIO()==-1)
		return 0;

	print_usage();
	rumble();
	wait_start();
	rumble();
	printf( "go...\n" );

	while( loop ) {
		update_buttons();
		handle_commands();
		//display_buttons();
	}
	printf( "bye!\n" );

  	return 0;
}
