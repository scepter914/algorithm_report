#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main(void){
    int filesize, pagesize, mmapsize;
    char *addr;
    filesize = lseek(0,0,SEEK_END);
    printf("hoge");
    pagesize = getpagesize();
    printf("hoge");
    mmapsize = (filesize + (pagesize - 1)) / pagesize * pagesize;
    printf("hoge");
    addr = mmap(0, mmapsize, PROT_READ, MAP_PRIVATE, 0, 0);
    printf("hoge");
    printf("%s\n", (char *)addr[1]); 
}

