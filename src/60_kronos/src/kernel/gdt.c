#include "kernel/gdt.h"

extern void gdt_flush(addr_t);

struct gdt_entry_struct gdt_entries[5];
struct gdt_ptr_struct gdt_ptr;

void gdt_init() {
    gdt_ptr.limit = (sizeof(struct gdt_entry_struct) * 5) - 1;
    gdt_ptr.base = &gdt_entries;


    // Segments
    gdt_set_gate(0, 0, 0, 0, 0);                 // Null
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);  // Kernel code
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);  // Kernel data
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);  // User code
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);  // User data

    gdt_flush(&gdt_ptr);
}

void gdt_set_gate(uint32_t index, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
    gdt_entries[index].base_low = (base & 0xFFFF);
    gdt_entries[index].base_middle = (base >> 16) & 0xFF;
    gdt_entries[index].base_high = (base >> 24) & 0xFF;
    gdt_entries[index].limit = (limit & 0xFFFF);
    gdt_entries[index].granularity = (limit >> 16) & 0x0F;
    gdt_entries[index].granularity |= (granularity & 0xF0);
    gdt_entries[index].access = access;
}