#include "libc/idt.h"
#include "libc/isr.h"
#include "libc/stdint.h"
#include "io.h"
#include "libc/common.h"

#define IDT_ENTRIES_USED 48

idt_entry_t idt_entries[256];
idt_ptr_t idt_ptr;

extern void* isr_stub_table[];
extern void idt_flush(uint32_t);

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt_entries[num].base_low = base & 0xFFFF;
    idt_entries[num].base_high = (base >> 16) & 0xFFFF;
    idt_entries[num].selector = sel;
    idt_entries[num].always0 = 0;
    idt_entries[num].flags = flags;
}

void remap_pic() {
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x00);
    outb(0xA1, 0x00);
}

void init_idt() {
    idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
    idt_ptr.base = (uint32_t)&idt_entries;

    // Optional: clear all entries first
    for (int i = 0; i < 256; i++) {
        idt_set_gate(i, 0, 0, 0);
    }

    remap_pic();

    // Load ISR (0–31) and IRQ (32–47) stubs
    for (int i = 0; i < IDT_ENTRIES_USED; i++) {
        idt_set_gate(i, (uint32_t)isr_stub_table[i], 0x08, 0x8E);
    }

    idt_flush((uint32_t)&idt_ptr);
}
