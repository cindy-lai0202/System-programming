#include "svmsg_file.h"

static void             /* SIGCHLD handler */
grimReaper(int sig)
{
    int savedErrno;

    savedErrno = errno;                 /* waitpid() might change 'errno' */
    while (waitpid(-1, NULL, WNOHANG) > 0)
        continue;
    errno = savedErrno;
}

static void             /* Executed in child process: serve a single client */
serveRequest(const void *buffer)
{
    int fd;
    ssize_t numRead;
    struct requestMsg *req;
    struct responseMsg resp;
    mqd_t cmqd;
    char clientMQ[RESP_MSG_SIZE];
    
    req = (struct requestMsg *) buffer;
    printf("Received request from client: %d\n", req->clientId);
    printf("PATHNAME: %s\n", req->pathname);
    snprintf(clientMQ, CLIENT_MQ_NAME_LEN, CLIENT_MQ_TMP, (long)req->clientId); //snprintf(place,size,format,data)
    
    cmqd=mq_open(clientMQ,O_RDWR);
    if(cmqd==(mqd_t)-1){
    	errExit("client mqd_open");
    }
    
    fd = open(req->pathname, O_RDONLY);
    if (fd == -1) {                     /* Open failed: send error text */
        resp.mtype = RESP_MT_FAILURE;
        snprintf(resp.data, sizeof(resp.data), "%s", "Couldn't open");
        if(mq_send(cmqd,(char *)&resp,sizeof(resp),0)==-1)
        	errExit("mq_send");
    }

    /* Transmit file contents in messages with type RESP_MT_DATA. We don't
       diagnose read() and msgsnd() errors since we can't notify client. */

    resp.mtype = RESP_MT_DATA;
    while ((numRead = read(fd, resp.data, RESP_MSG_SIZE-((int)sizeof(resp.mtype)))) > 0){
        if (mq_send(cmqd, (char *)&resp, sizeof(resp.mtype) + numRead, 0) == -1)
            break;
    }
    /* Send a message of type RESP_MT_END to signify end-of-file */

    resp.mtype = RESP_MT_END;
    mq_send(cmqd, (char *)&resp, sizeof(resp.mtype), 0);         /* Zero-length mtext */
}

int
main(int argc, char *argv[])
{
    struct requestMsg req;
    pid_t pid;
    ssize_t msgLen;
    struct sigaction sa;
    
    void *buffer;
    unsigned prio;
    mqd_t smqd;
    struct mq_attr attr;

    /* Create server message queue */
    smqd=mq_open(SERVER_MQ, O_RDONLY|O_CREAT ,0666,NULL); //NULL default 
    if (smqd == (mqd_t)-1)
        errExit(" server mq_open");

    if(mq_getattr(smqd,&attr)==-1)
    	errExit("mq_getattr");
    	
    buffer=malloc(attr.mq_msgsize);
    if(buffer==NULL)
    	errExit("malloc");
    
    /* Establish SIGCHLD handler to reap terminated children */

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = grimReaper;
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
        errExit("sigaction");

    /* Read requests, handle each in a separate child process */

    for (;;) {
        msgLen=mq_receive(smqd,buffer,attr.mq_msgsize,&prio);
        if (msgLen == -1) {
            if (errno == EINTR)         /* Interrupted by SIGCHLD handler? */
                continue;               /* ... then restart msgrcv() */
            errMsg("mq_receive");           /* Some other error */
            break;                      /* ... so terminate loop */
        }


        pid = fork();                   /* Create child process */
        if (pid == -1) {
            errMsg("fork");
            break;
        }

        if (pid == 0) {                 /* Child handles request */
            serveRequest(buffer);
            _exit(EXIT_SUCCESS);
        }

        /* Parent loops to receive next client request */
    }

    if (mq_unlink(SERVER_MQ) == -1) {
    	errExit("mq_unlink");
     }

    exit(EXIT_SUCCESS);
}
