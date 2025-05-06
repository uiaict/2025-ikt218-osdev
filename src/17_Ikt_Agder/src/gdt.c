#include "libc/gdt.h"
#include <libc/stdint.h>

// Define the GDT with 3 entries: NULL, Code, and Data segments
struct gdt_entry gdt[3];
struct gdt_ptr gp;

void print(const char* str) {
    volatile char *vga = (char*)0xB8000;
    while (*str) {
        *vga = *str++;      // Character
        *(vga + 1) = 0x0F;  // White on black
        vga += 2;
    }
}


// Function to set a GDT entry
void gdt_set_entry(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = (limit >> 16) & 0x0F;

    gdt[num].granularity |= gran & 0xF0;
    gdt[num].access = access;
}

// Load the GDT (extern assembly function)
extern void gdt_flush(uint8_t);

// Initialize the GDT
void gdt_init() {
    gp.limit = (sizeof(struct gdt_entry) * 3) - 1; // Size of the GDT minus 1
    gp.base = (uint8_t)&gdt; // Base address of the GDT

    // Set up the three GDT entries
    gdt_set_entry(0, 0, 0, 0, 0);                // NULL segment
    gdt_set_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
    gdt_set_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment

     // Print debug message to check progress
     print("GDT initialized, before flush\n");  // Your custom printing function

    // Load the GDT and switch to protected mode
    gdt_flush((uint8_t)&gp);
}
