#include <signal.h>
#include "fifo_seqnum.h"

int main(int argc, char *argv[])
{
    int serverFd, dummyFd, clientFd;
    char clientFifo[CLIENT_FIFO_NAME_LEN];
    struct request req;
    struct response resp;
    int seqNum = 0;   
  
     /* This is our "service" */
    /* Create well-known FIFO, and open it for reading */
    umask(0);                           /* So we get the permissions we want */
    if (mkfifo(SERVER_FIFO, S_IRUSR | S_IWUSR | S_IWGRP) == -1 && errno != EEXIST)
        errExit("mkfifo %s", SERVER_FIFO);
        
    //open fifo for reading    
    serverFd = open(SERVER_FIFO, O_RDONLY);
    if (serverFd == -1)
        errExit("open %s", SERVER_FIFO);

    /* Open an extra write descriptor, so that we never see EOF */
    dummyFd = open(SERVER_FIFO, O_WRONLY|O_NONBLOCK);
    if (dummyFd == -1)
        errExit("open %s", SERVER_FIFO);
        
    //first time write fail:recive RTS;second time write fail:recive SIGPIPE and terminate
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)    
    	errExit("signal");

    for (;;) {
           /* Read requests and send responses */
        for(int i=0;i<2000;i++){
        	if (read(serverFd, &req, sizeof(struct request))!= sizeof(struct request)) {
            		fprintf(stderr, "Error reading request; discarding\n");
           		continue;                   /* Either partial read or error */
        	}
        	break;
        }

        /* Open client FIFO (previously created by client) */
        snprintf(clientFifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLATE,(long) req.pid);
       
        //O_NONBLOCK
        for(int i=0;i<2000;i++){
        	clientFd = open(clientFifo, O_WRONLY|O_NONBLOCK);
        	if (clientFd == -1) {           /* Open failed, give up on client */
           		errMsg("open %s", clientFifo);
            		continue;
        	}
        	printf("Server opened Client %d request.\n", req.pid);
        	break;
	}

        /* Send response and close FIFO */
        resp.seqNum = seqNum;
        if (write(clientFd, &resp, sizeof(struct response))!= sizeof(struct response)){
            fprintf(stderr, "Error writing to FIFO %s\n", clientFifo);
          
         }
        printf("Server %d response Client %d.\n", getpid(), req.pid);
        if (close(clientFd) == -1){
            errMsg("close"); 
        }

        seqNum += req.seqLen;           /* Update our sequence number */

    }
    
    return 0;
}


