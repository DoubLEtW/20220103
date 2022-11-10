#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>
#include<sys/mman.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/fcntl.h>


#define THREAD_NO 10

char * ptr = NULL;
int blocksize;


int create_map(void)
{
	int fd = open("thread_copy.c",O_RDONLY);
	int Fsize = lseek(fd,0,SEEK_END);
	ptr = mmap(NULL,Fsize,PROT_READ,MAP_PRIVATE,fd,0);
	return Fsize;
}

void* thread_copy(void* arg)
{
	int i=*(int *)arg;
	int POS = i* blocksize;
	printf("Copy Thread 0x%x , i = %d, POS = %d , Blocksize = %d\n",(unsigned int)pthread_self(),i,POS,blocksize);
	char buf[blocksize];
	bzero(buf,sizeof(buf));
	snprintf(buf,blocksize+1,"%s",ptr+POS);
	//
	int dest_fd = open("1234.c",O_RDWR|O_CREAT,0664);
	lseek(dest_fd,POS,SEEK_SET);
	//
	write(dest_fd,buf,strlen(buf));
	close(dest_fd);
	pthread_exit(NULL);

}

int thread_cur(const char * File)
{
	int fd = open(File,O_RDONLY);
	int Fsize = lseek(fd , 0 , SEEK_END);

	if(Fsize % THREAD_NO == 0)
		return Fsize / THREAD_NO;
	else
		return Fsize / THREAD_NO + 1;
}

int thread_create(void)
{
	pthread_t tids[THREAD_NO];
	int i;
	for(i=0;i<THREAD_NO;i++)
	{
		pthread_create(&tids[i],NULL,thread_copy,(void *)&i);
		usleep(10000);
	}

	while(i--)
	{
		pthread_join(tids[i],NULL);
	}
	return 0;
}


int main()
{
	
	int Fsize;
	Fsize = create_map();
	blocksize = thread_cur("thread_copy.c");
	thread_create();
	munmap(ptr,Fsize);
	printf("Thread Copy succeed\n");
	return 0;
}
