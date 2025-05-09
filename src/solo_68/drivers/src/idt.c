#include "idt.h"
#include "mem.h"

extern void idt_load(uint32_t);

#define IDT_ENTRIES 256

static struct idt_entry idt[IDT_ENTRIES];
static struct idt_ptr idtp;

void set_idt_entry(int i, uint32_t base, uint16_t selector, uint8_t flags) {
    idt[i].base_low = base & 0xFFFF;
    idt[i].selector = selector;
    idt[i].zero = 0;
    idt[i].type_attr = flags;
    idt[i].base_high = (base >> 16) & 0xFFFF;
}

void init_idt() {
    idtp.limit = sizeof(struct idt_entry) * IDT_ENTRIES - 1;
    idtp.base = (uint32_t)&idt;

    memset(&idt, 0, sizeof(struct idt_entry) * IDT_ENTRIES);

    extern void* isr_stub_table[];
    for (int i = 0; i < 32; ++i) {
        set_idt_entry(i, (uint32_t)isr_stub_table[i], 0x08, 0x8E);
    }

    idt_load((uint32_t)&idtp);
}