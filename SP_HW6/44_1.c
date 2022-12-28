#include <stdlib.h>
#include<sys/types.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<ctype.h>

#define BUFSIZE 1024
int main(int argc,char **argv){
	int pdin[2];
	int pdout[2];
	char buf[BUFSIZE];
	ssize_t readlen;
	if(pipe(pdin)==-1){
		perror("pipe in");
		exit(1);
	}
	if(pipe(pdout)==-1){
		perror("pipe out");
		exit(1);
	}
	
	switch(fork()){
		case -1:
			perror("fork");
			exit(1);
		
		case 0:
			close(pdin[1]);
			close(pdout[0]);
			/*//test
			printf("222");
			fflush(stdout);*/
			while((readlen=read(pdin[0],buf,BUFSIZE))>0){
				for (int i=0;i<readlen;i++){
					buf[i]=toupper(buf[i]);//unsigned char
				}
				if(write(pdout[1],buf,readlen)!=readlen){
					perror("write");
				}
			}
			if(readlen==-1){
				perror("read");
				exit(1);
			}
			exit(0);
		
		default:
			close(pdin[0]);
			close(pdout[1]);
			
			while((readlen=read(STDIN_FILENO,buf,BUFSIZE))>0){
			
				if(write(pdin[1],buf,readlen)!=readlen){
					perror("write");
				}
				readlen=read(pdout[0],buf,BUFSIZE);
				if(readlen==-1){
					perror("read");
					exit(1);
				}else if(readlen>0){
					if(write(STDOUT_FILENO,buf,readlen)!=readlen){
						perror("write");
					}
				}
			}
			
			if(readlen==-1){
				perror("read");
				exit(1);
			}
			wait(NULL);
			exit(0);
			
		
	}
}
