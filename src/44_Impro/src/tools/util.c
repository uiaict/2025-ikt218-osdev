#include "util.h"



//send data (1B) to a port
void outPortB(uint16_t port, uint8_t data) {
    asm volatile ("outb %1, %0" :: "dN" (port), "a" (data));
}

//read a byte from a port
uint8_t inPortB(uint16_t port) {
    uint8_t result;
    asm volatile ("inb %1, %0"
                  : "=a"(result)
                  : "dN"(port));
    return result;
}



