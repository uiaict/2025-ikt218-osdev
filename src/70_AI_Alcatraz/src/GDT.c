#include "GDT.h"
#include "libc/stdint.h"

// GDT entries and pointer
struct gdt_entry gdt[3];
struct gdt_ptr gp;

// Function to set a GDT entry
void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    // Set base address
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    // Set limits
    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F);

    // Set granularity and access flags
    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access = access;
}

// Assembly function to load the GDT
extern void gdt_flush(uint32_t);

// Initialize the GDT
void gdt_init() {
    // Setup GDT pointer and limit
    gp.limit = (sizeof(struct gdt_entry) * 3) - 1;
    gp.base = (uint32_t)&gdt;

    // NULL descriptor
    gdt_set_gate(0, 0, 0, 0, 0);

    // Code segment: base=0, limit=4GB, access=0x9A (present, ring 0, code segment, readable)
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    // Data segment: base=0, limit=4GB, access=0x92 (present, ring 0, data segment, writable)
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    // Load the GDT
    gdt_flush((uint32_t)&gp);
}
