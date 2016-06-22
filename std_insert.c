#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>

int main(void){
    int filesize, pagesize, mmapsize;
    char *addr;
    filesize = lseek(0,0,SEEK_END);
    pagesize = getpagesize();
    mmapsize = (filesize + (pagesize - 1)) / pagesize * pagesize;
    addr = mmap(0, mmapsize, PROT_READ, MAP_PRIVATE, 0, 0);
    printf("hoge\n");
    for(int i = 0; i < 10; i++){
        printf("%c\n", *(addr + i));
    }
}

