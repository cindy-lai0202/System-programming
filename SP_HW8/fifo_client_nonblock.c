#include "fifo_seqnum.h"

static char clientFifo[CLIENT_FIFO_NAME_LEN];
static void             /* Invoked on exit to delete client FIFO */
removeFifo(void)
{
    unlink(clientFifo);
}
int
main(int argc, char *argv[])
{
    int serverFd, clientFd;
    struct request req;
    struct response resp;

    if (argc > 1 && strcmp(argv[1], "--help") == 0)
        usageErr("%s [seq-len]\n", argv[0]);

    /* Create our FIFO (before sending request, to avoid a race) */

    umask(0);                   /* So we get the permissions we want */
    snprintf(clientFifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLATE,
            (long) getpid());
    if (mkfifo(clientFifo, S_IRUSR | S_IWUSR | S_IWGRP) == -1
                && errno != EEXIST)
        errExit("mkfifo %s", clientFifo);

    if (atexit(removeFifo) != 0)
        errExit("atexit");

    /* Construct request message, open server FIFO, and send message */
    for(int i=0;i<10;i++){
    clientFd = open(clientFifo, O_RDONLY|O_NONBLOCK);
    if (clientFd == -1)
        errExit("open %s", clientFifo);

    if (read(clientFd, &resp, sizeof(struct response))
            != sizeof(struct response))
        printf("time %d:Can't read response from server\n",i+1);
        
    printf("time %d:client %d receive the response from server.\n", i+1, getpid());
    
    
    req.pid = getpid();
    req.seqLen = (argc > 1) ? getInt(argv[1], GN_GT_0, "seq-len") : 1;

    serverFd = open(SERVER_FIFO, O_WRONLY);
    if (serverFd == -1)
        errExit("open %s", SERVER_FIFO);

    if (write(serverFd, &req, sizeof(struct request)) !=
            sizeof(struct request))
        fatal("Can't write to server");

    /* Open our FIFO, read and display response */

    printf("time %d:PID %d send the request to server.\n", i+1, req.pid);
        sleep(1);
    }

    exit(EXIT_SUCCESS);
}



