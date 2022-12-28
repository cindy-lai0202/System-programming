#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#define MAX_READ 1024

int main(int argc,char *argv[])
{
	int fd,fd2;
	ssize_t numRead;
	int hole=0;
	char buffer[MAX_READ+1];
	if(argc=3)
	{
		fd=open(argv[1],O_RDONLY);
		if (fd == -1)
			printf("open source error");
		
		fd2=open(argv[2],O_CREAT | O_WRONLY | O_TRUNC,0644);
		if (fd2 == -1)
			printf("open dest error");
		
		while((numRead = read(fd, buffer, MAX_READ))!=0)
		{
			if (numRead == -1)
				printf("read error");
			
			for(int i=0;i<numRead;i++)
			{
				if(buffer[i]=='\0')
					hole=hole+1;
				if(hole>0)
				{
					lseek(fd2,hole,SEEK_CUR);
					//write(fd2,&buffer[i],1);
					hole = 0;
				}
				else
					write(fd2,&buffer[i],1);
			}
		}
	close(fd);
	close(fd2);	
	}
return 0;	
}
