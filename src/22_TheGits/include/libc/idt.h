#ifndef IDT_H
#define IDT_H

#include "libc/stdint.h"
#include "libc/io.h"
#define IDT_SIZE 256

void send_eoi(uint8_t irq);
void remap_pic();

// IDT Entry structure
struct idt_entry {
    uint16_t offset_low;  // Lower 16 bits of the ISR address
    uint16_t selector;    // Code segment selector in GDT
    uint8_t zero;         // Must be zero
    uint8_t type_attr;    // Type and attributes
    uint16_t offset_high; // Upper 16 bits of the ISR address
} __attribute__((packed));

// IDT Pointer structure
struct idt_ptr {
    uint16_t limit; // Size of the IDT - 1
    uint32_t base;  // Base address of the IDT
} __attribute__((packed));


void init_idt();
void set_idt_entry(int index, uint32_t isr, uint16_t selector, uint8_t type_attr);
void default_int_handler();

#endif // IDT_H