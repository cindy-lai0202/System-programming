#include <signal.h>
#include "tlpi_hdr.h"

#define TESTSIG SIGUSR1

int main(int argc, char *argv[])
{

    siginfo_t info;

    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s num-sigs\n", argv[0]);

    int numSigs = getInt(argv[1], GN_GT_0, "num-sigs");
    
    

    /* Block the signal before fork(), so that the child doesn't manage
       to send it to the parent before the parent is ready to catch it */
       
       
       
    sigset_t blockedMask, emptyMask;
    sigemptyset(&blockedMask);
    sigaddset(&blockedMask, TESTSIG);
    if (sigprocmask(SIG_SETMASK, &blockedMask, NULL) == -1)
        errExit("sigprocmask");

    sigemptyset(&emptyMask);
    
    pid_t childPid=fork();
    switch (childPid) {
    case -1: errExit("fork");

    case 0:     /* child */
        for (int scnt = 0; scnt < numSigs; scnt++) {
            if (kill(getppid(), TESTSIG) == -1)
                errExit("kill");
            if (sigwaitinfo(&blockedMask, &info) == -1 && errno != EINTR)
                    errExit("sigsuspend");
        }
        exit(EXIT_SUCCESS);

    default: /* parent */
        for (int scnt = 0; scnt < numSigs; scnt++) {
            if (sigwaitinfo(&blockedMask, &info) == -1 && errno != EINTR)
                    errExit("sigsuspend");
            if (kill(childPid, TESTSIG) == -1)
                errExit("kill");
        }
        exit(EXIT_SUCCESS);
    }
}
