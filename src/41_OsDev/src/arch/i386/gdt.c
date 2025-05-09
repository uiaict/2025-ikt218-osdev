#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <kernel/arch/gdt.h>

////////////////////////////////////////
// GDT Entry and Pointer Structures
////////////////////////////////////////

struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  base_high;
} __attribute__((packed));

struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

////////////////////////////////////////
// Global Variables
////////////////////////////////////////

struct gdt_entry gdt[3];
struct gdt_ptr   gp;

extern void gdt_flush(uint32_t);

////////////////////////////////////////
// GDT Setup
////////////////////////////////////////

// Set one GDT entry
void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt[num].base_low     = base & 0xFFFF;
    gdt[num].base_middle  = (base >> 16) & 0xFF;
    gdt[num].base_high    = (base >> 24) & 0xFF;

    gdt[num].limit_low    = limit & 0xFFFF;
    gdt[num].granularity  = ((limit >> 16) & 0x0F) | (gran & 0xF0);
    gdt[num].access       = access;
}

// Install the GDT and load it with lgdt
void gdt_install() {
    gp.limit = (sizeof(struct gdt_entry) * 3) - 1;
    gp.base  = (uint32_t)&gdt;

    gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);  // Code segment
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);  // Data segment

    gdt_flush((uint32_t)&gp);
}
