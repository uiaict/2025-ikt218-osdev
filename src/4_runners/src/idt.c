#include "idt.h"

#define IDT_ENTRIES 256

static struct idt_entry idt[IDT_ENTRIES];
static struct idt_ptr idtp;

// Assembly stubs
extern void isr0_stub(void);
extern void isr1_stub(void);
extern void isr2_stub(void);
extern void idt_flush(uint32_t);

void set_idt_entry(int idx, uint32_t base, uint16_t selector, uint8_t flags) {
    idt[idx].offset_low  = base & 0xFFFF;
    idt[idx].selector    = selector;
    idt[idx].zero        = 0;
    idt[idx].type_attr   = flags;
    idt[idx].offset_high = (base >> 16) & 0xFFFF;
}

void idt_init(void) {
    idtp.limit = sizeof(struct idt_entry) * IDT_ENTRIES - 1;
    idtp.base = (uint32_t)&idt;

    for (int i = 0; i < IDT_ENTRIES; i++)
        set_idt_entry(i, 0, 0, 0);

    set_idt_entry(0, (uint32_t)isr0_stub, 0x08, 0x8E);
    set_idt_entry(1, (uint32_t)isr1_stub, 0x08, 0x8E);
    set_idt_entry(2, (uint32_t)isr2_stub, 0x08, 0x8E);

    idt_flush((uint32_t)&idtp);
}
