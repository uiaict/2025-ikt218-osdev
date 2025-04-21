#pragma once
#include <stdint.h>

#define IDT_ENTRIES 256

// Strukturen til én IDT-entry (hver interrupt peker til en handler)
struct idt_entry {
    uint16_t offset_low;    // Lower 16 bits av adressen til ISR
    uint16_t selector;      // Kernel segment selector
    uint8_t  zero;          // Alltid 0
    uint8_t  type_attr;     // Type og attributter
    uint16_t offset_high;   // Upper 16 bits av ISR-adressen
} __attribute__((packed));

// IDT-ptr (brukes av lidt-instruksjonen)
struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

void idt_init();
void idt_set_gate(int n, uint32_t handler, uint16_t selector, uint8_t flags);
