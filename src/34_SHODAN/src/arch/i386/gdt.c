#include "gdt.h"

#define GDT_ENTRIES 3  // We define 3 GDT entries: NULL, Code, and Data

// The actual GDT array
struct gdt_entry gdt[GDT_ENTRIES];

// The GDT pointer (structure passed to 'lgdt')
struct gdt_ptr gp;

// This function is written in assembly and flushes the old GDT
extern void gdt_flush(uint32_t);

// Helper function to set up a GDT entry
static void gdt_set_gate(int num, uint32_t base, uint32_t limit,
                         uint8_t access, uint8_t granularity) {
    gdt[num].base_low    = (base & 0xFFFF);        // Base bits 0–15
    gdt[num].base_middle = (base >> 16) & 0xFF;    // Base bits 16–23
    gdt[num].base_high   = (base >> 24) & 0xFF;    // Base bits 24–31

    gdt[num].limit_low   = (limit & 0xFFFF);       // Limit bits 0–15
    gdt[num].granularity = ((limit >> 16) & 0x0F); // Limit bits 16–19

    // Combine granularity flags with upper limit
    gdt[num].granularity |= (granularity & 0xF0);
    gdt[num].access = access;                      // Set access flags
}

// Initializes the GDT and loads it into the CPU
void gdt_install() {
    gp.limit = (sizeof(struct gdt_entry) * GDT_ENTRIES) - 1; // Total size
    gp.base = (uint32_t)&gdt;                                // Address of GDT

    // Null descriptor (required)
    gdt_set_gate(0, 0, 0, 0, 0);
    // Code segment: base 0x0, full limit, executable/readable
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    // Data segment: base 0x0, full limit, writable
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    // Load the new GDT with 'lgdt' and update segment registers
    gdt_flush((uint32_t)&gp);
}