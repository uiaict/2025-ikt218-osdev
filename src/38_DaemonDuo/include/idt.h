#ifndef IDT_H
#define IDT_H

#include "libc/stdint.h"
#include "terminal.h"

// IDT entry
struct idt_entry {
    uint16_t base_lo;
    uint16_t sel;
    uint8_t always0;
    uint8_t flags;
    uint16_t base_hi;
} __attribute__((packed));

// IDT pointer
struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

// Function to set an IDT gate
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);

// Inline assembly to load the IDT
void idt_load();

void isr_0x20();
void isr_0x21();
void isr_0x22();

// Function to install the IDT
void idt_install();

#endif