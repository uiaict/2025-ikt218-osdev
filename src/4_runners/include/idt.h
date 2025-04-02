#pragma once

#include "libc/stdint.h"

struct idt_entry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t  zero;
    uint8_t  type_attr;
    uint16_t offset_high;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

void idt_init(void);
void set_idt_entry(int index, uint32_t handler, uint16_t selector, uint8_t flags); // <--- ADD THIS
