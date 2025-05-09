#include "interrupts.h"
#include "terminal.h"

struct idt_entry_t idt_entries[IDT_ENTRIES];
struct idt_ptr_t   idt_ptr;

// Configure a single IDT
static void idt_set_gate(uint8_t n, uint32_t handler, uint16_t sel, uint8_t flags) {
    idt_entries[n].offset_low  = handler & 0xFFFF;
    idt_entries[n].selector    = sel;
    idt_entries[n].zero        = 0;
    idt_entries[n].type_attr   = flags;             // e.g. 0x8E = present, ring 0, 32‑bit interrupt gate
    idt_entries[n].offset_high = (handler >> 16) & 0xFFFF;
}

// Install ISR 1,2,3 and load IDT
void init_interrupts(void) {
    idt_ptr.limit = sizeof(struct idt_entry_t) * IDT_ENTRIES - 1;
    idt_ptr.base  = (uint32_t)&idt_entries;

    // Clear table
    for (int i = 0; i < IDT_ENTRIES; i++) {
        idt_set_gate(i, 0, 0, 0);
    }

    // Set the three ISRs
    idt_set_gate(ISR1, (uint32_t)isr1, 0x08, 0x8E);
    idt_set_gate(ISR2, (uint32_t)isr2, 0x08, 0x8E);
    idt_set_gate(ISR3, (uint32_t)isr3, 0x08, 0x8E);

    // Load it into the CPU
    idt_load((uint32_t)&idt_ptr);
}

// Called by each ISR stub with the vector number
void isr_common(int int_no) {
    terminal_write("Caught interrupt: ");
    terminal_put('0' + int_no);
    terminal_put('\n');
}