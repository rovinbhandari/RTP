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
		
*/

#define	ID		"SERVER=> "
#define	MAXTHREADS	10

#define	CLOSED		0
#define	OPEN		1

struct client_info
{
	struct packet hp;
	struct sockaddr_in sinc;
};

struct client_info* client_info_alloc(struct packet*, struct sockaddr_in*);

char* timestamp(char*);

