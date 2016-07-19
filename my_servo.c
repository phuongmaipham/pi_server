#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
	
char s[20]="";
long i,j;
char* data;
char buffer [50]="";

int
main (int argc, char **argv)
{

        printf("Content-Length: 500\n");
        printf("Content-Type: text/plain\n");
        printf("Date: Mon, 13 Jun 2016 15:48:49 GMT\n");
        printf("\n");
        printf("Motor is staring...");

        data = getenv("QUERY_STRING");
        sscanf(data,"a=\%ld&b=\%ld",&i,&j);
        printf("a=\%ld, b=\%ld\n",i,j);

	system("tmp/PiBits/ServoBlaster/user/servod");
/*	
        data = getenv("QUERY_STRING");
        sscanf(data,"a=\%ld&b=\%ld",i,j);
        printf("\%ld,\%ld\n",i,j);
*/	
	sprintf (buffer, "echo %ld=%ld% > /dev/servoblaster", i, j);
	system(buffer);
//	system("echo 3=100% > /dev/servoblaster");
	

}
