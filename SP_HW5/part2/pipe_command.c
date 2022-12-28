/* 
 * pipe_command.c  :  deal with pipes
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "shell.h"
#include <sys/wait.h>

#define STD_OUTPUT 1
#define STD_INPUT  0

void pipe_and_exec(char **myArgv) {
  	int pipe_argv_index = pipe_present(myArgv);
  	int pipefds[2];
	char **left_argv;
	char **right_argv;
	pid_t pid;
	int state;

  	switch (pipe_argv_index) {

    	case -1:	/* Pipe at beginning or at end of argv;  See pipe_present(). */
      		fputs ("Missing command next to pipe in commandline.\n", stderr);
      		errno = EINVAL;	/* Note this is NOT shell exit. */
      		break;

    	case 0:	/* No pipe found in argv array or at end of argv array.
			See pipe_present().  Exec with whole given argv array. */
  		if(execvp(myArgv[0],myArgv)<0){
			perror("execvp error\n");
			exit(1);
		}
      		break;

    	default:	/* Pipe in the middle of argv array.  See pipe_present(). */


      		/* Split arg vector into two where the pipe symbol was found.
       		 * Terminate first half of vector.
			 *
       		 * Fill in code. */
       		int i=0;
       		 int j=0;
       		 int k=0;
       		
       		 while(myArgv[i]!=NULL){
			i++;
		}
		/*printf("%d",pipe_argv_index);
		fflush(stdout);*/
       		left_argv = (char **) malloc(pipe_argv_index*sizeof(char*));
       		 for(j;j<pipe_argv_index;j++){
       		 	left_argv[j]=myArgv[j];
       		 }
       		 
       		 right_argv = (char **) malloc((i-pipe_argv_index-1)*sizeof(char*));
       		 for(j=pipe_argv_index+1;j<i;j++){
       		 	right_argv[k]=myArgv[j];
       		 	k++;
       		 }
      		/* Create a pipe to bridge the left and right halves of the vector. 
			 *
			 * Fill in code. */
		if (pipe(pipefds) == -1) {
			perror("pipe");
			exit(1);
		}
		
      		/* Create a new process for the right side of the pipe.
       		 * (The left side is the running "parent".)
       		 *
			 * Fill in code to replace the underline. */
      		switch(pid=fork()) {

        		case -1 :
	  				break;

        		/* Talking parent.  Remember this is a child forked from shell. */
        		default :
	  				/* - Redirect output of "parent" through the pipe.
	  				 * - Don't need read side of pipe open.  Write side dup'ed to stdout.
	 	 			 * - Exec the left command.
					 *
					 * Fill in code. */
					close(pipefds[0]);
					if (dup2(pipefds[1],1)==-1){
						perror("dup err\n");
						exit(1);
					}
					if(execvp(left_argv[0],left_argv)<0){
						perror("left execvp error\n");
						exit(1);
					}
					
	  				break;

        		//Listening child. 
        		case 0 :
 
	  				/* - Redirect input of "child" through pipe.
					  * - Don't need write side of pipe. Read side dup'ed to stdin.
				  	 * - Exec command on right side of pipe and recursively deal with other pipes
					 *
					 * Fill in code. */
				close(pipefds[1]);
				//printf("%d",pipefds[1]);
				//fflush(stdout);
				if (dup2(pipefds[0],0)==-1){
					perror("dup2 err\n");
					exit(1);
				}
				if(execvp(right_argv[0],right_argv)<0){
					perror("right execvp error\n");
					exit(1);
					}	 
          			pipe_and_exec(&myArgv[pipe_argv_index+1]);
          		
			}
			close(pipefds[0]);
			close(pipefds[1]);
			
	}
	free(left_argv);
	free(right_argv);
	perror("Couldn't fork or exec child process");
  	exit(errno);
}
