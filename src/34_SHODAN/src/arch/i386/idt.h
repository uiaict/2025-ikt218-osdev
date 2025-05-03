#ifndef IDT_H
#define IDT_H

#include <stdint.h>

#define IDT_ENTRIES 256

struct idt_entry_t {
    uint16_t base_low;
    uint16_t sel;       // Kernel segment selector
    uint8_t  always0;
    uint8_t  flags;
    uint16_t base_high;
} __attribute__((packed));

struct idt_ptr_t {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

void idt_install();
void set_idt_gate(int n, uint32_t handler);

// Pointer to ISR stub functions defined in isr_stubs.asm
extern void* isr_stub_table[];

#endif
