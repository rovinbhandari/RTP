#include "commons.h"

#include <time.h>
/*
	for:
		ctime()
*/

#include <sys/stat.h>
/*
	for:
		stat()
*/

#include <pthread.h>
/*
	for:
		pthread_mutex_init()
		pthread_cond_init()
		pthread_create()
		pthread_cond_signal()
		pthread_cond_wait()
		pthread_exit()
*/

#define	ID		"SERVER=> "		// identifies the messages sent to stdout by the server.
#define	MAXTHREADS	10			// a limit can be put to the maximum number of threads to be created at any time, hence limiting the number of clients served by the server simultaneously; the limit may be due to resource restriction.

#define	CLOSED		0			// state of a thread which is free to process a new client's request. (~Unlocked state)
#define	OPEN		1			// state of a thread which is busy processing a client's request. (~Locked state)

#define	FNF		"THERE WAS AN ERROR IN LOCATING THE FILE.\n"	// File Not Found error.

// the following struct sets the information to be passed to a new thread created by the main thread of the server.
struct client_info
{
	struct packet hp;			// packet (server host packet) as received from the client.
	struct sockaddr_in sinc;		// the sockaddr_in struct of the client to be served by the (newly) created thread.
};

struct client_info* client_info_alloc(struct packet*, struct sockaddr_in*);		// allocates memory for the struct client_info.

char* timestamp(char*);				// locates the file and returns its last modified timestamp in a char*.

