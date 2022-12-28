#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
	int fd;
	int state;
	int pd[2];
	char path[100];
	pid_t pid;
	getcwd(path,sizeof(path));
	char *dir[]={ "ls","-l", path, NULL };
	
	if (pipe(pd) == -1) {
		perror("pipe");
		exit(1);
	}
	switch(pid=fork()){
		case -1:
			perror("fork error\n");
			break;
		case 0:
			close(pd[1]);
		
			if(pd[0]!=0){
				if(dup2(1,pd[0])==-1)
				{
					perror("dup error\n");
					exit(1);
				}
			}
			if(execvp("ls",dir)<0){
				perror("execvp error\n");
				exit(1);
			}
			
			break;
		case 1:	
			
			close(pd[0]);
			if(dup2(pd[1],1)==-1)
			{
				perror("dup error\n");
				exit(1);
			}
			break;
			
	}
	if(waitpid(pid,&state,0)==-1){
		perror("waitpid\n");
		exit(1);
	}
	close(pd[0]);
	close(pd[1]);
	return 0;
}
