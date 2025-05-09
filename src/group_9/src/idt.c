#include "idt.h"
#include "isr.h"
#include "irq.h"
#include <stdint.h>
#include <string.h> 


struct idt_entry idt[256];
struct idt_ptr idtp;

extern void idt_load(uint32_t);

// Function to set an entry in the IDT
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_low  = base & 0xFFFF;
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].sel       = sel;
    idt[num].always0   = 0;
    idt[num].flags     = flags;
}

// Function to initialize and load the IDT
void idt_install() {
    idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
    idtp.base  = (uint32_t)&idt;

    memset(&idt, 0, sizeof(struct idt_entry) * 256);

    isr_install();   // Load CPU exception handlers
    irq_install();   // Load IRQ handlers

    idt_load((uint32_t)&idtp);  // Load the IDT into the CPU
}
