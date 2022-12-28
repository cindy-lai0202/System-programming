/*
 * shell.c  : test harness for parse routine
 */

#define LONGLINE 255
#include<stdlib.h>
#include <stdio.h>
#include "shell.h"

int argv_count=0;

int main() {
    char line[LONGLINE];
    int i;
    char **myArgv;

    fputs("myshell -> ", stdout);
    while (fgets(line, LONGLINE, stdin)) {

        /* Create argv array based on commandline. */
        if (myArgv = parse(line)) {

           
            if (is_builtin(myArgv[0])) { /* If command is recognized as a builtin, do it. */
                do_builtin(myArgv);         
            } else {					/* Non-builtin command. */
	           run_command(myArgv);
	
            }

            free_argv(myArgv);			/* Free argv array. */
        }

        fputs("myshell -> ", stdout);
    }
    exit(0);
}
