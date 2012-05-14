#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
        //if (argc < 2) { printf("Usage: %s filename\n",argv[0]); exit(-1); }

        struct stat fst;
        bzero(&fst,sizeof(fst));
        
        //printf("sizeof(struct stat) = %d\n", sizeof(struct stat)); 

        if (stat(argv[1],&fst) != 0) { printf("stat() failed with errno %d\n",errno); exit(-1); }

        //printf("Information for %s\n",argv[1]);
        //printf("----------------------------\n\n");
        //printf("Last accessed:\t %s",ctime(&fst.st_atime));
        printf("Last modified:\t %s",ctime(&fst.st_mtime));
        //printf("Last changed:\t %s",ctime(&fst.st_ctime)); 
        
        return 0;
}
