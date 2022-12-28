
#include "svmsg_file.h"

char clientMQ[CLIENT_MQ_NAME_LEN];
static void
removeQueue(void)
{
    if (mq_unlink(clientMQ) == -1)
        errExit("mq_unlink");
}

int
main(int argc, char *argv[])
{
    struct requestMsg req;
    struct responseMsg resp;
    int numMsgs;
    ssize_t msgLen, totBytes;
    
    int sendprio;
    mqd_t smqd,cmqd;
    unsigned prio;
    
    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s pathname\n", argv[0]);

    if (strlen(argv[1]) > sizeof(req.pathname) - 1)
        cmdLineErr("pathname too long (max: %ld bytes)\n",
                (long) sizeof(req.pathname) - 1);

    if (atexit(removeQueue) != 0)
        errExit("atexit");
    /* Get server's queue identifier; create queue for response */
    if(argc==3){
    	sendprio=atoi(argv[2]);
    }else{
    	sendprio=0;
    }
    
    snprintf(clientMQ, CLIENT_MQ_NAME_LEN, CLIENT_MQ_TMP, (long) getpid());
    printf("Request from:%s\n",clientMQ);
    cmqd=mq_open(clientMQ,O_RDONLY | O_CREAT,0666,NULL);
    if(cmqd==(mqd_t)-1){
    	errExit("client mqd_open");
    }
    smqd=mq_open(SERVER_MQ,O_RDWR);
    if(smqd==(mqd_t)-1){
    	errExit("server mqd_open");
    }
    
   
    /* Send message asking for file named in argv[1] */
     
    req.clientId = getpid();
    strncpy(req.pathname, argv[1], sizeof(req.pathname) - 1);
    req.pathname[sizeof(req.pathname) - 1] = '\0';
                                        /* Ensure string is terminated */

    if (mq_send(smqd, (char *)&req, sizeof(req), sendprio) == -1)
        errExit("msgsnd");

    /* Get first response, which may be failure notification */

    msgLen = mq_receive(cmqd, (char *)&resp, RESP_MSG_SIZE, &prio);
    if (msgLen == -1)
        errExit("msgrcv");

    if (resp.mtype == RESP_MT_FAILURE) {
        printf("%s\n", resp.data);      /* Display msg from server */
        exit(EXIT_FAILURE);
    }

    /* File was opened successfully by server; process messages
       (including the one already received) containing file data */

    totBytes = msgLen-sizeof(resp.mtype);                  /* Count first message */
    for (numMsgs = 1; resp.mtype == RESP_MT_DATA; numMsgs++) {
        msgLen = mq_receive(cmqd,(char *) &resp, RESP_MSG_SIZE,&prio);
        if (msgLen == -1)
            errExit("msgrcv");

        totBytes += msgLen-sizeof(resp.mtype);
    }

    printf("Received %ld bytes (%d messages)\n", (long) totBytes, numMsgs);


    exit(EXIT_SUCCESS);
}
