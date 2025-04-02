#ifndef PIC_H
#define PIC_H

#include "libc/stdint.h"

void pic_remap(uint8_t offset1, uint8_t offset2);
void pic_clear_mask(uint8_t irq_line);
#endif