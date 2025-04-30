#include "libc/gdt.h"
#include "libc/stdint.h"

// Define the GDT structures
static gdt_entry_t gdt_entries[5];
static gdt_ptr_t gdt_ptr;

// External assembly function to load GDT
extern void gdt_flush(uint32_t);

void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt_entries[num].base_low = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low = (limit & 0xFFFF);
    gdt_entries[num].granularity = ((limit >> 16) & 0x0F) | (gran & 0xF0);
    gdt_entries[num].access = access;
}

void init_gdt() {
    gdt_ptr.limit = sizeof(gdt_entries) - 1;
    gdt_ptr.base = (uint32_t)&gdt_entries;

    // Null descriptor
    gdt_set_gate(0, 0, 0, 0, 0);
    
    // Kernel code segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 
                GDT_ACCESS_PRESENT | GDT_ACCESS_PRIVILEGE_RING0 | GDT_ACCESS_EXECUTABLE | GDT_ACCESS_READWRITE, 
                GDT_GRAN_4KB | GDT_GRAN_32BIT);
    
    // Kernel data segment
    gdt_set_gate(2, 0, 0xFFFFFFFF, 
                GDT_ACCESS_PRESENT | GDT_ACCESS_PRIVILEGE_RING0 | GDT_ACCESS_READWRITE, 
                GDT_GRAN_4KB | GDT_GRAN_32BIT);
    
    // User code segment
    gdt_set_gate(3, 0, 0xFFFFFFFF, 
                GDT_ACCESS_PRESENT | GDT_ACCESS_PRIVILEGE_RING3 | GDT_ACCESS_EXECUTABLE | GDT_ACCESS_READWRITE, 
                GDT_GRAN_4KB | GDT_GRAN_32BIT);
    
    // User data segment
    gdt_set_gate(4, 0, 0xFFFFFFFF, 
                GDT_ACCESS_PRESENT | GDT_ACCESS_PRIVILEGE_RING3 | GDT_ACCESS_READWRITE, 
                GDT_GRAN_4KB | GDT_GRAN_32BIT);

    // Load the GDT
    gdt_flush((uint32_t)&gdt_ptr);
}