#include "libc/stdint.h"
#include "gdt.h"

// GDT Entry (8 bytes)
struct gdt_entry {
    uint16_t limit_low;    // Limit bits 0-15
    uint16_t base_low;     // Base bits 0-15
    uint8_t  base_middle;  // Base bits 16-23
    uint8_t  access;       // Access flags
    uint8_t  granularity;  // Granularity 
    uint8_t  base_high;    // Base bits 24-31
} __attribute__((packed));

// GDT pointer (for lgdt)
struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

// Our GDT and pointer
struct gdt_entry gdt[3];      //we have 3 entries for gdt
struct gdt_ptr gdt_desc;      //pointer to table above

extern void gdt_flush(uint32_t);  // We'll define this in assembly


//this function is used to set the fields in a GDT entry.
//It looks complicated but it really just follows the specification
void gdt_set_entry(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    gdt[num].limit_low    = (limit & 0xFFFF);
    gdt[num].base_low     = (base & 0xFFFF);
    gdt[num].base_middle  = (base >> 16) & 0xFF;
    gdt[num].access       = access;
    gdt[num].granularity  = ((limit >> 16) & 0x0F) | (gran & 0xF0);
    gdt[num].base_high    = (base >> 24) & 0xFF;
}

//Initializing our table. So far everything runs in ring 0
void init_gdt() {
    gdt_set_entry(0, 0, 0, 0, 0);                // Null                (1-7)
    gdt_set_entry(1, 0, 0xFFFFF, 0x9A, 0xCF);    // Code: 0x08          (8-15)
    gdt_set_entry(2, 0, 0xFFFFF, 0x92, 0xCF);    // Data: 0x10          (16-23)

    gdt_desc.limit = sizeof(gdt) - 1;
    gdt_desc.base = (uint32_t)&gdt;
    
    //We are actually sending a pointer to our GDT table inside assembly.
    //In assembly we access this parameter through a stack
    gdt_flush((uint32_t)&gdt_desc);
}
