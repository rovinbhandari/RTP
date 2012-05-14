#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define	NPACKETS	5
#define	LENBUFFER	1024
#define	PORTSERVER	5000
#define IPSERVER	"127.0.0.1"

#define	er(e, x)					\
	{						\
		perror("ERROR: " #e "\n");		\
		exit(x);				\
	}	// better is to use a do-while loop (refer: http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Multi-statement_Macro)

int main(void)
{
	struct sockaddr_in sin_server;
	int size_sockaddr = sizeof(struct sockaddr), socket_fd, i;
	char data[LENBUFFER];

	if((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		er("SOCKET", 1)
	
	memset((char*) &sin_server, 0, sizeof(struct sockaddr_in));
	sin_server.sin_family = AF_INET;
	sin_server.sin_port = htons(PORTSERVER);
	
	if(!inet_aton(IPSERVER, &sin_server.sin_addr))
		er("inet_aton()", 2)

	printf("UDP Client started up.\nAttempting communication with server @ %s:%d...\n\n", IPSERVER, PORTSERVER);

	for(i = 1; i <= NPACKETS; i++)
	{
		sprintf(data, "data(%d/%d)\0", i, NPACKETS);
		
		printf("Sending packet %d of %d with data: \"%s\" ...\n", i, NPACKETS, data);

		if(sendto(socket_fd, data, strlen(data) + 1, 0, (struct sockaddr*) &sin_server, size_sockaddr) == -1)
			er("sendto()", 3)

		sleep(2);
	}

	close(socket_fd);

	return 0;
}

