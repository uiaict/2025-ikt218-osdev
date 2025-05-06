#ifndef GDT_H
#define GDT_H

#include <libc/stdint.h>

// GDT entry structure
struct gdt_entry {
    uint16_t limit_low;      // Lower 16 bits of the limit
    uint16_t base_low;       // Lower 16 bits of the base
    uint8_t  base_middle;    // Next 8 bits of the base
    uint8_t  access;         // Access flags
    uint8_t  granularity;    // Granularity and upper 4 bits of limit
    uint8_t  base_high;      // Upper 8 bits of the base
};

// GDT pointer structure (for gdt_flush)
struct gdt_ptr {
    uint16_t limit;          // Size of the GDT
    uint32_t base;           // Base address of the GDT
};

// Function to initialize the GDT
void gdt_init();

#endif
