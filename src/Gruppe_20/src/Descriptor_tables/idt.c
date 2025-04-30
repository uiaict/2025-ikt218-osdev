#include "libc/idt.h"
#include "libc/isr.h"
#include "libc/stdint.h"
#include "io.h"  // For outb()
//#include "descriptor_tables.h"
#include "libc/common.h"


//static struct idt_entry idt_entries[256];
//static struct idt_ptr idt_ptr;

// Define the IDT structures
idt_entry_t idt_entries[256];
idt_ptr_t idt_ptr;

extern void* isr_stub_table[];

// External assembly functions
extern void idt_flush(uint32_t);

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt_entries[num].base_low = base & 0xFFFF;
    idt_entries[num].base_high = (base >> 16) & 0xFFFF;
    idt_entries[num].selector = sel;
    idt_entries[num].always0 = 0;
    idt_entries[num].flags = flags;
}

void init_idt() {
    idt_ptr.limit = sizeof(struct idt_entry) * 256 - 1;
    idt_ptr.base = (uint32_t)&idt_entries;

    // Initialize PIC (Programmable Interrupt Controller)
    outb(0x20, 0x11);  // ICW1
    outb(0xA0, 0x11);
    outb(0x21, 0x20);  // ICW2: Master PIC vector offset
    outb(0xA1, 0x28);  // ICW2: Slave PIC vector offset
    outb(0x21, 0x04);  // ICW3
    outb(0xA1, 0x02);
    outb(0x21, 0x01);  // ICW4
    outb(0xA1, 0x01);
    outb(0x21, 0x0);   // Mask interrupts
    outb(0xA1, 0x0);

    // Set up first few ISRs
    for (int i = 0; i < 32; i++) {
        idt_set_gate(i, (uint32_t)isr_stub_table[i], 0x08, 0x8E);
    }

    idt_flush((uint32_t)&idt_ptr);
}