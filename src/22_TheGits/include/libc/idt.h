#ifndef IDT_H
#define IDT_H

#include "libc/stdint.h"
#include "libc/io.h"

void send_eoi(uint8_t irq);
void remap_pic();

// IDT Entry struktur
struct idt_entry {
    uint16_t offset_low;  // Lavere 16 biter av ISR-adressen
    uint16_t selector;    // Kode-segment selector i GDT
    uint8_t zero;         // Må være null
    uint8_t type_attr;    // Type og attributter
    uint16_t offset_high; // Øvre 16 biter av ISR-adressen
} __attribute__((packed));

// IDT Pointer struktur
struct idt_ptr {
    uint16_t limit; // Størrelsen på IDT - 1
    uint32_t base;  // Baseadresse til IDT
} __attribute__((packed));

// Funksjonsprototyper
void init_idt();
void set_idt_entry(int index, uint32_t isr, uint16_t selector, uint8_t type_attr);
void default_int_handler();

#endif // IDT_H