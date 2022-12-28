/*
 * shell.c  : test harness for parse routine
 */

#define LONGLINE 255

#include <stdio.h>
#include <stdlib.h>
#include "shell.h"
int argv_count=0;
int main(int argc, char* argv[]) {
	char line[LONGLINE];
  	char **myArgv;

  	fputs("myshell -> ",stdout);
  	
  	while (fgets(line,LONGLINE,stdin)) {
    	/* Create argv array based on commandline. */
    	if ((myArgv = parse(line))!= NULL) {
    	
      		/* If command is recognized as a builtin, do it. */
      		if (is_builtin(myArgv[0])) {
        		do_builtin(myArgv);
			/* Non-builtin command. */
			} else {
				run_command(myArgv);
			}
			/*test
			int a=0;
			while(myArgv[a]!=NULL){
			printf("%s",myArgv[a]);
			a++;	
			}
		 	fflush(stdout);*/
			// Free argv array. 
			free_argv(myArgv);
		}

    	fputs("myshell -> ",stdout);
	}
  	exit(0);
}
