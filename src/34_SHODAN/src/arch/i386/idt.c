#include "idt.h"
#include <string.h>
#include "terminal.h"

// IDT structure
struct idt_entry_t idt[IDT_ENTRIES];
struct idt_ptr_t idtp;

// External assembly functions
extern void* isr_stub_table[]; // First 32 ISRs (0–31)
extern void* irq_stub_table[]; // IRQs (32–47)
extern void idt_load();        // Assembly function to load IDT
extern void irq_remap();       // Function to remap PIC IRQs

void set_idt_gate(int n, uint32_t handler) {
    idt[n].base_low = handler & 0xFFFF;
    idt[n].base_high = (handler >> 16) & 0xFFFF;
    idt[n].sel = 0x08;         // Kernel code segment selector
    idt[n].always0 = 0;
    idt[n].flags = 0x8E;       // Present, ring 0, 32-bit interrupt gate
}

void idt_install() {
    // Set up the IDT pointer
    idtp.limit = sizeof(struct idt_entry_t) * IDT_ENTRIES - 1;
    idtp.base = (uint32_t)&idt;
    memset(&idt, 0, sizeof(idt));

    // Remap PIC to avoid conflicts with CPU exceptions
    irq_remap();

    for (int i = 0; i < 16; i++) {
            set_idt_gate(32 + i, (uint32_t)irq_stub_table[i]);
            // terminal_write("IRQ stub set at ");
            // terminal_putint(32 + i);
            // terminal_write("\n");
        }
        
    // Install ISRs (0–31)
    for (int i = 0; i < 32; i++) {
        set_idt_gate(i, (uint32_t)isr_stub_table[i]);
    }

   
    

    // Load the new IDT
    idt_load();

    terminal_write("IDT installed!\n");
}
