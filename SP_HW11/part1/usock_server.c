/*
 * usock_server : listen on a Unix socket ; fork ;
 *                child does lookup ; replies down same socket
 * argv[1] is the name of the local datafile
 * PORT is defined in dict.h
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include "dict.h"

int main(int argc, char **argv) {
    struct sockaddr_un server;
    int sd,cd,n;
    Dictrec tryit;
    ssize_t numRead;

    if (argc != 3) {
      fprintf(stderr,"Usage : %s <dictionary source>"
          "<Socket name>\n",argv[0]);
      exit(errno);
    }

    /* Setup socket.
     * Fill in code. */
    sd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sd == -1) {
	perror("socket");
	exit(errno);
    }
    
    /* Initialize address.
     * Fill in code. */
    // remove刪除的給定文件名，以便它不再訪問 
    if (remove(argv[2]) == -1 && errno != ENOENT) {
	perror("remove");
	exit(errno);
    }
    //memset(&server, 0, sizeof(struct sockaddr_un));
    server.sun_family = AF_UNIX;
    strncpy(server.sun_path, argv[2], sizeof(server.sun_path) - 1);
    if (bind(sd, (struct sockaddr *) &server, sizeof(server)) == -1) {
	perror("bind");
	exit(errno);
    }

    /* Name and activate the socket.
     * Fill in code. */
     //5:can connect buffer numbers
    if (listen(sd, 5) == -1) {
		perror("listen");
		exit(errno);
	}
    /* main loop : accept connection; fork a child to have dialogue */
    for (;;) {
		/* Wait for a connection.
		 * Fill in code. */
		cd = accept(sd, NULL, NULL);
		if (cd == -1) {
			perror("accept");
			exit(errno);
		}
		/* Handle new client in a subprocess. */
		switch (fork()) {
			case -1 : 
				DIE("fork");
			case 0 :
				close (sd);	/* Rendezvous socket is for parent only. */
				/* Get next request.
				 * Fill in code. */
				memset(&tryit, 0, sizeof(tryit));
				while ((numRead = read(cd, &tryit, sizeof(tryit))) > 0) {

					/* Lookup request. */
					switch(lookup(&tryit,argv[1]) ) {
						/* Write response back to client. */
						case FOUND: 
							/* Fill in code. */
							if (write(cd, &tryit.text, sizeof(tryit.text)) <= 0 ){
								perror("server write");
								exit(errno);
							}
							break;
						case NOTFOUND: 
							/* Fill in code. */
							strcpy(tryit.text, "Not Found!\n");
							if (write(cd, &tryit.text, sizeof(tryit.text)) <= 0 ){
								perror("server write");
								exit(errno);
							}
							break;
						case UNAVAIL:
							DIE(argv[1]);
					} /* end lookup switch */

				} /* end of client dialog */

				/* Terminate child process.  It is done. */
				exit(0);

			/* Parent continues here. */
			default :
				close(cd);
				break;
		} /* end fork switch */
    } /* end forever loop */
} /* end main */
