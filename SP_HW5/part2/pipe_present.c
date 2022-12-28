/*
 *  pipe_present.c :  check for |
 */

#include <stdio.h>
#include "shell.h"
#include <string.h>

/*
 * Return index offset into argv of where "|" is,
 * -1 if in an illegal position (first or last index in the array),
 * or 0 if not present.
 */
int pipe_present(char ** myCurrentArgv) {
	int index = 0;
	int ret=0;
	int i=0;
  	/* Search through myCurrentArgv for a match on "|". */
  	while(myCurrentArgv[i]!=NULL){
		i++;
	}
	for(index=0;index<i;index++){
  		if(strcmp("|",myCurrentArgv[index])==0){
  			ret=1;
  			break;
  		}
  	}

  	if((ret==1)&&((index==0)||(index==(i-1)))) /* At the beginning or at the end. */ {
    		return -1;
  	} else if (ret==0)/* Off the end. */ {
  
    		return 0;
  	} else {
    	/* In the middle. */
    		return index;
  	}
  
}
