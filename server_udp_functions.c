#include "server_udp.h"

char* timestamp(char* path)
{
	int x;
	struct stat fst;
	memset((char*) &fst, 0, sizeof(struct stat));

	if(x = stat(path, &fst))
		er("stat()", x);
	
	return ctime(&fst.st_mtime);
}
