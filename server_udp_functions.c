#include "server_udp.h"

struct client_info* client_info_alloc(struct packet* p, struct sockaddr_in* s)		// allocates memory for the struct client_info.
{
	struct client_info* c = (struct client_info*) malloc(sizeof(struct client_info));
	
	memcpy(&(c->hp), p, sizeof(struct packet));
	memcpy(&(c->sinc), s, sizeof(struct sockaddr_in));
	
	return c;
}

char* timestamp(char* path)				// locates the file and returns its last modified timestamp in a char*.
{
	int x;			// stores error code of system calls.
	struct stat fst;
	memset((char*) &fst, 0, sizeof(struct stat));
	char s[LENBUFFER] = FNF;
		
	if(x = stat(path, &fst))			// if the given path was not correct or the permissions of the path are not set appropriately
		return s;
		
	return ctime(&fst.st_mtime);
}

