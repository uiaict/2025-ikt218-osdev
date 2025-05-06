#include "i386/descriptorTables.h"

//////////////////////////////////100%gp  fra nå...
// gdt.c


extern void gdt_flush(uint32_t gdt_ptr);

void init_gdt()
{
    gdt_ptr.limit = (sizeof(struct GDTEntry) * 5) - 1;
    gdt_ptr.base = (uint32_t)&gdt;

    set_gdt_gate(0, 0, 0, 0, 0); // NULL segment

    set_gdt_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Index is 1, its going to start wirh a base 0 ,
                                                // the limit is going to be 8 f's, its defining the limit of a segment.
                                                // The access is 0x9A, and the granuality is 0xCF    //code segment

    set_gdt_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
    set_gdt_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
    set_gdt_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment

    gdt_flush((uint32_t)&gdt_ptr);
}

// Setter en oppføring i GDT-tabellen
void set_gdt_gate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity)
{
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = (limit >> 16) & 0x0F;

    gdt[num].granularity = granularity & 0xF0;
    gdt[num].access = access;
}
