#include "libc/stdint.h"

#define GDT_ENTRIES 5

struct gdt_entry{
    // 1 entry is 8 bytes
    uint16_t limit_low;
    uint16_t base_low; 
    uint8_t base_middle;
    uint8_t access;     // 4bit pres, priv, type, && 4bit Type flags
    uint8_t granularity; // 4bit Other flags && 4bit junk (1111)
    uint8_t base_high;
} __attribute__((packed));

struct gdt_ptr{
    // Properties for the eventual gdt_entry array
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));


void init_gdt();

void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);

void gdt_load(struct gdt_ptr *gdt_ptr);