#include "io.h"

void outb(uint16_t port, uint8_t val){
    asm volatile ( "outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
};