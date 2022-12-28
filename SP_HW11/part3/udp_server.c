/*
 * udp_server : listen on a UDP socket ;reply immediately
 * argv[1] is the name of the local datafile
 * PORT is defined in dict.h
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>
#include<string.h>

#include "dict.h"

int main(int argc, char **argv) {
	static struct sockaddr_in server,client;
	int sockfd,siz;
	Dictrec dr, *tryit = &dr;
	ssize_t numBytes;

	if (argc != 2) {
		fprintf(stderr,"Usage : %s <datafile>\n",argv[0]);
		exit(errno);
	}

	/* Create a UDP socket.
	 * Fill in code. */
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd == -1) {
		perror("socket");
		exit(errno);
	}

	/* Initialize address.
	 * Fill in code. */
	memset(&server, 0, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT);

	/* Name and activate the socket.
	 * Fill in code. */
	if (bind(sockfd, (struct sockaddr *) &server, sizeof(server)) == -1) {
		perror("bind");
		exit(errno);
	}

	for (;;) { /* await client packet; respond immediately */

		siz = sizeof(client); /* siz must be non-zero */

		/* Wait for a request.
		 * Fill in code. */

		while (numBytes = recvfrom(sockfd, tryit, sizeof(tryit), 0, (struct sockaddr *) &client, &siz) > 0) {
			if (numBytes == -1) {
				perror("recvfrom");
				exit(errno);
			}
			/* Lookup request and respond to user. */
			switch(lookup(tryit,argv[1]) ) {
				case FOUND: 
					/* Send response.
					 * Fill in code. */
					if (sendto(sockfd, tryit->text, TEXT, 0, (struct sockaddr *) &client, siz) == -1) {
						perror("sendto");
						exit(1);
					}
					break;
				case NOTFOUND : 
					/* Send response.
					 * Fill in code. */
					strcpy(tryit->text, "Not Found!");
					if (sendto(sockfd, tryit->text, TEXT, 0, (struct sockaddr *) &client, siz) == -1) {
						perror("sendto");
						exit(1);
					}
					break;
				case UNAVAIL:
					DIE(argv[1]);
			} /* end lookup switch */
		} /* end while */
	} /* end forever loop */
} /* end main */
