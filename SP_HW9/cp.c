#include <sys/mman.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc,char *argv[]){
	int fd1,fd2;
	struct stat statbuf1,statbuf2;
	char *addr1,*addr2; 
	
	//open the file you want to copy
	if((fd1=open(argv[1],O_RDONLY))==-1){
		perror("open error");
		exit(1);
	}
	if(fstat(fd1,&statbuf1)==-1){
		perror("fstat error");
		exit(1);
	}
	
	addr1=mmap(NULL,statbuf1.st_size,PROT_READ,MAP_PRIVATE,fd1,0);
	if(addr1==MAP_FAILED){
		perror("mmap error");
		exit(1);
	}
	printf("file size:%ld\n",statbuf1.st_size);
	close(fd1);
	
	//open the file which to copy to
	if((fd2=open(argv[2],O_RDWR|O_CREAT|O_TRUNC,777))==-1){
		perror("open2 error");
		exit(1);
	}
	
	if(ftruncate(fd2,statbuf1.st_size)==-1){
		perror("ftruncate error");
		exit(1);
	}
	
	if(fstat(fd2,&statbuf2)==-1){
		perror("fstat2 error");
		exit(1);
	}
	printf("copyed file size:%ld\n",statbuf2.st_size);
	
	addr2=mmap(NULL,statbuf1.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd2,0);
	if(addr2==MAP_FAILED){
		perror("mmap2 error");
		exit(1);
	}
	
	memcpy(addr2,addr1,statbuf1.st_size);
	if(msync(addr2,statbuf1.st_size,MS_SYNC)==-1){
		perror("msync error");
		exit(1);
	}
	close(fd2);
	exit(0);
	
	

}
