#include "../include/gdt.h"

// Define GDT entries
struct gdt_entry gdt[3]; // NULL, Code, Data
struct gdt_ptr gp;

// Function to set a GDT gate
void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    // Set descriptor base address
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;
    
    // Set descriptor limits
    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F);
    
    // Set granularity and access flags
    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access = access;
}

// Initialize the GDT
void gdt_init() {
    // Set up the GDT pointer
    gp.limit = (sizeof(struct gdt_entry) * 3) - 1;
    gp.base = (uint32_t)&gdt;
    
    // NULL descriptor (index 0)
    gdt_set_gate(0, 0, 0, 0, 0);
    
    // Code segment descriptor (index 1)
    // Access: Present=1, DPL=0, Type=1, Code=1, Conforming=0, Readable=1, Accessed=0
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    
    // Data segment descriptor (index 2)
    // Access: Present=1, DPL=0, Type=1, Code=0, Expand-Down=0, Writable=1, Accessed=0
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
    
    // Load the GDT
    gdt_flush((uint32_t)&gp);
}