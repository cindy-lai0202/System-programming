#include<sys/types.h>
#include<sys/mman.h>
#include <sys/stat.h>
#include<fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#define MSGSIZE 80
#define SHM_PATHNAME "/hw10"

int dataid = 0;

static void handlerUSR1(int sig, siginfo_t *siginfo, void *context)
{
	dataid = siginfo->si_value.sival_int;
}

static void handlerINT(int sig, siginfo_t *siginfo, void *context)
{
	dataid = -1;
}

int getdata(int bufferSize, int dataNum,char *address){
	sigset_t wait;
	char buf[MSGSIZE];
	int msgid;
	int errorNum = 0;
	int recvNum=0;
	
	for(int i = 0; i < dataNum; i++) {
	
		sigfillset(&wait);	//fully filled (11111..)
		sigdelset(&wait, SIGUSR1);
		sigdelset(&wait, SIGINT);

		if (dataid == -1) {
			break;
		}
			
		//wait for SIGUSR1 SIGINT signal
		if (sigsuspend(&wait) != -1) {
			perror("sigsuspend");
		}
		if (dataid == -1) {
			break;
		}
		memcpy(buf, address + MSGSIZE * (dataid % bufferSize), MSGSIZE);
		//don't need first  %s values in buf(message 0/1/2...),only need %d in buf and stored in &msgid
		sscanf(buf, "%*s %d", &msgid);
		if (msgid != dataid) {
			errorNum++;
			continue;
		}
		recvNum++;
	}
	return recvNum;
}


int addconsumer(int bufferSize, int dataNum){
	int shmfd;
	struct stat sb;
	char *address;
	struct sigaction act;
	struct sigaction act2;
	int recvNum=0;
	
	if ((shmfd = shm_open(SHM_PATHNAME, O_RDONLY,0)) == -1) {
		perror("shm_open child");
		exit(1);
	}
	
	if (fstat(shmfd, &sb) == -1) {
		perror("fstat");
		exit(1);
	}

	address = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, shmfd, 0);
	if (address == MAP_FAILED) {
		perror("mmap src");
		exit(1);
	}
	
	if (close(shmfd) == -1) { 
		perror("close");
		exit(1);
	}
	
	sigemptyset(&act.sa_mask);
	act.sa_sigaction = handlerUSR1; //void (*sa_sigaction) (int, siginfo_t *, void *); can carry message
					//sa_handler can't carry message
	act.sa_flags = SA_SIGINFO;

	sigemptyset(&act2.sa_mask);
	act2.sa_sigaction = handlerINT;
	act2.sa_flags = SA_SIGINFO;
	
	if (sigaction(SIGUSR1, &act, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}
	if (sigaction(SIGINT, &act2, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}
	
	recvNum=getdata(bufferSize,dataNum,address);
	return recvNum;
}




int main(int argc,char *argv[]){

	int dataNum,sendRate,consumerNum,bufferSize;
	int shmfd;
	struct stat sb;
	char message[MSGSIZE];
	char *address;
	pid_t pid;
	int pipefd[2];
	union sigval val;
	int child = 0, allchild = 0;
	int numRead;
	char buff[16];
	double loss=0.0;

	if(argc!=5){
		printf("./program dataNum sendRate consumerNum bufferSize");
		exit(1);
	}	
	dataNum=atoi(argv[1]);
	sendRate=atoi(argv[2]);
	consumerNum=atoi(argv[3]);
	bufferSize=atoi(argv[4]);
	pid_t pidlist[1500];
	
	if ((shmfd = shm_open(SHM_PATHNAME, O_RDWR | O_CREAT | O_TRUNC ,0666)) == -1) {
		perror("shm_open child");
		exit(1);
	}
	if (ftruncate(shmfd, sizeof(message)*bufferSize) == -1){ 
		perror("ftruncate");
		exit(1);
	}
	
	if (fstat(shmfd, &sb) == -1) {
		perror("fstat");
		exit(1);
	}

	address = mmap(NULL, sb.st_size, PROT_WRITE, MAP_SHARED, shmfd, 0);
	if (address == MAP_FAILED) {
		perror("mmap");
		exit(1);
	}
	
	if (close(shmfd) == -1) { 
		perror("close");
		exit(1);
	}
	
	if (pipe(pipefd) == -1) {
		perror("pipe");
		exit(1);
	}
	
	for(int i=0;i<consumerNum;i++){
		int recvNum=0;
		switch(pid=fork()){
			case -1:
				perror("fork");
				exit(1);
			case 0:
				recvNum=addconsumer(bufferSize, dataNum);
				sprintf(buff, "%d", recvNum); //int to char %d
				close(pipefd[0]);
				write(pipefd[1], buff, sizeof(buff));
				close(pipefd[1]);
				exit(0);
			default:
				pidlist[i]=pid;
		}

	}
	close(pipefd[1]);
	sleep(1);
	
	for (int i = 0; i < dataNum; i++) {
		snprintf(message, sizeof(message), "message %d", i);
		memcpy(address + sizeof(message)*(i % bufferSize), message, sizeof(message));
		//additional int in signal 
		val.sival_int = i;
		//send SIGUSR1
		for (int j = 0; j < consumerNum; j++) {
			if (sigqueue(pidlist[j], SIGUSR1, val) == -1) {
				perror("sigqueue");
			}
		}
		usleep(sendRate); //unit is ms(sleep() unit is s)
	}
	
	for (int i = 0; i < consumerNum; i++) {
		//send SIGINT
		if (sigqueue(pidlist[i], SIGINT, val) == -1) {
			perror("sigqueue");
			break;
		}
		//wait for child finish and read number of success recive
		if (waitpid(pidlist[i], NULL, 0) > 0) {
			numRead = read(pipefd[0], buff, sizeof(buff));
			if (numRead <= 0) {
				perror("numRead error");
				break;
			} else {
				sscanf(buff, "%d", &child);
				printf("%d: child %d receive %d\n",i+1, pidlist[i], child);
				allchild += child;
			}
		}
	}
	close(pipefd[0]);
	
	loss = 1-((double)allchild / (double)(dataNum*consumerNum));
	printf("M = %d\t R = %d ms\tN = %d\n", dataNum, sendRate, consumerNum);
	printf("------------------------------------\n");
	printf("Total message: %d (%d * %d)\n", dataNum*consumerNum, dataNum, consumerNum);
	printf("Sum of received message by all consumer: %d\n", allchild);
	printf("Loss rate: %.5f\n", loss*100);
	printf("------------------------------------\n");

	if (shm_unlink(SHM_PATHNAME) == -1) {
		perror("shm_unlink");
		exit(1);
	}

}
