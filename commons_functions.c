#include "commons.h"

struct packet* ntohp(struct packet* np)
{
	size_t size_packet = sizeof(struct packet);
	struct packet* hp = (struct packet*) malloc(size_packet);
	memset(hp, 0, size_packet);
	
	hp->conid = ntohs(np->conid);
	hp->type = ntohs(np->type);
	hp->status = ntohs(np->status);
	strcpy(hp->buffer, np->buffer);
	
	return hp;	
}

struct packet* htonp(struct packet* hp)
{
	size_t size_packet = sizeof(struct packet);
	struct packet* np = (struct packet*) malloc(size_packet);
	memset(np, 0, size_packet);
	
	np->conid = ntohs(hp->conid);
	np->type = ntohs(hp->type);
	np->status = ntohs(hp->status);
	strcpy(np->buffer, hp->buffer);
	
	return np;
}

