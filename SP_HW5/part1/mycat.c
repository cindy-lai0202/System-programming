#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include<fcntl.h>
int main(int argc,char *argv[])
{
	int fd;
	ssize_t numRead;
	char buffer[1024];
	if(argc==2){
		fd=open(argv[1],O_RDONLY);
		if (fd == -1)
			perror("open error");
		
		while(numRead = read(fd, buffer, 1024)!=0)
		{
		if (numRead == -1)
			perror("read error");
		printf("%s", buffer);
		}
	}else{
		printf("Usage: mycat filename\n");
	}
	return 0;
}

