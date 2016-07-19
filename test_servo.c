//gcc test_servo.co -o test_servo


#define DELAY 500000


void absolute_position() {
	system( "sudo echo 2=50% > /dev/servoblaster" );
	system( "sudo echo 3=50% > /dev/servoblaster" );
	usleep(DELAY);

	system( "sudo echo 2=0% > /dev/servoblaster" );
	usleep(DELAY);

	system( "sudo echo 2=100% > /dev/servoblaster" );
	usleep(DELAY);

	system( "sudo echo 2=50% > /dev/servoblaster" );
	usleep(DELAY);

	system( "sudo echo 3=0% > /dev/servoblaster" );
	usleep(DELAY);

	system( "sudo echo 3=100% > /dev/servoblaster" );
	usleep(DELAY);

	system( "sudo echo 3=50% > /dev/servoblaster" );
	usleep(DELAY);
}


void relative_position() {
	int i, j;

	system( "sudo echo 2=0% > /dev/servoblaster" );
	for(i=0; i<200; i++)
		system( "sudo echo 2=+1 > /dev/servoblaster" );
	for(i=0; i<200; i++)
		system( "sudo echo 2=-1 > /dev/servoblaster" );
	system( "sudo echo 2=50% > /dev/servoblaster" );

	usleep(DELAY);

	system( "sudo echo 2=0% > /dev/servoblaster" );
	for(i=0; i<20; i++)
		system( "sudo echo 2=+10 > /dev/servoblaster" );
	for(i=0; i<20; i++)
		system( "sudo echo 2=-10 > /dev/servoblaster" );
	system( "sudo echo 2=50% > /dev/servoblaster" );

	usleep(DELAY);

	system( "sudo echo 3=0% > /dev/servoblaster" );
	for(i=0; i<20; i++)
		system( "sudo echo 3=+10 > /dev/servoblaster" );
	for(i=0; i<20; i++)
		system( "sudo echo 3=-10 > /dev/servoblaster" );
	system( "sudo echo 3=50% > /dev/servoblaster" );
}


void main() {
	//absolute_position();
	relative_position();
}
