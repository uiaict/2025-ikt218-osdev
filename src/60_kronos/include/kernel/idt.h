#pragma once
#include "libc/stdint.h"

// http://www.osdever.net/bkerndev/Docs/idt.htm 
struct idt_entry_struct {
    uint16_t base_low;
    uint16_t sel;
    uint8_t always_zero;
    uint8_t flags;
    uint16_t base_high;
} __attribute__((packed));

struct idt_ptr_struct {
    uint16_t limit;
    unsigned int base;
} __attribute__((packed));

struct idt_entry_struct idt[256];
struct idt_ptr_struct idtp;

void idt_init();
void idt_set_gate(uint16_t index, uint32_t base, uint16_t sel, uint8_t flags);