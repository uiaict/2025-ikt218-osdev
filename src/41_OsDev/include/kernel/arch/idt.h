#pragma once
#include <stdint.h>

////////////////////////////////////////
// IDT Initialization
////////////////////////////////////////

// Install the Interrupt Descriptor Table
void idt_install(void);

////////////////////////////////////////
// IDT Entry Structures
////////////////////////////////////////

// Single IDT entry (8 bytes)
struct idt_entry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t  zero;
    uint8_t  type_attr;
    uint16_t offset_high;
} __attribute__((packed));

// IDT pointer (passed to lidt instruction)
struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

////////////////////////////////////////
// IDT Configuration
////////////////////////////////////////

// Set an individual IDT gate
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);
