#pragma once

#include "libc/stdint.h"

// Ports for PIC
#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1

// Commands
#define PIC_EOI 0x20  // End of interrupt

void pic_remap(int offset1, int offset2);
void pic_send_eoi(uint8_t irq);
