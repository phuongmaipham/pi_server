#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int
main (int argc, char **argv)
{

        printf("Content-Length: 500\n");
        printf("Content-Type: text/plain\n");
        printf("Date: Mon, 13 Jun 2016 15:48:49 GMT\n");
        printf("\n");
        printf("Motor is staring...");

	system("tmp/PiBits/ServoBlaster/user/servod");
}
