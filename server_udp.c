#include "server_udp.h"

int main(void)
{
	struct sockaddr_in sin_server, sin_client;
	int socket_fd, size_sockaddr = sizeof(struct sockaddr), port_client;
	char data[LENBUFFER];

	if((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		er("socket()", 1);
	
	memset((char*) &sin_server, 0, sizeof(struct sockaddr_in));
	sin_server.sin_family = AF_INET;
	sin_server.sin_port = htons(PORTSERVER);
	sin_server.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(socket_fd, (struct sockaddr*) &sin_server, size_sockaddr) == -1)
		er("bind()", 2);
	
	printf(ID "UDP Server started up @ local:%d. Waiting for client(s)...\n\n", PORTSERVER);

	while(1)
	{
		if(recvfrom(socket_fd, data, LENBUFFER, 0, (struct sockaddr*) &sin_client, &size_sockaddr) == -1)
			er("recvfrom()", 3);
		port_client = ntohs(sin_client.sin_port);
		printf(ID "Packet received from %s:%d with data: \"%s\"\n", inet_ntoa(sin_client.sin_addr), port_client, data);
		fflush(stdout);
		
		char reply[LENBUFFER];
		sprintf(reply, "Last modified timestamp is: %s", timestamp(data));
		if(sendto(socket_fd, reply, strlen(reply) + 1, 0, (struct sockaddr*) &sin_client, size_sockaddr) == -1)
			er("sendto()", 4);
	}

	close(socket_fd);

	return 0;
}

