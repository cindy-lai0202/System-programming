
#include "svmsg_seqnum.h"

static int clientid;
static void             /* Invoked on exit to delete client FIFO */
removeQueue(void)
{
    if (msgctl(clientid, IPC_RMID, NULL) == -1) {
        errExit("msgctl");
    }
}

int
main(int argc, char *argv[])
{
    int serverid;
    struct request req;
    struct response resp;

    if (argc > 1 && strcmp(argv[1], "--help") == 0)
        usageErr("%s [seq-len]\n", argv[0]);

    serverid=msgget(SERVER_KEY,S_IWUSR);
    if(serverid==-1){
    	errExit("msgget server");
    }
  
    clientid=msgget(IPC_PRIVATE,IPC_CREAT| 0660);
    if(clientid==-1){
    	errExit("msgget client");
    }
    
    if (atexit(removeQueue) != 0)
        errExit("atexit");

    req.mtype = 1;
    req.data.clientid = clientid;
     if (argc == 1) {
        req.data.seqLen = 1;
    } else {
        req.data.seqLen = getInt(argv[1], GN_NONNEG, "req.seqLen");
    }
  
    if(msgsnd(serverid,&req,sizeof(struct requestData),0)==-1){
    	errExit("msgsnd");
        
    }

    if (msgrcv(clientid, &resp, sizeof(int), 1, 0) == -1) {
        errExit("msgrcv");
    }

    printf("%d\n", resp.seqNum);
    fflush(stdout);
    exit(EXIT_SUCCESS);
}
