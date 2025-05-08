#ifndef IDT_H
#define IDT_H

#include "libc/stdint.h"

// An entry in the IDT (8 bytes total)
struct idt_entry {
    uint16_t offset_low;   // Lower 16 bits of handler address
    uint16_t selector;     // Kernel code segment selector
    uint8_t  zero;         // This must always be zero
    uint8_t  flags;        // Type and attributes
    uint16_t offset_high;  // Upper 16 bits of handler address
} __attribute__((packed));

// Pointer structure for lidt instruction (6 bytes total)
struct idt_ptr {
    uint16_t limit;        // Size of IDT (bytes) - 1
    uint32_t base;         // Address of first element in IDT
} __attribute__((packed));

#define IDT_ENTRIES 256       // Total number of interrupt vectors

extern struct idt_entry idt[IDT_ENTRIES];
extern struct idt_ptr   idtp;

// Set an entry in the IDT
void idt_set_gate(uint8_t num, uint32_t handler_addr, uint16_t sel, uint8_t flags);

// Install our IDT
void install_idt(void);

// Prototypes for three ISRs
void isr0(void);
void isr1(void);
void isr2(void);

#endif // IDT_H
