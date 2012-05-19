#include <arpa/inet.h>
/*
	for:
		htons()
		htonl()
		ntohs()
		inet_aton()
		inet_ntoa()
*/

#include <netinet/in.h>
/*
	for:
		inet_aton()
		inet_ntoa()
*/

#include <sys/types.h>
/*
	for:
		socket()
		bind()
		recvfrom()
		sendto()
		stat()
*/

#include <sys/socket.h>
/*
	for:
		socket()
		bind()
		recvfrom()
		sendto()
		inet_aton()
		inet_ntoa()
		AF_INET
		SOCK_DGRAM
*/

#include <errno.h>
/*
	for:
		return type of system calls
*/

#include <stdio.h>
/*
	for:
		printf()
		sprintf()
		fflush()
		perror()
		NULL
*/

#include <stdlib.h>
/*
	for:
		exit()
		malloc()
*/

#include <string.h>
/*
	for:
		memset()
		strlen()
		strcpy()
*/

#include <unistd.h>
/*
	for:
		close()
		sleep()
		stat()
*/

#define	DEBUG		1		// used to toggle "debug mode".

#define	LENBUFFER	506		// so as to make the whole packet well-rounded ( = 512 bytes)
#define	PORTSERVER	8484

#define	REQU		1
#define RACK		2
#define	DONE		3
#define	DACK		4

#define	NP		0		// network packet
#define	HP		1		// host packet


// print errors and error codes to stderr
#define	er(e, x)					\
	do						\
	{						\
		perror("ERROR IN: " #e "\n");		\
		fprintf(stderr, "%d\n", x);		\
		exit(-1);				\
	}						\
	while(0)

// the following packet structure serves for both host and network packet, the subsequent conversion functions handle the nature of the packet.
struct packet
{
	short int conid;
	short int type;
	short int status;
	char buffer[LENBUFFER];
};

void set0(struct packet*);		// sets memory allocated to a packet to 0.

struct packet* ntohp(struct packet*);	// converts a packet from network to host.
struct packet* htonp(struct packet*);	// converts a packet from host to network.

void printpacket(struct packet*, int);	// prints the contents and the nature of a packet. prints only in the debug mode.

