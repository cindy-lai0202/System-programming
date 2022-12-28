#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/utsname.h>

int main(){

	struct utsname uts;
	if (uname(&uts)==-1){
		perror("uname error");
		exit(1);
	}
	printf("hostname: %s\n",uts.nodename);
	printf("%s\n",uts.release);
	
	uid_t hostid=gethostid();
	printf("hostid: %d\n",hostid);
	
	return 0;
}

