#include <stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<ctype.h>
#include<stdio.h>
#include <string.h>

#define MAX 1024
#define BUFSIZE 1024

static FILE *_popen(const char *command, const char *type);
static int _pclose(FILE *f);

static void readout(FILE *f);
static void writein(FILE *f);
int fdpid[MAX];

int main(int argc,char *argv[]){
	
	int pd[2],pid;
	
	char *command=argv[2];
	

	for (int i = 0; i < MAX; i++) {
		fdpid[i] = 0;
	}
	
	if ((argc!=3)||(!(strcmp(argv[1],"r"))&&!(strcmp(argv[1],"w")))){
		printf("command error!");
		exit(1);
	}
	
	FILE *f = _popen(command, argv[1]);
	if (f == NULL){
		perror("_popen");
		exit(1);
	}
	
	
	if(strcmp(argv[1],"r")==0){
		readout(f);
		
	}else if(strcmp(argv[1],"w")==0){
		writein(f);
	}
	
	if (_pclose(f) == -1)
		perror("pclose");

	
	
	return 0;
}

static void writein(FILE *f) {
	char buf[BUFSIZE];
	int fd = fileno(f);
	if (fd == -1){
		perror("fileno");
		exit(1);
	}
	while (fgets(buf, BUFSIZE, stdin) != NULL) {
		if (buf[strlen(buf)-1] == '\n')
			buf[strlen(buf)-1] = '\0';

		if (write(fd, buf, strlen(buf)) == -1){
				perror("write");
				exit(1);
		}
	}
}


static void readout(FILE *f) {
	char buf[BUFSIZE];
	ssize_t numRead;

	int fd = fileno(f);
	if (fd == -1){
		perror("fileno");
		exit(1);
	}
	while ((numRead = read(fd, buf, BUFSIZE)) != 0) {
		if (write(STDOUT_FILENO, buf, numRead) != numRead){
				perror("write");
				exit(1);
		}
	}

	if (numRead == -1){
		perror("read");
		exit(1);
	}
}

static FILE * _popen(const char *command, const char *type) {
	long childPid; 
	int pd[2];    
	FILE *f;
	if (pipe(pd) == -1){
		perror("pipe");
		exit(1);
	}

	switch (childPid = fork()) {
		case -1:
			perror("fork");
			exit(1);

		case 0:
			if(strcmp(type,"r")==0){
				close(pd[0]);
				if(pd[1]!=STDOUT_FILENO){
					if(dup2(pd[1],STDOUT_FILENO)==-1){
						perror("dup");
						exit(1);
					}
				}
				close(pd[1]);
			}else if(strcmp(type,"w")==0){
				close(pd[1]);
				if(pd[0]!=STDIN_FILENO){
					if(dup2(pd[0],STDIN_FILENO)==-1){
						perror("dup");
						exit(1);
					}
				}
			}
			execlp("/bin/sh", "/bin/sh", "-c", command, (char *) 0);
			break;

	default:
		
		if(strcmp(type,"r")==0){
				close(pd[1]);
				if (pd[0] >= MAX) {
					printf("over 1024");
					exit(1);
				}
				fdpid[pd[0]]=childPid;
				f=fdopen(pd[0],"r");
			}else if(strcmp(type,"w")==0){
				close(pd[0]);
				if (pd[1] >= MAX) {
					printf("over 1024");
					exit(1);
				}
				fdpid[pd[1]]=childPid;
				f=fdopen(pd[1],"w");				
			}
		return f;
	
	}


}



static int _pclose(FILE *f) {
	int fd; 
	long childPid; 
	int stat;
	
	if ((fd = fileno(f)) == -1)
		return -1;

	if ((childPid = fdpid[fd]) == 0) {
		perror("no pid");
		exit(1);
	}

	if (waitpid(childPid, &stat, 0) == -1){
		printf("%d",WEXITSTATUS(stat));
		exit(1);
	}

	return WEXITSTATUS(stat);
}

