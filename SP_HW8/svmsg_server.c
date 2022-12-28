
#include "svmsg_seqnum.h"

int main(int argc, char *argv[])
{
    struct request req;
    struct response resp;
    int seqNum = 0;                     /* This is our "service" */
    int serverid;

    serverid=msgget(SERVER_KEY,IPC_CREAT | 0660);
    if(serverid==-1){
    	errExit("msgget");
    }
    resp.mtype = 1;
    for (;;) {    
                   /* Read requests and send responses */
        if (msgrcv(serverid, &req, sizeof(struct requestData),1,0)==-1) {
            errMsg("msgrcv");
            break;    
        }
	printf("Server recv Client %d request.\n", req.data.clientid);
        resp.seqNum = seqNum;
        if(msgsnd(req.data.clientid,&resp,sizeof(int),0)==-1){
            errMsg("msgsnd");
            break;
        }

        seqNum += req.data.seqLen;   
       printf("Server %d response Client %d.\n", getpid(), req.data.clientid);
       fflush(stdout);
               /* Update our sequence number */
    }
    
    if (msgctl(serverid, IPC_RMID, NULL) == -1) {
        errExit("msgctl");
    }

    exit(0);
}
