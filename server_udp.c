#include "server_udp.h"

int socket_fd, x;
pthread_t threads[MAXTHREADS];
pthread_mutex_t threads_mutex[MAXTHREADS];
pthread_cond_t threads_cond[MAXTHREADS];
int threads_state[MAXTHREADS];
	
void* serve_client(void* info);

int main(void)
{
	// BEGIN: initialization
	int nthreads = 0;
	struct sockaddr_in sin_server, sin_client;
	unsigned short int port_client;
	size_t size_sockaddr = sizeof(struct sockaddr), size_packet = sizeof(struct packet);
	char path[LENBUFFER];
	short int connection_id = -2;
	struct packet* shp;							// client host packet
	struct packet* data = (struct packet*) malloc(size_packet);		// network packet
	
	if((x = socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		er("socket()", x);
	
	memset((char*) &sin_server, 0, sizeof(struct sockaddr_in));
	sin_server.sin_family = AF_INET;
	sin_server.sin_port = htons(PORTSERVER);
	sin_server.sin_addr.s_addr = htonl(INADDR_ANY);

	if((x = bind(socket_fd, (struct sockaddr*) &sin_server, size_sockaddr)) == -1)
		er("bind()", x);
	
	int i;
	for(i = 0; i < MAXTHREADS; i++)
	{
		threads_state[i] = CLOSED;
		
		if(x = pthread_mutex_init(&threads_mutex[i], NULL))
			er("pthread_mutex_init()", x);
		
		if(x = pthread_cond_init(&threads_cond[i], NULL))
			er("pthread_cond_init()", x);
	}
	
	printf(ID "UDP Server started up @ local:%d. Waiting for client(s)...\n\n", PORTSERVER);
	// END: initialization
	
	while(1)
	{
		// BEGIN: request + done acknowledgement
		set0(data);
		if((x = recvfrom(socket_fd, data, size_packet, 0, (struct sockaddr*) &sin_client, &size_sockaddr)) == -1)
			er("request + done acknowledgement recvfrom()", x);
		/*
		if(connection_id == -2)
			connection_id = 0;
		else
			connection_id++;
		*/
		shp = ntohp(data);
		//shp->conid = connection_id;
		printpacket(shp, HP);
		port_client = ntohs(sin_client.sin_port);
		strcpy(path, shp->buffer);
		printf(ID "Packet received from %s:%d with data: %s\n", inet_ntoa(sin_client.sin_addr), port_client, path);
		fflush(stdout);
		if(shp->conid == -1)
		{
			// create a new thread if nthreads <= MAXTHREADS.
			if(!(nthreads <= MAXTHREADS))
			{
				// drop the packet.
				if(DEBUG)
					fprintf(stderr, ID "PACKET DROPPED.\n");
				continue;
			}
			
			nthreads++;
			
			if(DEBUG)
				fprintf(stderr, ID "CREATING A NEW THREAD.\n");
			
			// assign id to the new connection.
			int j = 0;
			while(j < MAXTHREADS)
			{
				if(threads_state[j] == CLOSED)
					break;
				j++;
			}
			shp->conid = j;
			
			struct client_info* ci = client_info_alloc(shp, &sin_client);
			if(x = pthread_create(threads + nthreads, NULL, &serve_client, ci))
				er("pthread_create()", x);
		}
		else if(shp->type == DACK)
		{
			pthread_cond_signal(&threads_cond[shp->conid]);
			nthreads--;
			
			printf(ID "Client says: %s\n", shp->buffer);
			fflush(stdout);
		}
		else
		{
			// do error checking here...
			// ...
			fprintf(stderr, "STRAY PACKET RECEIVED\n");
		}
		// END: request + done acknowledgement
	
	
		/*
		// BEGIN: done acknowledgement
		set0(data);
		if((x = recvfrom(socket_fd, data, size_packet, 0, (struct sockaddr*) &sin_client, &size_sockaddr)) == -1)
			er("done acknowledgement recvfrom()", x);
		shp = ntohp(data);
		printpacket(shp, HP);
		// do error checking here...
		// ...
		printf(ID "Client says: %s\n", shp->buffer);
		fflush(stdout);
		set0(shp);
		set0(data);
		// END: done acknowledgement
		*/
	
	}
		
	/*
	// BEGIN: cleanup
	free(shp);
	free(data);
	//free(&sin_client);
	//free(&sin_server);
	close(socket_fd);
	printf(ID "Done.\n");
	fflush(stdout);
	// END: cleanup
	*/
	
	return 0;
}

void* serve_client(void* info)
{
	struct client_info* ci = (struct client_info*) info;
	struct packet cihp = ci->hp;
	struct packet* data;
	struct sockaddr_in cisinc = ci->sinc;
	char path[LENBUFFER];
	size_t size_sockaddr = sizeof(struct sockaddr), size_packet = sizeof(struct packet);
	
	threads_state[cihp.conid] = OPEN;
	
	strcpy(path, cihp.buffer);
	
	printpacket(&cihp, HP);
	
	// BEGIN: request acknowledgement
	cihp.type = RACK;
	sprintf(cihp.buffer, "Querying the timestamp of: %s", path);
	printpacket(&cihp, HP);
	data = htonp(&cihp);
	if((x = sendto(socket_fd, data, size_packet, 0, (struct sockaddr*) &cisinc, size_sockaddr)) == -1)
		er("request acknowledgement sendto()", x);
	printf(ID "Calling the timestamp function...\n");
	fflush(stdout);
	// END: request acknowledgement
	
	// BEGIN: done
	cihp.type = DONE;
	sprintf(cihp.buffer, "Last modified timestamp is: %s", timestamp(path));
	printpacket(&cihp, HP);
	data = htonp(&cihp);
	if((x = sendto(socket_fd, data, size_packet, 0, (struct sockaddr*) &cisinc, size_sockaddr)) == -1)
		er("done sendto()", x);
	printf(ID "Timestamp sent.\n");
	fflush(stdout);
	// END: done
	
	pthread_cond_wait(&threads_cond[cihp.conid], &threads_mutex[cihp.conid]);
	threads_state[cihp.conid] = CLOSED;
	
	pthread_exit(NULL);
}

