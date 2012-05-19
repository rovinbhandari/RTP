#include "server_udp.h"

int socket_fd, x;			// socket_fd: socket file descriptor for the server to connect to. x: stores error code of system calls.
pthread_t threads[MAXTHREADS];				// thread-pool.
pthread_mutex_t threads_mutex[MAXTHREADS];		// thread mutex array.
pthread_cond_t threads_cond[MAXTHREADS];		// cond wait array.
int threads_state[MAXTHREADS];				// strores the states of the threads according to id (= index of the array).
	
void* serve_client(void* info);		// serves the client's request till the client sends a "done acknowledgement".

int main(void)
{
	// BEGIN: initialization
	int nthreads = 0;		// stores count of simultaneous clients being served at any time.
	struct sockaddr_in sin_server, sin_client;
	size_t size_sockaddr = sizeof(struct sockaddr), size_packet = sizeof(struct packet);
	struct packet* shp;							// shp: server host packet
	struct packet* data = (struct packet*) malloc(size_packet);		// data: network packet
	
	if((x = socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)		// intializes the socket file descriptor for client-server communication (server side).
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
		threads_state[i] = CLOSED;		// because initially, all threads in the pool are to be made available.
		
		if(x = pthread_mutex_init(&threads_mutex[i], NULL))
			er("pthread_mutex_init()", x);
		
		if(x = pthread_cond_init(&threads_cond[i], NULL))
			er("pthread_cond_init()", x);
	}
	
	printf(ID "UDP Server started up @ local:%d. Waiting for client(s)...\n\n", PORTSERVER);
	// END: initialization
	
	// the following loop should not be confused with "Busy Waiting"! It is merely meant to run indefinite times (till there is an error which exit()s the server or the user kills the server process).
	while(1)
	{
		// BEGIN: request + done acknowledgement
		set0(data);		// clearing out the struct before initalization.
		if((x = recvfrom(socket_fd, data, size_packet, 0, (struct sockaddr*) &sin_client, &size_sockaddr)) == -1)
			er("request + done acknowledgement recvfrom()", x);
		
		shp = ntohp(data);
		
		printpacket(shp, HP);
		
		printf(ID "Packet received from %s:%d with data: %s\n", inet_ntoa(sin_client.sin_addr), ntohs(sin_client.sin_port), shp->buffer);
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
			
			// assign id to the new connection. gives the id of the free thread having lowest index value.
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
	}
		
	return 0;
}

void* serve_client(void* info)
{
	// create local copies of the client information obtained via the (only) argument passed through pthread_create().
	struct client_info* ci = (struct client_info*) info;
	struct packet cihp = ci->hp;
	struct packet* data;
	struct sockaddr_in cisinc = ci->sinc;
	char path[LENBUFFER], ts[LENBUFFER];		// path: temporary storage for the path of the file whose timestamp is being requested by the client. ts: temporary storage for the timestamp of the file whose timestamp is being requested by the client.
	size_t size_sockaddr = sizeof(struct sockaddr), size_packet = sizeof(struct packet);
	
	threads_state[cihp.conid] = OPEN;	// renders the thread running current call of this function locked.
	
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
	strcpy(ts, timestamp(path));
	if(!strcmp(ts, FNF))
		cihp.status = -1;
	sprintf(cihp.buffer, "Last modified timestamp is: %s", ts);
	
	printpacket(&cihp, HP);
	
	data = htonp(&cihp);
	if((x = sendto(socket_fd, data, size_packet, 0, (struct sockaddr*) &cisinc, size_sockaddr)) == -1)
		er("done sendto()", x);
	
	printf(ID "Timestamp sent.\n");
	fflush(stdout);
	// END: done
	
	pthread_cond_wait(&threads_cond[cihp.conid], &threads_mutex[cihp.conid]);	// waits for the signal from the client connected to the thread running current call of this function to send "done acknowledgement".
	threads_state[cihp.conid] = CLOSED;	// renders the thread running current call of this function unlocked (free for other clients to take up).
	
	pthread_exit(NULL);		// terminates the thread running current call of this function.
}

