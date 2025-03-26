#include "gdt_idt_table.h"
#include "isr.h"
#include "libc/stdint.h"
#include "common.h"

gdt_entry_t gdt_entries[5];
gdt_ptr_t gdt_ptr;


extern void gdt_flush(uint32_t gdt_ptr);

static void init_gdt();

static void gdt_set_gate(uint32_t , uint32_t , uint32_t , uint8_t , uint8_t );



static void init_gdt()
{
    gdt_ptr.limit = (sizeof(gdt_entry_t)* 5)-1;
    gdt_ptr.base = (uint32_t)&gdt_entries;

    gdt_set_gate(0, 0, 0, 0, 0); 
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);  
    gdt_flush((uint32_t) &gdt_ptr);

}
static void gdt_set_gate(uint32_t num , uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    gdt_entries[num].base_low = (base & 0xFFFF);
    gdt_entries[num].base_middle = ((base >> 16)& 0xFF);
    gdt_entries[num].base_high = ((base >> 24) & 0xFF);

    gdt_entries[num].limit_low = (base & 0xFFFF);
    gdt_entries[num].granularity  = ((base >> 16) & 0xF);

    gdt_entries[num].granularity  |= (gran & 0xF);
    gdt_entries[num].access = access;
}