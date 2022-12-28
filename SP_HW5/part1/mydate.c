#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
	time_t rawtime;
	struct tm*timeinfo;
	char buf[128];
	time(&rawtime);
	timeinfo=localtime(&rawtime);
	strftime(buf,sizeof(buf),"%b %d(%a), %Y %I:%M %p",timeinfo);
	printf("%s\n",buf);
	return 0;
}
