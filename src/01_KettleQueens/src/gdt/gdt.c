#include "gdt/gdt.h"

// GDT Entry structure (8 bytes)
struct gdt_entry {
    uint16_t limit_low;      // Lower 16 bits of the segment limit
    uint16_t base_low;       // Lower 16 bits of the base
    uint8_t base_middle;     // Next 8 bits of the base
    uint8_t access;          // Access flags
    uint8_t granularity;     // Limit + flags
    uint8_t base_high;       // Last 8 bits of the base
} __attribute__((packed));

// GDT Pointer structure (used by lgdt)
struct gdt_ptr {
    uint16_t limit;          // Size of the GDT (in bytes - 1)
    uint32_t base;           // Address of the first GDT entry
} __attribute__((packed));

// Declare our GDT and GDT pointer
struct gdt_entry gdt[3];     // Null, Code, Data
struct gdt_ptr gp;

// Fills one entry in the GDT
static void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt[num].base_low    = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high   = (base >> 24) & 0xFF;

    gdt[num].limit_low   = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F);

    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access = access;
}

// Sets up the GDT with 3 entries and loads it
void gdt_init() {
    gp.limit = (sizeof(struct gdt_entry) * 3) - 1;
    gp.base = (uint32_t)&gdt;

    gdt_set_gate(0, 0, 0, 0, 0);                // NULL segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);  // Code segment
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);  // Data segment

    gdt_flush((uint32_t)&gp);  // Assembly function to actually load GDT
}
