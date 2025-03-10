#ifndef IDT_H
#define IDT_H

#include <libc/stdint.h>

struct idt_entry_t {
    uint16_t base_low;
    uint16_t selector;
    uint8_t always0;
    uint8_t flags;
    uint16_t base_high;
} __attribute__((packed));

// Funksjon for å sette opp en IDT-entry (brukes av ISRs og IRQs)
void idt_set_gate(int num, uint32_t base, uint16_t sel, uint8_t flags);

struct idt_ptr_t {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

void init_idt();

#endif
