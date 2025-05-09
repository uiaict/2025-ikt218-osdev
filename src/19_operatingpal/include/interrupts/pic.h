#pragma once

#include "libc/stdint.h"

// PIC I/O port addresses
#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1

// End-of-interrupt command code
#define PIC_EOI 0x20

// Remaps PIC interrupts to avoid conflicts with CPU exceptions (IRQs start at offset1/offset2)
void pic_remap(int offset1, int offset2);

// Sends end-of-interrupt signal to the appropriate PIC
void pic_send_eoi(uint8_t irq);
