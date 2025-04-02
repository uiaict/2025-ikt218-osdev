#include "irq.h"
#include "pic.h"
#include "idt.h"  // Assuming you have this for IDT functions
#include <libc/stdint.h>

// Array of function pointers for IRQ handlers
static irq_handler_t irq_handlers[16] = {0};

// Common C function to handle all IRQs
void irq_common_handler(uint32_t irq_num) {
    // Convert from IDT number to IRQ number
    uint8_t irq = irq_num - 0x20;
    
    // Send EOI (End of Interrupt) signal to the PICs
    pic_send_eoi(irq);
    
    // If we have a handler for this IRQ, call it
    if (irq_handlers[irq] != 0) {
        irq_handlers[irq]();
    }

}

// Register a handler for a specific IRQ
void irq_install_handler(uint8_t irq, irq_handler_t handler) {
    if (irq < 16) {
        irq_handlers[irq] = handler;
        pic_clear_mask(irq);  // Enable this IRQ
    }
}

// Unregister a handler for a specific IRQ
void irq_uninstall_handler(uint8_t irq) {
    if (irq < 16) {
        irq_handlers[irq] = 0;
        pic_set_mask(irq);  // Disable this IRQ
    }
}

// Initialize IRQ system
void irq_init(void) {
    // Initialize the PIC
    pic_init();
    
    // Map IRQs to interrupts 0x20-0x2F in the IDT
    for (int i = 0; i < 16; i++) {
        // For IRQs 0-7: map to interrupts 0x20-0x27
        // For IRQs 8-15: map to interrupts 0x28-0x2F
        int interrupt_num = 0x20 + i;
        
        // Set up the IDT entry for this IRQ
        // Your exact implementation will depend on your IDT setup function
        // This assumes you have a function like:
        // idt_set_gate(interrupt_num, handler_address, selector, flags)
        //extern void* irq_handlers_table[16];
        //void idt_set_interrupt_gate(uint8_t vector, void* isr_handler);
        extern void* irq_handler_table[16]; // Declare the table of ISR handler addresses
        idt_set_interrupt_gate(interrupt_num, irq_handlers[i]); // Use 'i' as the index
    }
    
    // Initially mask all interrupts except for what we explicitly need
    for (int i = 0; i < 16; i++) {
        if (i != IRQ1) { // Keep keyboard (IRQ1) enabled as in your pic_init
            pic_set_mask(i);
        }
    }
}