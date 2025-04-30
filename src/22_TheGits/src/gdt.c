
#include "libc/gdt.h"

struct gdt_entry gdt[3]; // Null, Code, Data
struct gdt_ptr gdt_descriptor;

// Function to set up a GDT entry
void set_gdt_entry(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
    gdt[index].base_low = base & 0xFFFF;
    gdt[index].base_middle = (base >> 16) & 0xFF;
    gdt[index].base_high = (base >> 24) & 0xFF;

    gdt[index].limit_low = limit & 0xFFFF;
    gdt[index].granularity = (limit >> 16) & 0x0F;
    gdt[index].granularity |= granularity & 0xF0;
    gdt[index].access = access;
}

void load_gdt(struct gdt_ptr* gdt_descriptor) {
    __asm__ volatile (
        "lgdt (%0)\n"          // Load GDT using the lgdt instruction
        "mov $0x10, %%ax\n"    // Data segment selector (index 2)
        "mov %%ax, %%ds\n"     // Update DS
        "mov %%ax, %%es\n"     // Update ES
        "mov %%ax, %%fs\n"     // Update FS
        "mov %%ax, %%gs\n"     // Update GS
        "mov %%ax, %%ss\n"     // Update SS
        "ljmp $0x08, $1f\n"    // Far jump to code segment selector (index 1)
        "1:\n"                 // Label for jump target
        :
        : "r"(gdt_descriptor)  // Input: pointer to the GDT descriptor
        : "memory", "rax"      // Clobbered: memory and rax
    );
}

// Initializes the GDT
void init_gdt() {
    gdt_descriptor.limit = (sizeof(struct gdt_entry) * 3) - 1;
    gdt_descriptor.base = (uint32_t)&gdt;

    set_gdt_entry(0, 0, 0, 0, 0);                // NULL segment
    set_gdt_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
    set_gdt_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment

    load_gdt(&gdt_descriptor); // Load GDT with inline assembly
}