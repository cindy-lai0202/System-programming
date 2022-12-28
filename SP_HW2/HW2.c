#include<stdio.h>
#include<unistd.h>
#include<getopt.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#define MAX_READ  5012

int main(int argc,char *argv[])
{
	int linenumber=10;
	int opt,fd;
	ssize_t numRead,printByte;
	int linecount,startline,offset,n=0;
	char buffer[MAX_READ+1];
	char *filename;
	char *buf1,*buf2;
	if(argc>=3)
	{
		while((opt=getopt(argc,argv,"n"))!=-1)
		{
			switch(opt)
			{	
				case 'n':
					if (argc==3)
						{filename=argv[2];
						linenumber = 10;}
					else if(argc==4)
						{filename=argv[3];
						linenumber=atoi(argv[2]);}
					break;
				case '?':
					if(optopt=='n')
						printf("requires an argument");
					else
						printf("wrong command!");
						exit(EXIT_FAILURE);
			}
		}
		
	}

	fd=open(filename,O_RDONLY);
	if (fd == -1)
		printf("open error");
		
	char *buf=buffer;
	while((numRead = read(fd, buffer, MAX_READ))>0)
	{
		if (numRead == -1)
			printf("read error");
		offset-= numRead;
		while ((buf1 = strchr(buf, '\n')) != NULL) 
		{
			linecount++;
			buf = buf1 + 1;
		}
	}
	
	char *buff=buffer;
	
	if(linecount>=linenumber)
	{
		n=linecount-linenumber;
		for (startline = 0; startline < n; startline++) 
		{
			buf2 = strchr(buff, '\n');
			if (buf2 == NULL)
				buff=NULL;
			else
				buff = buf2 + 1;
		}
		offset += ((unsigned long)buff- (unsigned long)&buffer[0]);
		lseek(fd, offset, SEEK_END);
		
		while((printByte = read(fd, buffer, MAX_READ)) >0) 
		{
			write(STDOUT_FILENO, buffer, printByte);
		}
	}
	else
	{
		lseek(fd, 0, SEEK_SET);
		while((printByte = read(fd, buffer, sizeof(buffer))) > 0) 
		{
			write(STDOUT_FILENO, buffer, printByte);
		}
	}
	return 0;
}

