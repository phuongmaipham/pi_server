#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
        printf("Accept-Ranges: bytes\n");
        printf("Content-length: 2579330\n");
        printf("Content-type: image/jpeg\r\n");

        printf("\r\n");

        //printf("Content-type: text/plain \n\n");

        FILE * toto;
        char get_char;
        toto = fopen("/var/www/html/cam.jpg", "rb");

        //int titi = open("/var/www/html/cam.jpg", O_RDONLY);
        int titi = open("/var/www/html/test.jpg", O_RDONLY);
	if(toto ==  NULL) return -1;

        char buffer[2579330];
        bzero(buffer, sizeof(buffer));
        int i = read(titi, buffer, sizeof(buffer));
        //write(1, buffer, i+1);
        int j= 0;
        while(j !=i)
        {
                printf("%c",  buffer[j]);
                j++;
        }
        fclose(toto);

        return 1;
}

