#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define	LENBUFFER	1024
#define	PORTSERVER	5000

#define	er(e, x)					\
	{						\
		perror("ERROR: " #e "\n");		\
		exit(x);				\
	}	// better is to use a do-while loop (refer: http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Multi-statement_Macro)

int main(void)
{
	struct sockaddr_in sin_server, sin_client;
	int socket_fd, size_sockaddr = sizeof(struct sockaddr);
	char data[LENBUFFER];

	if((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		er("SOCKET", 1)
	
	memset((char*) &sin_server, 0, sizeof(struct sockaddr_in));
	sin_server.sin_family = AF_INET;
	sin_server.sin_port = htons(PORTSERVER);
	sin_server.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(socket_fd, (struct sockaddr*) &sin_server, size_sockaddr) == -1)
		er("bind()", 2)
	
	printf("UDP Server started up @ local:%d.\nWaiting for client(s)...\n\n", PORTSERVER);

	while(1)
	{
		if(recvfrom(socket_fd, data, LENBUFFER, 0, (struct sockaddr*) &sin_client, &size_sockaddr) == -1)
			er("recvfrom()", 3)

		printf("Packet received from %s:%d;\ndata: %s\n\n", inet_ntoa(sin_client.sin_addr), ntohs(sin_client.sin_port), data);
		fflush(stdout);
	}

	close(socket_fd);

	return 0;
}

