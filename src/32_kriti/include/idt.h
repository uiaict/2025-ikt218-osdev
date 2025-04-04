// idt.h
#ifndef IDT_H
#define IDT_H

#include <libc/stdint.h>
#include <libc/stdbool.h>

// IDT entry structure
typedef struct {
    uint16_t base_low;      // Lower 16 bits of handler address
    uint16_t selector;      // Kernel segment selector
    uint8_t always0;        // Always zero
    uint8_t flags;          // Flags
    uint16_t base_high;     // Upper 16 bits of handler address
} __attribute__((packed)) idt_entry_t;

// IDT pointer structure
typedef struct {
    uint16_t limit;         // Size of IDT - 1
    uint32_t base;          // Base address of IDT
} __attribute__((packed)) idt_ptr_t;

// Initialize the IDT
void idt_init(void);

// Set an IDT gate (entry)
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);

// Initialize the PIC
void pic_init(void);

// PIC port constants
#define PIC1         0x20    // Master PIC
#define PIC2         0xA0    // Slave PIC
#define PIC1_COMMAND PIC1
#define PIC1_DATA    (PIC1+1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA    (PIC2+1)

#endif /* IDT_H */