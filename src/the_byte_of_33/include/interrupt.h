#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <libc/stdint.h>

// IDT entry structure
struct idt_entry {
    uint16_t base_low;   // Lower 16 bits of handler address
    uint16_t sel;        // Kernel segment selector
    uint8_t  zero;       // Always 0
    uint8_t  flags;      // Flags (e.g., present, privilege level)
    uint16_t base_high;  // Upper 16 bits of handler address
} __attribute__((packed));

// IDT pointer structure
struct idt_ptr {
    uint16_t limit;      // Size of IDT - 1
    uint32_t base;       // Base address of IDT
} __attribute__((packed));

void init_idt(void);
void init_irq(void);
void isr_handler(uint8_t interrupt);
void irq_handler(uint8_t irq);

#endif