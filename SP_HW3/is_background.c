/*
 * is_background.c :  check for & at end
 */

#include <stdio.h>
#include "shell.h"
#include<string.h>
#include<stdlib.h>

int is_background(char ** myArgv) {
	
	//newargv = (char **) malloc(sizeof(char*));
  	if (*myArgv == NULL)
    		return 0;

  	/* Look for "&" in myArgv, and process it.
  	 *
	 *	- Return TRUE if found.
	 *	- Return FALSE if not found.
	 *
	 * Fill in code.
	 */
	int ret;
	int i=0;
	int background=0;

	while(myArgv[i]!=NULL){
		i++;
	}
	for(int j=0;j<i;j++){
		ret=strcmp(myArgv[j],"&");
		if(ret==0){
			background=1;	
		}
	}

	return background;
}
