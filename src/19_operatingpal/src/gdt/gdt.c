#include "gdt/gdt.h"

struct GDT_entry gdt[3];
struct GDT_ptr GDT_descriptor;

extern void GDT_flush(uint32_t); 

static void GDT_set_entry(int i, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt[i].base_low    = (base & 0xFFFF);
    gdt[i].base_middle = (base >> 16) & 0xFF;
    gdt[i].base_high   = (base >> 24) & 0xFF;

    gdt[i].limit_low   = (limit & 0xFFFF);
    gdt[i].granularity = ((limit >> 16) & 0x0F) | (gran & 0xF0);
    gdt[i].access      = access;
}

void gdt_init() {
    GDT_set_entry(0, 0, 0, 0, 0);                
    GDT_set_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xA0); 
    GDT_set_entry(2, 0, 0xFFFFFFFF, 0x92, 0xA0); 

    GDT_descriptor.limit = (sizeof(struct GDT_entry) * 3) - 1;
    GDT_descriptor.base  = (uint32_t)&gdt;

    GDT_flush((uint32_t)&GDT_descriptor);
}
