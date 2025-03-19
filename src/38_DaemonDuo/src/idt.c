#include "idt.h"
#include "terminal.h"

// Define the IDT with 256 entries (maximum possible interrupts)
struct idt_entry idt[256];
struct idt_ptr idtp;

// Function to set an IDT gate
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_lo = base & 0xFFFF;          // Lower 16 bits of the base address
    idt[num].base_hi = (base >> 16) & 0xFFFF; // Upper 16 bits of the base address
    idt[num].sel = sel;                       // Kernel segment selector
    idt[num].always0 = 0;                     // Always set to 0
    idt[num].flags = flags;                   // Flags (e.g., present, privilege level)
}

// Inline assembly to load the IDT
void idt_load() {
    __asm__ __volatile__("lidt (%0)" : : "r" (&idtp));
}

// Example ISRs
void isr_0x20() {
    // Handle interrupt 0x20
    writeline("ISR 0x20 triggered!\n");
}

void isr_0x21() {
    // Handle interrupt 0x21
    writeline("ISR 0x21 triggered!\n");
}

void isr_0x22() {
    // Handle interrupt 0x22
    writeline("ISR 0x22 triggered!\n");
}

// Function to install the IDT
void idt_install() {
    idtp.limit = (sizeof(struct idt_entry) * 256) - 1; // Size of the IDT
    idtp.base = (uint32_t)&idt;                       // Base address of the IDT

    // Clear the IDT by setting all entries to 0
    for (int i = 0; i < 256; i++) {
        idt_set_gate(i, 0, 0, 0);
    }

    // Set up ISRs for interrupts 0x20, 0x21, and 0x22
    idt_set_gate(0x20, (uint32_t)isr_0x20, 0x08, 0x8E);
    idt_set_gate(0x21, (uint32_t)isr_0x21, 0x08, 0x8E);
    idt_set_gate(0x22, (uint32_t)isr_0x22, 0x08, 0x8E);

    idt_load(); // Load the IDT
}