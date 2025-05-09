#ifndef GDT_H
#define GDT_H

#include "libc/stdint.h"

// Represents a single entry in the Global Descriptor Table (GDT)
struct GDT_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  base_high;
} __attribute__((packed));

// Special pointer structure used by the LGDT instruction to load the GDT
struct GDT_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

// Initializes the Global Descriptor Table and loads it into the CPU
void gdt_init();

#endif
