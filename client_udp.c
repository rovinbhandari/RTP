#include "client_udp.h"

int main(int argc, char* argv[])
{
	if(argc != 2)
		er("incorrect usage", -1);
		
	// BEGIN: initialization
	struct sockaddr_in sin_server;
	int socket_fd, x;			// socket_fd: socket file descriptor for the client to connect to. x: stores error code of system calls.
	size_t size_sockaddr = sizeof(struct sockaddr), size_packet = sizeof(struct packet);
	short int connection_id;		// temporary storage for the connection id of the client-server communication.
	struct packet* chp = (struct packet*) malloc(size_packet);		// chp: client host packet
	struct packet* data;							// data: network packet
	
	if((x = socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)		// intializes the socket file descriptor for client-server communication (client side).
		er("socket()", x);
	
	memset((char*) &sin_server, 0, sizeof(struct sockaddr_in));	// clearing out the struct before initalization.
	sin_server.sin_family = AF_INET;
	sin_server.sin_port = htons(PORTSERVER);
	
	if(!(x = inet_aton(IPSERVER, &sin_server.sin_addr)))
		er("inet_aton()", x);
			
	printf(ID "UDP Client started up. Attempting communication with server @ %s:%d...\n\n", IPSERVER, PORTSERVER);
	// END: initialization
	
	// BEGIN: request
	set0(chp);		// clearing out the struct before initalization.
	chp->type = REQU;
	chp->conid = -1;
	strcpy(chp->buffer, argv[1]);
	
	printpacket(chp, HP);
	
	printf(ID "Requesting Server for timestamp of: %s ...\n", chp->buffer);
	fflush(stdout);
	
	data = htonp(chp);
	if((x = sendto(socket_fd, data, size_packet, 0, (struct sockaddr*) &sin_server, size_sockaddr)) == -1)
		er("request sendto()", x);
	// END: request
	
	// BEGIN: request acknowledgement
	set0(data);		// clearing out the struct before initalization.
	if((x = recvfrom(socket_fd, data, size_packet, 0, (struct sockaddr*) &sin_server, &size_sockaddr)) == -1)
		er("request acknowledgement recvfrom()", x);
	
	chp = ntohp(data);
	
	printpacket(chp, HP);
	
	connection_id = chp->conid;
	// do error checking here...
	// ...
	
	printf(ID "Reply from Server: %s\n", chp->buffer);
	fflush(stdout);
	// END: request acknowledgement
	
	// BEGIN: done
	set0(data);		// clearing out the struct before initalization.
	if((x = recvfrom(socket_fd, data, size_packet, 0, (struct sockaddr*) &sin_server, &size_sockaddr)) == -1)
		er("done recvfrom()", x);
	
	chp = ntohp(data);
	
	printpacket(chp, HP);
	
	// do error checking here...
	// ...
	
	printf(ID "Reply from Server: %s", chp->buffer);
	fflush(stdout);
	// END: done
	
	// BEGIN: done acknowledgement
	set0(chp);		// clearing out the struct before initalization.
	chp->type = DACK;
	chp->conid = connection_id;
	strcpy(chp->buffer, "Thanks!");
	
	printpacket(chp, HP);
	
	data = htonp(chp);
	if((x = sendto(socket_fd, data, size_packet, 0, (struct sockaddr*) &sin_server, size_sockaddr)) == -1)
		er("done acknowledgement sendto()", x);
	
	printf(ID "Bye!\n");
	fflush(stdout);
	// END: done acknowledgement
	
	// BEGIN: cleanup
	free(chp);
	free(data);
	close(socket_fd);
	printf(ID "Done.\n");
	fflush(stdout);
	// END: cleanup
	
	return 0;
}

