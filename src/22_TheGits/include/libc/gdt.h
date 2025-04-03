
#ifndef GDT_H
#define GDT_H

#include "libc/stdint.h"

// GDT entry structure
struct gdt_entry {
    uint16_t limit_low;  // Lower 16 bits of the limit
    uint16_t base_low;   // Lower 16 bits of the base
    uint8_t base_middle; // Next 8 bits of the base
    uint8_t access;      // Access flags
    uint8_t granularity; // Granularity and limit high bits
    uint8_t base_high;   // Last 8 bits of the base
} __attribute__((packed));


struct gdt_ptr {
    uint16_t limit; 
    uint32_t base;  
} __attribute__((packed));

// Funksjonsdeklarasjoner
void init_gdt();

#endif // GDT_H