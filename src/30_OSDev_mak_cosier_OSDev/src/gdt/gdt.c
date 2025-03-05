
#include "libc/gdt.h"

//#define NUM_GDT_ENTRIES 5



 
struct gdt_entry_struct gdt_entries[5];
struct gdt_ptr_struct   gdt_ptr;
  


 
void init_gdt()
{
    
    gdt_ptr.limit = (sizeof(struct gdt_entry_struct) * 5) - 1;
    gdt_ptr.base  = (uint32_t)&gdt_entries;
    
    
    gdt_set_gate(0, 0, 0, 0, 0);

    
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);

    
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

    gdt_flush((uint32_t)&gdt_ptr);
}


 
 
void gdt_set_gate(int32_t idx, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    gdt_entries[idx].base_low    = (base & 0xFFFF);
    gdt_entries[idx].base_middle = (base >> 16) & 0xFF;
    gdt_entries[idx].base_high   = (base >> 24) & 0xFF;

    gdt_entries[idx].limit_low   = (limit & 0xFFFF);
    
    gdt_entries[idx].granularity = (uint8_t)((limit >> 16) & 0x0F);

    gdt_entries[idx].granularity |= (gran & 0xF0);

    gdt_entries[idx].access = access;
}


