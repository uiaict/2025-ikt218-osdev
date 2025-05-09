#include <libc/idt.h>

#define IDT_ENTRIES 256
struct idt_entry_t idt[IDT_ENTRIES];
struct idt_ptr_t idt_ptr;

extern void idt_flush(uint32_t);

void idt_set_gate(int num, uint32_t base, uint16_t selector, uint8_t flags) {
    idt[num].base_low = base & 0xFFFF;
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].selector = selector;
    idt[num].always0 = 0;
    idt[num].flags = flags | 0x60;  // Bruk 0x60 for bruker-modus
}

void init_idt() {
    idt_ptr.limit = (sizeof(struct idt_entry_t) * IDT_ENTRIES) - 1;
    idt_ptr.base = (uint32_t)&idt;

    // Load the IDT
    idt_flush((uint32_t)&idt_ptr);
}
