#include "client_udp.h"

int main(int argc, char* argv[])
{
	if(argc != 2)
		er("incorrect usage", -1);
		
	struct sockaddr_in sin_server;
	int size_sockaddr = sizeof(struct sockaddr), socket_fd;
	char data[LENBUFFER];

	if((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		er("socket()", 1);
	
	memset((char*) &sin_server, 0, sizeof(struct sockaddr_in));
	sin_server.sin_family = AF_INET;
	sin_server.sin_port = htons(PORTSERVER);
	
	if(!inet_aton(IPSERVER, &sin_server.sin_addr))
		er("inet_aton()", 2);

	printf(ID "UDP Client started up. Attempting communication with server @ %s:%d...\n\n", IPSERVER, PORTSERVER);

	strcpy(data, argv[1]);
	printf(ID "Sending packet with data: \"%s\" ...\n", data);
	fflush(stdout);
	if(sendto(socket_fd, data, strlen(data) + 1, 0, (struct sockaddr*) &sin_server, size_sockaddr) == -1)
		er("sendto()", 3);
	
	if(recvfrom(socket_fd, data, LENBUFFER, 0, (struct sockaddr*) &sin_server, &size_sockaddr) == -1)
		er("recvfrom()", 4);
	printf(ID "Reply from Server: %s\n\n", data);
	fflush(stdout);
	
	close(socket_fd);

	return 0;
}

