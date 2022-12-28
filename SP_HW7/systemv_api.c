#define _XOPEN_SOURCE 700

#include <limits.h>
#include <linux/limits.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { SIGSET = 1, SIGHOLD = 2, SIGRELSE = 3, SIGIGNORE = 4, SIGPAUSE = 5 };
typedef void (*sighandler_t)(int);
static void pexit(const char *error);
static void handler(int sig);
static sighandler_t _sigset(int sig, sighandler_t handler);
static int _sighold(int sig);
static int _sigrelse(int sig);
static int _sigignore(int sig);
static int _sigpause(int sig);

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("1:sigset   - Set SIGSET\n");
  		printf("2:sighold   - Blocks SIGINT\n");
  		printf("3:sigrelse  - Blocks SIGINT and then removes it from the process procmask\n");
  		printf("4:sigignore - Ignores SIGINT\n");
  		printf("5:sigpause  - suspends execution until SIGINT is received\n");
  		exit(0);
 	}

  	char *endptr;
  	long int number;
  	struct sigaction action;

	number = strtol(argv[1], &endptr, 10);
  	if (endptr == argv[1] || number < 1 || number > 5) {
    		printf("invalid number\n");
    		exit(1);
  	}

	switch (number) {
    		case SIGSET:
      			if (_sigset(SIGINT, handler) == (sighandler_t) -1) {
        			pexit("_sigset");
      			}

      			printf("Changed disposition of SIGSET. Type Ctrl-C to see its handler.\n");
      			pause();
      			break;
      			
    		case SIGHOLD:
      			if (_sighold(SIGINT) == -1) {
        			pexit("_sighold");
      			}

      			printf("SIGINT is blocked. Ctrl-C will not stop this process.\n");
      			pause();
      			break;

    		case SIGRELSE:
      			if (_sighold(SIGINT) == -1) {
        			pexit("_sighold");
      			}
     			 printf("SIGINT blocked\n");

      			if (_sigrelse(SIGINT) == -1) {
       				pexit("_sigrelse");
      			}
      			printf("SIGINT unblocked. Finish this process with Ctrl-C\n");
      			pause();
      			break;

    		case SIGIGNORE:
      			if (_sigignore(SIGINT) == -1) {
        			pexit("_sigignore");
      			}
      			printf("SIGINT is ignored. Ctrl-C will not stop this process\n");
      			pause();
      			break;

    		case SIGPAUSE:
      			action.sa_handler = handler;
      			if (sigaction(SIGINT, &action, NULL) == -1) {
        			pexit("sigaction");
      			}

      			printf("Suspending process until you type Ctrl-C\n");
      			_sigpause(SIGINT);
      			printf("Execution is back\n");
      			break;
  	}
	exit(0);
}

static sighandler_t _sigset(int sig, sighandler_t handler) {
	sigset_t blocked_signals;
  	struct sigaction act, oldact;

  	if (sigprocmask(SIG_BLOCK, NULL, &blocked_signals) == -1) {
    		return (sighandler_t) -1;
  	}

  	act.sa_handler = handler;
  	if (sigaction(sig, &act, &oldact) == -1) {
    		return (sighandler_t) -1;
  	}

  	if (handler == SIG_HOLD) {
    		_sighold(sig);
    		return oldact.sa_handler;
  	}

  	if (sigismember(&blocked_signals, sig)) {
    		return SIG_HOLD;
  	} else {
    		return oldact.sa_handler;
  	}
}

//block SIGINT
static int _sighold(int sig) {
  	sigset_t block;
  	sigemptyset(&block);
  	sigaddset(&block, sig);

  	return sigprocmask(SIG_BLOCK, &block, NULL);
	}
//unblock SIGINT
static int _sigrelse(int sig) {
  	sigset_t unblock;
  	sigemptyset(&unblock);
  	sigaddset(&unblock, sig);

  	return sigprocmask(SIG_UNBLOCK, &unblock, NULL);
	}

static int _sigignore(int sig) {
  	struct sigaction act;
  	act.sa_handler = SIG_IGN;
  	
  	return sigaction(sig, &act, NULL);
	}

static int _sigpause(int sig) {
  	sigset_t mask;
  	if (sigprocmask(SIG_SETMASK, NULL, &mask) == -1) {
    		pexit("sigprocmask");
  	}
  	sigdelset(&mask, sig);

  	return sigsuspend(&mask);
}

static void handler(__attribute__((unused)) int sig) {
  	printf("\tSIGINT received\n");
}

static void pexit(const char *error) {
  	perror(error);
  	exit(1);
}
