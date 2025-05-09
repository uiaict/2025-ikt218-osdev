#include "util.h"

void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %1, %0" : : "dN"(port), "a"(val));
}

uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ( "inb %1, %0" : "=a"(ret) : "Nd"(port) );
    return ret;
}

static uint32_t seed = 1;

void srand(uint32_t s) {
    seed = s;
}

int rand() {
    seed = seed * 1103515245 + 12345;
    return (seed >> 16) & 0x7FFF;
}