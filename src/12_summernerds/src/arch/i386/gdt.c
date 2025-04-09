#include "../src/arch/i386/gdt.h"
#include "libc/stdint.h"



//////////////////////////////////100%gp  fra nå...
//gdt.c

extern void gdt_flush(uint32_t);


void init_gdt();


void set_gdt_gate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity);

GDTEntry gdt_entries[5];    // A array with 5 GDT segments usually:(Null, Code, Dat, User-cod, User-data). (Håndeterer minnesegmentering og definerer kode/data segmenter)
GDTPointer  gdt_ptr;           // A pointer to the completed GDT that will be loaded

extern GDTPointer idt_ptr;

//////////////////////////////////... til nå


// GDT-pointer-struktur
struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));


void flush_gdt() {
    __asm__ volatile 
    (
        "lgdt (%0)\n"        // Last GDT fra gdt_descriptor
        "mov %%cr0, %%eax\n" // Hent CR0 til EAX
        "or $1, %%eax\n"     // Sett PE-bit (bit 0)
        "mov %%eax, %%cr0\n" // Skriv tilbake for å aktivere protected mode
        :
        : "r" (&gdt_ptr)
        : "eax"
    );
}


void init_desc_tables() 
{
    init_gdt();
}


void init_gdt()
{
    gdt_ptr.limit = (sizeof(struct GDTEntry) * 5)-1;
    gdt_ptr.base = (uint32_t)gdt_entries;

    set_gdt_gate(0,0,0,0,0);                    //NULL segment 
    
    set_gdt_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Index is 1, its going to start wirh a base 0 , 
                                                //the limit is going to be 8 f's, its defining the limit of a segment. 
                                                //The access is 0x9A, and the granuality is 0xCF    //code segment

    set_gdt_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
    set_gdt_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
    set_gdt_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment

    gdt_flush((uint32_t)&gdt_ptr);

}

// Setter en oppføring i GDT-tabellen
void set_gdt_gate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
    gdt_entries[num].base_low    = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high   = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low   = (limit & 0xFFFF);
    gdt_entries[num].granularity = (limit >> 16) & 0x0F;

    gdt_entries[num].granularity = granularity & 0xF0;
    gdt_entries[num].access      = access;
}
