#ifndef IDT_H
#define IDT_H

#include "libc/stdint.h"

#define IDT_ENTRIES 256

// Struct for an IDT entry 
typedef struct {
    uint16_t base_low;
    uint16_t sel;       // Kernel segment selector
    uint8_t always0;
    uint8_t flags;
    uint16_t base_high;
} __attribute__((packed)) idt_entry_t;

// Pointer struct passed to 'lidt'
typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_ptr_t;

// Declare the IDT and its pointer
extern idt_entry_t idt[IDT_ENTRIES];
extern idt_ptr_t idt_reg;

// Function declarations (signatures only)
void idt_install();
void idt_set_gate(int n, uint32_t handler);
extern void idt_flush(uint32_t); // Assembly function



#endif
