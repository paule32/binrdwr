#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

void vmExec(uint8_t * image, size_t codelen)
{
    char *d = mmap(0,codelen ,
    PROT_READ   | PROT_WRITE |
    PROT_EXEC   ,
    MAP_PRIVATE | MAP_ANON,-1,0);
    memcpy(d,image,codelen);
    ((void(*)(void))d)();
}
