#include "gdt.h"

struct GdtEntry gdt_entries[3];
struct GdtPtr gdt_ptr;

static void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt_entries[num].base_low = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low = (limit & 0xFFFF);
    
    gdt_entries[num].granularity = ((limit >> 16) & 0x0F) | (gran & 0xF0);
    
    gdt_entries[num].access = access;
}

void init_gdt() {

    gdt_ptr.limit = (sizeof(struct GdtEntry) * 3) - 1;
    gdt_ptr.base = (uint32_t)&gdt_entries;

    gdt_set_gate(0, 0, 0, 0, 0);
    
  
    gdt_set_gate(1, 0, 0xFFFFF, 0x9A, 0xCF);
    
    
    gdt_set_gate(2, 0, 0xFFFFF, 0x92, 0xCF);
    
    gdt_flush((uint32_t)&gdt_ptr);
}