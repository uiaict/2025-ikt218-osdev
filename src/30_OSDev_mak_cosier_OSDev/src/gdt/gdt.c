
#include "libc/gdt.h"

//#define NUM_GDT_ENTRIES 5


// We'll keep everything static so it doesn't leak outside this file.
 
struct gdt_entry_struct gdt_entries[5];
struct gdt_ptr_struct   gdt_ptr;
  

// Forward declaration: helper to fill a GDT entry. 


 // init_gdt:
 // 1) Fill in the gdt_ptr (limit and base).
 // 2) Initialize each GDT entry (5 total).
 // 3) Call gdt_flush (assembly) to load the GDTR register.
 
void init_gdt(){
    // The limit is just "size of 5 entries minus 1".
    gdt_ptr.limit = (sizeof(struct gdt_entry_struct) * 5) - 1;
    gdt_ptr.base  = (uint32_t)&gdt_entries;
    
    // 1) Null segment — must be present and all zeroes.
    gdt_set_gate(0, 0, 0, 0, 0);

    // 2) Kernel Code segment (base=0, limit=0xFFFFFFFF)
    //    access = 0x9A  (10011010b) => Present, Ring 0, Code segment
    //    gran   = 0xCF  (11001111b) => 32-bit mode, 4KB granularity
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    // 3) Kernel Data segment (base=0, limit=0xFFFFFFFF)
    //    access = 0x92  (10010010b) => Present, Ring 0, Data segment
    //    gran   = 0xCF  (same as above)
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    // 4) User Code segment (base=0, limit=0xFFFFFFFF)
    //    access = 0xFA  => Present, Ring 3, Code segment
    //    gran   = 0xCF  => 32-bit, 4KB
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);

    // 5) User Data segment (base=0, limit=0xFFFFFFFF)
    //    access = 0xF2  => Present, Ring 3, Data segment
    //    gran   = 0xCF
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

    // Load the new GDT (this is an assembly function defined elsewhere).
    gdt_flush((uint32_t)&gdt_ptr);
}


 // gdt_set_gate: helper function to fill in one GDT entry.
 
void gdt_set_gate(int32_t idx, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran){
    gdt_entries[idx].base_low    = (base & 0xFFFF);
    gdt_entries[idx].base_middle = (base >> 16) & 0xFF;
    gdt_entries[idx].base_high   = (base >> 24) & 0xFF;

    gdt_entries[idx].limit_low   = (limit & 0xFFFF);
    gdt_entries[idx].granularity = (uint8_t)((limit >> 16) & 0x0F);

    // Set granularity flags (4KB pages, 32-bit protected mode)
    gdt_entries[idx].granularity |= (gran & 0xF0);
    // Set access flags (present bit, ring level, code/data, read/write, etc.)
    gdt_entries[idx].access = access;
}
