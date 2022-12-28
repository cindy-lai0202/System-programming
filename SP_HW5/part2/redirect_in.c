/*
 * redirect_in.c  :  check for <
 */

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "shell.h"
#define STD_OUTPUT 1
#define STD_INPUT  0
extern int errno ;
/*
 * Look for "<" in myArgv, then redirect input to the file.
 * Returns 0 on success, sets errno and returns -1 on error.
 */
int redirect_in(char ** myArgv) {
  	int i = 0;
  	int j = 0;
  	errno=0;
  	int fd;
	//int count=0;
  	/* search forward for <
  	 *
	 * Fill in code. */
	 while(myArgv[j]!=NULL){
		j++;
	}
	for(i=0;i<j;i++){
		if (strcmp(myArgv[i],"<")==0) 
			break;
	}
	
  	if (myArgv[i]!=NULL) {	/* found "<" in vector. */

    	/* 1) Open file.
     	 * 2) Redirect stdin to use file for input.
   		 * 3) Cleanup / close unneeded file descriptors.
   		 * 4) Remove the "<" and the filename from myArgv.
		 *
   		 * Fill in code. */
   		
   		fd=open(myArgv[i+1],O_RDONLY);
   		if (fd == -1){
			printf("errno=%s\n",strerror(errno));
			return -1;
		}
			
		if (dup2(fd,0)==-1){
			printf("errno=%s\n",strerror(errno));
			return -1;
		}
		
		close(fd);
		free(myArgv[i]);
		free(myArgv[i+1]);
		myArgv[i]=NULL;
		myArgv[i+1]=NULL;
		j=j-2;
  	}

  	return 0;
}
