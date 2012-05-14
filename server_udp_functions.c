#include "server_udp.h"

char* timestamp(char* path)
{
	struct stat fst;
	memset((char*) &fst, 0, sizeof(struct stat));

	if(stat(path, &fst))
		er("stat()", -1);
	
	return ctime(&fst.st_mtime);
}
