// wrappers.c
#include "interrupts.h"
#include "irq.h"
#include "descriptor_tables.h"

// Initialize and start the IDT
void start_idt(void) {
    // Simply call the internal function that sets up the IDT
    // This appears to come from descriptor_tables.h
    idt_ptr.limit = sizeof(struct idt_entry_t) * IDT_ENTRIES - 1;
    idt_ptr.base = (uint32_t) &idt;

    for (int i = 0; i < IDT_ENTRIES; i++) {
        idt[i].low = 0x0000;
        idt[i].high = 0x0000;
        idt[i].selector = 0x08;
        idt[i].zero = 0x00;
        idt[i].flags = 0x8E;
    }
    
    // Set up interrupt gates
    start_interrupts();
    
    // Load the IDT
    extern void idt_flush(uint32_t);
    idt_flush((uint32_t)&idt_ptr);
}

// Initialize the IRQ system
void start_irq(void) {
    // Call the init_irq function from irq.c
    init_irq();
}