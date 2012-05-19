#include "server_udp.h"

int main(void)
{
	// BEGIN: initialization
	struct sockaddr_in sin_server, sin_client;
	int socket_fd, size_sockaddr = sizeof(struct sockaddr) /* make into size_t */, port_client;
	char data[LENBUFFER], path[LENBUFFER];

	if((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		er("socket()", 1);
	
	memset((char*) &sin_server, 0, sizeof(struct sockaddr_in));
	sin_server.sin_family = AF_INET;
	sin_server.sin_port = htons(PORTSERVER);
	sin_server.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(socket_fd, (struct sockaddr*) &sin_server, size_sockaddr) == -1)
		er("bind()", 2);
	
	printf(ID "UDP Server started up @ local:%d. Waiting for client(s)...\n\n", PORTSERVER);
	// END: initialization
	
	while(1)
	{
		// BEGIN: request	
		if(recvfrom(socket_fd, data, LENBUFFER, 0, (struct sockaddr*) &sin_client, &size_sockaddr) == -1)
			er("recvfrom()", 3);
		port_client = ntohs(sin_client.sin_port);
		strcpy(path, data);
		printf(ID "Packet received from %s:%d with data: %s\n", inet_ntoa(sin_client.sin_addr), port_client, data);
		fflush(stdout);
		// END: request
		
		// BEGIN: request acknowledgement
		sprintf(data, "Querying the timestamp of: %s", path);
		if(sendto(socket_fd, data, strlen(data) + 1, 0, (struct sockaddr*) &sin_client, size_sockaddr) == -1)
			er("sendto()", 4);
		printf(ID "Calling the timestamp function...\n");
		fflush(stdout);
		// END: request acknowledgement
		
		// BEGIN: done
		sprintf(data, "Last modified timestamp is: %s", timestamp(path));
		if(sendto(socket_fd, data, strlen(data) + 1, 0, (struct sockaddr*) &sin_client, size_sockaddr) == -1)
			er("sendto()", 4);
		printf(ID "Timestamp sent.\n");
		fflush(stdout);
		// END: done
		
		// BEGIN: done acknowledgement
		if(recvfrom(socket_fd, data, LENBUFFER, 0, (struct sockaddr*) &sin_client, &size_sockaddr) == -1)
			er("recvfrom()", 3);
		printf(ID "Client says: %s\n", data);
		fflush(stdout);
		// END: done acknowledgement
	}

	// BEGIN: cleanup
	close(socket_fd);
	printf(ID "Done.\n");
	fflush(stdout);
	// END: cleanup
	
	return 0;
}

