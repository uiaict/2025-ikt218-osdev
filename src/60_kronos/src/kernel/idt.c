#include "kernel/idt.h"

extern void idt_flush(addr_t);

struct idt_entry_struct idt_entries[16];
struct idt_ptr_struct idt_ptr;

void idt_init() {
    idt_ptr.limit = (sizeof(struct idt_entry_struct) * 5) - 1;
    idt_ptr.base = &idt_entries;


    // Segments
    idt_set_gate(0, 0, 0, 0); // NULL
    // idt_set_gate(0, 0, 0, 0, 0);                 // Null
    // idt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);  // Kernel code
    // idt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);  // Kernel data
    // idt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);  // User code
    // idt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);  // User data

    idt_flush(&idt_ptr);
}

void idt_set_gate(uint16_t index, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[index].base_low = base & 0xFFFF;
    idt[index].base_high = (base >> 16) & 0xFFFF;
    idt[index].sel = sel;
    idt[index].always_zero = 0;
    idt[index].flags = flags; 
}