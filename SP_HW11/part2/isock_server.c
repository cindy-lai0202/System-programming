/*
 * isock_server : listen on an internet socket ; fork ;
 *                child does lookup ; replies down same socket
 * argv[1] is the name of the local datafile
 * PORT is defined in dict.h
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>

#include "dict.h"

int main(int argc, char **argv) {
	static struct sockaddr_in server;
	int sd,cd,n;
	Dictrec tryit;
	ssize_t numRead;

	if (argc != 2) {
		fprintf(stderr,"Usage : %s <datafile>\n",argv[0]);
		exit(1);
	}

	/* Create the socket.
	 * Fill in code. */
	sd = socket(AF_INET, SOCK_STREAM, 0);
	if (sd == -1) {
		perror("socket");
		exit(errno);
	}

	/* Initialize address.
	 * Fill in code. */
	memset(&server, 0, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY; //same as 0.0.0.0
	server.sin_port = htons(PORT);

	/* Name and activate the socket.
	 * Fill in code. */
	if (bind(sd, (struct sockaddr *) &server, sizeof(server)) == -1) {
		perror("bind");
		exit(errno);
	}
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
				while ((numRead = recv(cd, &tryit, sizeof(tryit), 0)) > 0) {
					/* Lookup the word , handling the different cases appropriately */
					switch(lookup(&tryit,argv[1]) ) {
						/* Write response back to the client. */
						case FOUND:
							/* Fill in code. */
							if (send(cd, &tryit.text, TEXT, 0) <= 0 ){
								perror("server write");
								exit(errno);
							}
							break;
						case NOTFOUND:
							/* Fill in code. */
							strcpy(tryit.text, "Not Found!");
							if (send(cd, &tryit.text, TEXT, 0) <= 0 ){
								perror("server write");
								exit(errno);
							}
							 break;
						case UNAVAIL:
							DIE(argv[1]);
					} /* end lookup switch */
				} /* end of client dialog */
				exit(0); /* child does not want to be a parent */

			default :
				close(cd);
				break;
		} /* end fork switch */
	} /* end forever loop */
} /* end main */
