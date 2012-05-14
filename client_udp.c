#include "commons.h"

#define	NPACKETS	5
#define IPSERVER	"127.0.0.1"
#define	ID		"CLIENT=> "

int main(void)
{
	struct sockaddr_in sin_server;
	int size_sockaddr = sizeof(struct sockaddr), socket_fd, i;
	char data[LENBUFFER];

	if((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		er("socket()", 1);
	
	memset((char*) &sin_server, 0, sizeof(struct sockaddr_in));
	sin_server.sin_family = AF_INET;
	sin_server.sin_port = htons(PORTSERVER);
	
	if(!inet_aton(IPSERVER, &sin_server.sin_addr))
		er("inet_aton()", 2);

	printf(ID "UDP Client started up. Attempting communication with server @ %s:%d...\n\n", IPSERVER, PORTSERVER);

	for(i = 1; i <= NPACKETS; i++)
	{
		sprintf(data, "data(%d/%d)\0", i, NPACKETS);
		printf(ID "Sending packet %d of %d with data: \"%s\" ...\n", i, NPACKETS, data);
		fflush(stdout);
		if(sendto(socket_fd, data, strlen(data) + 1, 0, (struct sockaddr*) &sin_server, size_sockaddr) == -1)
			er("sendto()", 3);
		
		if(recvfrom(socket_fd, data, LENBUFFER, 0, (struct sockaddr*) &sin_server, &size_sockaddr) == -1)
			er("recvfrom()", 4);
		printf(ID "Reply from Server: \"%s\"\n\n", data);
		fflush(stdout);
		
		sleep(2);	// better is to make the argument a variable in the window [1, 2] E |R
	}

	close(socket_fd);

	return 0;
}

