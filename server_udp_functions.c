#include "server_udp.h"

struct client_info* client_info_alloc(struct packet* p, struct sockaddr_in* s)
{
	struct client_info* c = (struct client_info*) malloc(sizeof(struct client_info));
	
	memcpy(&(c->hp), p, sizeof(struct packet));
	memcpy(&(c->sinc), s, sizeof(struct sockaddr_in));
	
	return c;
}

char* timestamp(char* path)
{
	int x;
	struct stat fst;
	memset((char*) &fst, 0, sizeof(struct stat));

	if(x = stat(path, &fst))
		er("stat()", x);
	
	return ctime(&fst.st_mtime);
}
