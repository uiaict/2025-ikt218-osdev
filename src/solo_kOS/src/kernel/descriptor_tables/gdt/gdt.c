// gdt.c
#include "kernel/gdt.h"

// We use the same GDT entry structure as in the header file.
// We need some global variables to hold the GDT entries and the GDT pointer so that both init_gdt and gdt_flush (in my .asm file) can access them.

// First, we have the actual array of GDT entries, which is the GDT itself.
struct gdt_entry gdt[GDT_ENTRIES];

// This is the GDT pointer, which will be passed to the CPU with the lgdt instruction.
struct gdt_ptr gdt_ptr;

// This function initializes the GDT — you can think of it like building a map that tells the CPU which parts of memory are safe to use and how they're configured.
void init_gdt() {
    // Set the GDT pointer: the size is the total size of all entries minus 1.
    gdt_ptr.limit = sizeof(struct gdt_entry) * GDT_ENTRIES - 1;

    // The base is just the address of the gdt array.
    gdt_ptr.base = (uint32_t)&gdt;

    // Null segment: This is a required entry and is just 8 zeroed bytes.
    // It's used to indicate that no segment is selected. No trick here — it just has to be all zeroes.
    gdt_set_gate(0, 0, 0, 0, 0);

    // Code segment: This covers the full 4GB address space (limit = 0xFFFFFFFF), with a base of 0 (flat memory model).
    // Access byte = 0x9A:
    // - Present: 1
    // - Privilege level: 0 (kernel mode)
    // - Executable: 1
    // - Readable: 1
    // Granularity byte = 0xCF:
    // - Granularity: 1 (4KB blocks)
    // - Size: 1 (32-bit segment)
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    // Data segment: Also covers 4GB, flat model.
    // Access byte = 0x92:
    // - Present: 1
    // - Privilege level: 0
    // - Executable: 0 (it's a data segment)
    // - Writable: 1
    // Granularity = same as above (0xCF)
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    // Load the GDT into the CPU and update the segment registers (cs, ds, ss, etc.).
    // This activates our new memory model.
    gdt_flush((uint32_t)&gdt_ptr);
}

// This function is actually redundant in my setup because I already load the GDT in my gdt_flush.asm file.
// But I kept it here just in case I ever want to do the GDT loading entirely in C using inline assembly.
void gdt_load(struct gdt_ptr *gdt_ptr) {
    asm volatile("lgdt %0" : : "m" (*gdt_ptr));
}

// This function sets up each individual GDT entry.
// It takes a segment number (0 for null, 1 for code, 2 for data), a base address, a limit, and the access and granularity flags.
void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    // Split the base address into its 3 components for the descriptor format.
    gdt[num].base_low    = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high   = (base >> 24) & 0xFF;

    // Same for the limit: lower 16 bits go directly, upper 4 go into granularity.
    gdt[num].limit_low   = (limit & 0xFFFF);
    gdt[num].granularity = (limit >> 16) & 0x0F;

    // Set the high nibble of granularity for flags (4KB granularity, 32-bit, etc.).
    gdt[num].granularity |= gran & 0xF0;

    // Finally, set the access byte (present bit, permissions, etc.).
    gdt[num].access = access;
}
