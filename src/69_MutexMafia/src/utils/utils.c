#include "utils.h"


void memset(void *ptr, char value, uint32_t count){
    char *tmp = (char*)ptr;
    for (; count != 0; count--) {
        *tmp++ = value;
    }
}

//funksjon som skriver til en I/O-port
void outPortB(uint16_t port, uint8_t value)
{
    asm volatile ("outb %0, %1 " : : "a" (value), "dN" (port));
}
//funksjon som leser fra en I/O-port
uint8_t inPortB(uint16_t port){
    uint8_t value;
    asm volatile ("inb %1, %0" : "=a" (value) : "dN" (port));
    return value;
}