/*
 * parse.c : use whitespace to tokenise a line
 * Initialise a vector big enough
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "shell.h"

/* Parse a commandline string into an argv array. */
char ** parse(char *line) {
  	static char delim[] = " \t\n"; /* SPACE or TAB or NL */
  	int count = 0;
  	char * token;
  	char **newArgv;
  	/* Nothing entered. */
  	if (line == NULL) {
    	return NULL;
  	}

  	/* Init strtok with commandline, then get first token.
     * Return NULL if no tokens in line. */
    	
	token = strtok(line, delim);
	if (token == NULL) {
		return NULL;
	}
	
	count=count+1;
  	/* Create array with room for first token.*/
	newArgv = (char **) malloc(count*sizeof(char*));
	if (newArgv == NULL) {
        printf("malloc failed\n");
	}
	newArgv[count-1]=token;
	
	
	token=strdup(token);
	if(token==NULL){
	printf("strdup failed\n");
	}
	
	newArgv[count-1]=token;
	printf("[%d] : %s\n",count-1,newArgv[count-1]);
  	/* While there are more tokens...
	 *	- Resize array.
	 *  - Give token its own memory, then install it.
	 */
	while(token){
		
		token = strtok(NULL, delim);
		if (token == NULL) {
			break;
		}
		
		count=count+1;

		token=strdup(token);
		if(token==NULL){
			printf("strdup failed\n");
		}
		
		char **new_newArgv =(char **) realloc(newArgv,(count+1)*sizeof(char*));
		if(new_newArgv == NULL){  
			printf("realloc failed\n");
		}
		newArgv = new_newArgv;
		newArgv[count-1] = token;
		printf("[%d] : %s\n",count-1,newArgv[count-1]);
		
	 }
	
  	/* Null terminate the array and return it.*/
	newArgv[count]=0;
  	return newArgv;
}

/*
 * Free memory associated with argv array passed in.
 * Argv array is assumed created with parse() above.
 */
void free_argv(char **oldArgv) {
	int i = 0;
	/* Free each string hanging off the array.
	 * Free the oldArgv array itself. */
	for (i=0; i < argv_count; i++) {
		free(oldArgv[i]);
	}
	free(oldArgv);
}
