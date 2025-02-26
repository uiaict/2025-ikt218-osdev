// src/arch/i386/gdt.h
#ifndef ARCH_I386_GDT_H
#define ARCH_I386_GDT_H

#include "libc/stdint.h"

// GDT entry structure
struct gdt_entry {
    uint16_t limit_low;    // The lower 16 bits of the limit
    uint16_t base_low;     // The lower 16 bits of the base
    uint8_t  base_middle;  // The next 8 bits of the base
    uint8_t  access;       // Access flags
    uint8_t  granularity;  // Granularity flags + limit bits 16-19
    uint8_t  base_high;    // The last 8 bits of the base
} __attribute__((packed));

// GDT pointer structure
struct gdt_ptr {
    uint16_t limit;        // Size of GDT minus 1
    uint32_t base;         // Base address of GDT
} __attribute__((packed));

// Initialize GDT
void gdt_init(void);

// External assembly function to reload segments
extern void gdt_flush(uint32_t);

#endif