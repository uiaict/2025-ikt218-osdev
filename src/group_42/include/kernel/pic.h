#ifndef PIC_H
#define PIC_H

#include "libc/stdint.h"

void remap_pic();
void send_eoi(uint8_t irq);

#endif