/*
 * lookup7 : does no looking up locally, but instead asks
 * a server for the answer. Communication is by Unix TCP Sockets
 * The name of the socket is passed as resource.
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "dict.h"

int lookup(Dictrec * sought, const char * resource) {
	static int sockfd;
	static struct sockaddr_un server;
	static int first_time = 1;
	int n;

	if (first_time) {     /* connect to socket ; resource is socket name */
		first_time = 0;

		/* Set up destination address.
		 * Fill in code. */
		sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
		if (sockfd == -1) {
			perror("socket client");
			exit(1);
		}
		/* Allocate socket. */
		memset(&server, 0, sizeof(struct sockaddr_un));
		server.sun_family = AF_UNIX;
		strcpy(server.sun_path,resource);

		/* Connect to the server.
		 * Fill in code. */
		 if (connect(sockfd, (struct sockaddr *) &server, sizeof(struct sockaddr_un)) == -1) {
			perror("connect client");
			exit(1);
		}
	}

	/* write query on socket ; await reply
	 * Fill in code. */
	if (write(sockfd, &sought->word, WORD) <= 0) {
		perror("client write");
	}
	
	if ((read(sockfd, sought->text, TEXT)) <= 0) {
		perror("client read");
	}
	if (sought->text == NULL) {
		printf("receive NULL\n");
	}

	if (strcmp(sought->text,"Not Found!\n") !=0 ) {
		return FOUND;
	}

	return NOTFOUND;
}
