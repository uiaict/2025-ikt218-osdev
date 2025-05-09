#ifndef IDT_H
#define IDT_H

#include "libc/stdint.h"

void init_idt(void);

// Define the IDT entry structure
typedef struct {
    uint16_t base_low;
    uint16_t sel;
    uint8_t always0;
    uint8_t flags;
    uint16_t base_high;
} __attribute__((packed)) idt_entry_t;

// Define the IDT pointer structure
typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_ptr_t;

// Declare the IDT and IDT pointer
extern idt_entry_t idt[256];   // IDT with 256 entries
extern idt_ptr_t idt_ptr;      // Pointer to IDT structure

// Function declarations
void idt_install();  // Install the IDT
extern void idt_load();  // IDT load function in assembly

#endif  // IDT_H
