#include "../include/libc/stdint.h"  // Change this line to use local stdint.h
#include "../include/gdt.h"

// GDT entries array - space for NULL, CODE, and DATA segments
static struct gdt_entry_struct gdt_entries[3];
static struct gdt_ptr_struct gdt_ptr;

// Helper function to set up a GDT gate
static void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt_entries[num].base_low = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low = (limit & 0xFFFF);
    gdt_entries[num].flags = ((limit >> 16) & 0x0F) | (gran & 0xF0);

    gdt_entries[num].access = access;
}

// External assembly function declaration
extern void gdt_flush(uint32_t);

void init_gdt(void) {
    gdt_ptr.limit = (sizeof(struct gdt_entry_struct) * 3) - 1;
    gdt_ptr.base = (uint32_t)&gdt_entries;

    // NULL descriptor (required)
    gdt_set_gate(0, 0, 0, 0, 0);
    
    // Code segment descriptor
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    
    // Data segment descriptor
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    // Load the GDT using assembly function
    gdt_flush((uint32_t)&gdt_ptr);
}
