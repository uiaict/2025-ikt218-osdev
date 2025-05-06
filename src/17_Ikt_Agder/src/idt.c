#include "libc/idt.h"
#include "libc/stdio.h"
#include "libc/stddef.h"

// Define the IDT with 256 entries
idt_entry_t idt[256];  // Array of IDT entries

// Define the IDT pointer
idt_ptr_t idt_ptr;  // Pointer to IDT structure

// Declare the PIT handler function (assuming it's defined elsewhere)
extern void pit_handler(void);  // Make sure this matches the declaration in pit.c


// Function to set an entry in the IDT
void set_idt_entry(int n, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[n].base_low = (base & 0xFFFF);
    idt[n].base_high = (base >> 16) & 0xFFFF;
    idt[n].sel = sel;
    idt[n].always0 = 0;
    idt[n].flags = flags;
}

void init_idt(void) {
    // IDT initialization code
    for (int i = 0; i < 256; i++) {
        set_idt_entry(i, 0, 0, 0);  // Set all entries to 0 initially 
    }   

    set_idt_entry(32, (uint32_t)pit_handler, 0x08, 0x8E); 
}
// Function to install the IDT
void idt_install() {
    // Set up the IDT pointer (size and base address)
    idt_ptr.limit = sizeof(idt) - 1;  // Set the limit of the IDT
    idt_ptr.base = (uint32_t)&idt;     // Set the base address of the IDT

    // Load the IDT using the 'lidt' instruction
    idt_load();
}