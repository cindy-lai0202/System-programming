/*
 * run_command.c :    do the fork, exec stuff, call other functions
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include <unistd.h>
#include<string.h>
#include "shell.h"

void run_command(char **myArgv) {
    pid_t pid;
 
    int stat;
    int background=0;
  background=is_background(myArgv);
  
  /* Create a new child process.
     * Fill in code.
	 */
	pid=fork();
    switch (pid) {

        /* Error. */
        case -1 :
            perror("fork");
            exit(errno);

        /* Parent. */
        default :
            /* Wait for child to terminate.
             * Fill in code.
			 */
		if(background==1){
			break;
		}else{
		if(waitpid(pid,&stat,0)==-1){
			printf("waitpid error\n");
			exit(1);
		}
		}
            /* Optional: display exit status.  (See wstat(5).)
             * Fill in code.
			 */
		if(WIFEXITED(stat)){
			printf("terminated by sigal %d. \n",WEXITSTATUS(stat));
		}
            return;

        /* Child. */
        case 0 :
            /* Run command in child process.
             * Fill in code.
			 */
		int count=0;
		int ret,str;
		int i=0;
		while(myArgv[i]!=NULL){
			i++;
		}
		if(background==1){
			char **newargv = (char **) malloc(sizeof(char*));
			for(int j=0;j<i;j++){
				str=strcmp(myArgv[j],"&");
				if(str!=0){
					count++;
					char **new_newargv =(char **) realloc(newargv,(count*sizeof(char*)));
					if(new_newargv == NULL){  
						printf("realloc failed\n");
					}
					newargv = new_newargv;
					newargv[count-1]=myArgv[j];
				}
		
		
			}
			printf("%d",count);
			ret = execvp(newargv[0],newargv);
			if(ret == -1)
			printf("execvp error\n");
			}else{
		ret = execvp(myArgv[0],myArgv);
		if(ret == -1)
			printf("execvp error\n");}
		return ;
            /* Handle error return from exec */
			exit(errno);
    }
    
}
