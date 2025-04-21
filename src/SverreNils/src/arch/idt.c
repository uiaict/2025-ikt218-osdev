#include "arch/idt.h"

extern void idt_load(struct idt_ptr*); // Implementeres i ASM

static struct idt_entry idt[IDT_ENTRIES];
static struct idt_ptr idt_desc;

static void idt_set_gate(int n, uint32_t handler, uint16_t selector, uint8_t flags) {
    idt[n].offset_low = handler & 0xFFFF;
    idt[n].selector = selector;
    idt[n].zero = 0;
    idt[n].type_attr = flags;
    idt[n].offset_high = (handler >> 16) & 0xFFFF;
}

void idt_init() {
    idt_desc.limit = sizeof(struct idt_entry) * IDT_ENTRIES - 1;
    idt_desc.base = (uint32_t)&idt;

    // Vi setter ikke opp faktiske ISR-er her ennå – det kommer i Task 2

    idt_load(&idt_desc);
}
