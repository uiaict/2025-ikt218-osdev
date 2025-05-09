#pragma once
#include "libc/stdint.h"

// http://www.osdever.net/bkerndev/Docs/gdt.htm 
// https://wiki.osdev.org/Global_Descriptor_Table
struct gdt_entry_struct {
    uint16_t limit;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed));

struct gdt_ptr_struct {
    uint16_t limit;
    unsigned int base;
} __attribute__((packed));

void gdt_init();
void gdt_set_gate(uint32_t index, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity);