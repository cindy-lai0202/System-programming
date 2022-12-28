#include<stdio.h>
#include<unistd.h>
#include<getopt.h>
#include<fcntl.h>
#include<stdlib.h>
#include <sys/types.h>
#define MAX_READ 100
int main(int argc,char *argv[])
{
	int opt,fd,flag;
	char buffer[MAX_READ+1];
	ssize_t numRead,numWrite,numWrite2;
	if(argc>=2)		
	{	
		flag=O_TRUNC | O_WRONLY | O_CREAT;
		while((opt=getopt(argc,argv,"a"))!=-1)
		{
			switch(opt)
			{
				case 'a':
					flag=O_APPEND |O_WRONLY | O_CREAT;
					break;
				case '?':
					printf("wrong command!");
					exit(EXIT_FAILURE);
				default:
					printf("wrong command!");
					exit(EXIT_FAILURE);
			}	
		}

		fd=open(argv[argc-1],flag,0644);
		if (fd == -1)
			printf("open error");
	
	}

	while((numRead = read(STDIN_FILENO, buffer, MAX_READ))!=0)
	{
		if (numRead == -1)
			printf("read error");
		
	
	
	numWrite=write(STDOUT_FILENO,buffer,numRead);
	if (numWrite == -1)
		printf("read error");

	if (argc>=2) 
	{
        numWrite2=write(fd,buffer,numRead);
        if (numWrite2 == -1)
		printf("write error");
    	}
    }
    close(fd);
	return 0;
}
