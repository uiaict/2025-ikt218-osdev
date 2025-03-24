#include "utils.h"


void memset(void *ptr, char value, uint32_t count){
    char *tmp = (char*)ptr;
    for (; count != 0; count--) {
        *tmp++ = value;
    }
}

void outPortB(uint16_t port, uint8_t value)
{
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}