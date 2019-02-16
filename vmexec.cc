#include "binheader.h"

void vmExec(uint8_t * image, uint32_t codelen)
{
    uint8_t *d  = reinterpret_cast<uint8_t*>(mmap(0,codelen,
    PROT_READ   | PROT_WRITE |
    PROT_EXEC   ,
    MAP_PRIVATE | MAP_ANON,-1,0));
    std::memcpy(d,image,codelen );
    ((void(*)(void))d)();
}
