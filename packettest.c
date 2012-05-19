#include "commons.h"

#define	NP	0
#define	HP	1

void printpacket(struct packet* p, int ptype)
{
	if(ptype)
		printf("\nHOST PACKET\n");
	else
		printf("\nNETWORK PACKET\n");
	
	printf("conid = %d\n", p->conid);
	printf("type = %d\n", p->type);
	printf("status = %d\n", p->status);
	printf("buffer = %s\n", p->buffer);
}

int main(void)
{
	size_t s = sizeof(struct packet);
	printf("Size of struct packet = %d\n", s);
	
	struct packet* a = (struct packet*) malloc(s);
	memset(a, 0, s);
	
	a->conid = 544;
	a->type = RACK;
	a->status = 0;
	strcpy(a->buffer, "U MAD BYTE ORDER?");
	
	struct packet* b;
	b = (struct packet*) malloc(s);
	memcpy(b, a, s);
	
	b->conid = 84;
	strcpy(b->buffer, "PROBLEM, ENDIAN?");
	
	printpacket(a, HP);
	printpacket(a, NP);
	
	printpacket(b, HP);
	
	struct packet* c;
	printpacket(c, HP);
	
	c = htonp(a);
	printpacket(c, NP);
	printpacket(c, HP);
	
	b = ntohp(c);
	printpacket(b, HP);
	
	printpacket(a, HP);
	
	return 0;
}

