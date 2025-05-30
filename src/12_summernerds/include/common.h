#ifndef COMMON_H
#define COMMON_H

#include "libc/stdint.h"

#ifdef __cplusplus
extern "C"
{
#endif

    void outb(uint16_t port, uint8_t value);
    void outw(uint16_t port, uint16_t value);
    uint8_t inb(uint16_t port);
    uint16_t inw(uint16_t port);

#ifdef __cplusplus
}
#endif

#endif
