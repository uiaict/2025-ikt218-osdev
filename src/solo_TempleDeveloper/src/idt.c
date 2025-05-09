#include "idt.h"
#include "libc/stdio.h"

// Define the IDT and its pointer
struct idt_entry idt[256];
struct idt_ptr   idtp;

// Assembly function to load the IDT (lidt)
extern void idt_load(uint32_t idt_ptr_address);

// Common C handler called from assembly stubs
void isr_handler(int int_number) {
    printf("Handled interrupt %d\n", int_number);
}

// Sets an individual entry in the IDT
void idt_set_gate(uint8_t num, uint32_t handler_addr, uint16_t sel, uint8_t flags) {
    idt[num].offset_low  = (uint16_t)(handler_addr & 0xFFFF);
    idt[num].selector    = sel;
    idt[num].zero        = 0;
    idt[num].flags       = flags;
    idt[num].offset_high = (uint16_t)((handler_addr >> 16) & 0xFFFF);
}

// Initializes the IDT and loads it into the CPU
void install_idt(void) {
    idtp.limit = (sizeof(idt) - 1);
    idtp.base  = (uint32_t)&idt;

    // fill up IDT with default values
    for (int i = 0; i < 256; i++) {
        idt_set_gate(i, 0, 0, 0);
    }

    // Set three example ISRs (interrupt vectors 0, 1, and 2)
    // 0x08 = kernel code segment, 0x8E = present, ring 0, 32-bit interrupt gate
    idt_set_gate(0, (uint32_t)isr0, 0x08, 0x8E);  //isr0, isr1... from assembly
    idt_set_gate(1, (uint32_t)isr1, 0x08, 0x8E);
    idt_set_gate(2, (uint32_t)isr2, 0x08, 0x8E);

    // Load the IDT into the CPU
    idt_load((uint32_t)&idtp);
}
