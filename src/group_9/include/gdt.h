#ifndef GDT_H
#define GDT_H

#include <stdint.h>

// GDT entry structure
struct gdt_entry {
    uint16_t limit_low;   
    uint16_t base_low; 
    uint8_t base_middle;
    uint8_t access;   
    uint8_t granularity; 
    uint8_t base_high;   
} __attribute__((packed));


// GDT pointer structure
struct gdt_ptr {
    uint16_t limit; 
    uint32_t base;     
} __attribute__((packed));


// Assembly function to flush GDT
extern void gdt_flush(uint32_t);

// High-level function to set up the GDT
void gdt_install();

#endif