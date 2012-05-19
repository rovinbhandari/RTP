#include "commons.h"

void set0(struct packet* p)			// sets memory allocated to a packet to 0.
{
	memset(p, 0, sizeof(struct packet));
}

struct packet* ntohp(struct packet* np)		// converts a packet from network to host.
{
	size_t size_packet = sizeof(struct packet);
	struct packet* hp = (struct packet*) malloc(size_packet);
	memset(hp, 0, size_packet);
	
	hp->conid = ntohs(np->conid);
	hp->type = ntohs(np->type);
	hp->status = ntohs(np->status);
	strcpy(hp->buffer, np->buffer);		// char* datatype doesn't need conversion; it is copied as it is.
	
	return hp;	
}

struct packet* htonp(struct packet* hp)		// converts a packet from host to network.
{
	size_t size_packet = sizeof(struct packet);
	struct packet* np = (struct packet*) malloc(size_packet);
	memset(np, 0, size_packet);
	
	np->conid = ntohs(hp->conid);
	np->type = ntohs(hp->type);
	np->status = ntohs(hp->status);
	strcpy(np->buffer, hp->buffer);		// char* datatype doesn't need conversion; it is copied as it is.
	
	return np;
}

void printpacket(struct packet* p, int ptype)	// prints the contents and the nature of a packet. prints only in the debug mode.
{
	if(!DEBUG)
		return;
	
	if(ptype)
		printf("\t\tHOST PACKET\n");
	else
		printf("\t\tNETWORK PACKET\n");
	
	printf("\t\tconid = %d\n", p->conid);
	printf("\t\ttype = %d\n", p->type);
	printf("\t\tstatus = %d\n", p->status);
	printf("\t\tbuffer = %s\n\n", p->buffer);
	
	fflush(stdout);
}

