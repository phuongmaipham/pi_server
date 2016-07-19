#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int main()
{
	printf("Accept-Ranges: bytes\n");
        printf("Content-length: 2579330\n");
        printf("Content-Type: text/plain\n");
        printf("Date: Mon, 13 Jun 2016 15:48:49 GMT\n");
        printf("\n");

	system("ps awux|grep 'apache' |awk {'print $2'}>pid");
	system("ps awux|grep 'raspivid' |awk {'print $2'}>pid");
	//int my_pid=0;
	char c;
	int i,j;
	int my_pid[10];
   	FILE *fp;

	fp = fopen("pid","r");
	if( fp == NULL )
  		{
      		perror("Error while opening the file.\n");
   		}
	for(i=0;i<11;i++)
       	{
        	my_pid[i] = 0;
        }
	i=0;
	while(c = fgetc(fp))
        {
		if( feof(fp) )
       	                {
                                break ;
                        }
		if(c =='\n')
		{
			i++;
		}
		else if(c !='\n')
		{
		//printf("%d\n",my_pid[1]);
                my_pid[i] = my_pid[i]*10+(c-'0');
		}
        }
	//printf("i: %d\n",i);
	
	for(j=0;j<i;j++)
        {
		printf("%d\n",my_pid[j]);
		kill(my_pid[j],9);
	}

/*
	while((c = fgetc(fp))!='\n')
	{
		printf("%d\n",my_pid);
		my_pid = my_pid*10+(c-'0');
	}
	printf("%d\n",my_pid);
*/

   	return 0;
}
