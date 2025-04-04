// idt.c
#include "idt.h"
#include <kprint.h>
#include "isr.h"

// Define the IDT entries and pointer
idt_entry_t idt_entries[256];
idt_ptr_t idt_ptr;

// Function to load the IDT
extern void idt_flush(uint32_t);

// Set an IDT gate (entry)
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt_entries[num].base_low = base & 0xFFFF;
    idt_entries[num].base_high = (base >> 16) & 0xFFFF;
    idt_entries[num].selector = sel;
    idt_entries[num].always0 = 0;
    // Uncomment the OR below if you want to user mode to be able to use interrupts
    idt_entries[num].flags = flags /* | 0x60 */;
}

// Initialize the IDT
void idt_init(void) {
    // Set up the IDT pointer
    idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
    idt_ptr.base = (uint32_t)&idt_entries;
    
    // Clear all IDT entries initially
    for (int i = 0; i < 256; i++) {
        idt_set_gate(i, 0, 0x08, 0x8E);
    }
    
    // Load the IDT
    idt_flush((uint32_t)&idt_ptr);
    
    kprint("IDT initialized\n");
}

// Assembly function to load the IDT
__asm__(
    ".global idt_flush\n"
    "idt_flush:\n"
    "   movl 4(%esp), %eax\n"  // Get the pointer to the IDT
    "   lidt (%eax)\n"         // Load the IDT
    "   ret\n"
);

// Initialize the PIC
void pic_init(void) {
    // ICW1: Start initialization sequence in cascade mode
    outb(PIC1_COMMAND, 0x11);
    outb(PIC2_COMMAND, 0x11);
    
    // ICW2: Set vector offset - Master: 0x20 (32), Slave: 0x28 (40)
    outb(PIC1_DATA, 0x20);    // Master PIC starts at 32
    outb(PIC2_DATA, 0x28);    // Slave PIC starts at 40
    
    // ICW3: Tell Master PIC that there is a slave PIC at IRQ2
    outb(PIC1_DATA, 0x04);    // Slave at IRQ2 (0000 0100)
    outb(PIC2_DATA, 0x02);    // Slave cascade identity (0000 0010)
    
    // ICW4: Set 8086 mode
    outb(PIC1_DATA, 0x01);
    outb(PIC2_DATA, 0x01);
    
    // Mask all interrupts (except IRQ2 for cascade)
    outb(PIC1_DATA, 0xFB);    // 1111 1011 - enable IRQ2
    outb(PIC2_DATA, 0xFF);    // 1111 1111 - disable all
    
    kprint("PIC initialized\n");
}