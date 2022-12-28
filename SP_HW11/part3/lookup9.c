/*
 * lookup9 : does no looking up locally, but instead asks
 * a server for the answer. Communication is by Internet UDP Sockets
 * The name of the server is passed as resource. PORT is defined in dict.h
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

#include <netinet/in.h>
#include "dict.h"

int lookup(Dictrec * sought, const char * resource) {
	static int sockfd;
	static struct sockaddr_in server;
	struct hostent *host;
	static int first_time = 1;

	if (first_time) {  /* Set up server address & create local UDP socket */
		first_time = 0;

		/* Set up destination address.
		 * Fill in code. */
		sockfd = socket(AF_INET, SOCK_DGRAM, 0);
		if (sockfd == -1) {
			perror("socket client");
			exit(1);
		}

		/* Allocate a socket.
		 * Fill in code. */
		memset(&server, 0, sizeof(struct sockaddr_in));
		server.sin_family = AF_INET;
		host = gethostbyname(resource);
		server.sin_port = htons(PORT);
		memcpy((char*)&server.sin_addr, host->h_addr_list[0], host->h_length);
	}

	/* Send a datagram & await reply
	 * Fill in code. */
	sendto(sockfd, &sought->word, WORD, 0, (struct sockaddr *)&server, sizeof(server));
	recvfrom(sockfd, &sought->text, TEXT, 0,(struct sockaddr *)&server, sizeof(server));
	
	if (strcmp(sought->text,"Not Found!") != 0) {
		return FOUND;
	}

	return NOTFOUND;
}
