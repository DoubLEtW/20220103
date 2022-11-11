#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>

int main(void)
{
	const char * icon = "|/-\\";
	char progress[100];
	bzero(progress,sizeof(progress));
	double percent = 0;

	while(1){
		if(percent > 100)
		{
			break;
		}
		progress[(int)percent] = '>';
		printf("[%-100s][%.2f%%]%c\r",progress,percent++,icon[(int)percent % 4]);
		fflush(stdout);
		usleep(20000);
	
	}
	printf("\n");
	return 0;
}
