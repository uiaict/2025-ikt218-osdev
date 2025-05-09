#include "kernel/idt.h"
#include "libc/stdint.h"

extern void idt_flush(uint32_t);

// Define the variables here that are declared as extern in the header
struct idt_entry_struct idt[256];
struct idt_ptr_struct idtp;

void idt_init() {
    idtp.limit = (sizeof(struct idt_entry_struct) * 256) - 1;
    idtp.base = (uint32_t)&idt;

    // Initialize the IDT to zeros
    for (int i = 0; i < 256; i++) {
        idt_set_gate(i, 0, 0, 0);
    }

    // Load the IDT
    idt_flush((uint32_t)&idtp);
}

void idt_set_gate(uint16_t index, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[index].base_low = base & 0xFFFF;
    idt[index].base_high = (base >> 16) & 0xFFFF;
    idt[index].sel = sel;
    idt[index].always_zero = 0;
    idt[index].flags = flags; 
}