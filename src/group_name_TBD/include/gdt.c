#include "libc/stdint.h"
#include "gdt.h"

struct gdt_entry gdt[GDT_ENTRIES];
struct gdt_ptr gdt_ptr;

void init_gdt(){
    
    // Store base and limiyt properties for gdt[]
    gdt_ptr.limit = sizeof(struct gdt_entry) * GDT_ENTRIES - 1;
    gdt_ptr.base = (uint32_t)&gdt;

    // num, base, limit, access, granularity
    gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment     9A, CF = 1001 1010, 1100 1111
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment     92, CF = 1001 0010, 1100 1111
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment, [[maybe_unused]]
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment, [[maybe_unused]] 		

    // Load the GDT
    gdt_load(&gdt_ptr);

    // Flush GDT pointer
    gdt_flush((uint32_t)&gdt_ptr); 
}



void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran){
    
    gdt[num].base_low = (base & 0xFFFF); // Exctract first 16bit from base with bitwise AND. 0xFFFF = 2^16
    gdt[num].base_middle = (base >> 16) & 0xFF; // Shift away the first 16bit, extract "first" 8bit from base
    gdt[num].base_high = (base >> 24) & 0xFF; // Shift away the first 24bit, extract "first" 8bit from base

    gdt[num].limit_low = (limit & 0xFFFF); // Exctract first 16bit from limit with bitwise AND. 0xFFFF = 2^16
    gdt[num].granularity = (limit >> 16) & 0x0F; // ??Sets granularity to 0x00??

    gdt[num].granularity |= gran & 0xF0; // ??adds 0 to end of gran? bitwise OR with granularity pointless as it is 0x00??
    gdt[num].access = access;
}

void gdt_load(struct gdt_ptr *gdt_ptr){
    // inline asm is implicitly volatile
    asm volatile("lgdt %0" : : "m"(*gdt_ptr)); // Runs asm: L(oad)GDT
}