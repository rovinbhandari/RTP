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
*/

#include <stdlib.h>
/*
	for:
		exit()
*/

#include <string.h>
/*
	for:
		memset()
		strlen()
*/

#include <unistd.h>
/*
	for:
		close()
		sleep()
*/

#define	LENBUFFER	512
#define	PORTSERVER	8484

#define	er(e, x)					\
	do						\
	{						\
		perror("ERROR: " #e "\n");		\
		exit(x);				\
	}						\
	while(0)

