#include "gdt.h"
#include "stdint.h"


extern void gdt_flush(uint32_t);

static void init_gdt();
static void gdt_set_gate(s32int,u32int,u32int,u8int,u8int);

static void init_gdt();
static void setGDTGate()uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity;


gdt_Ent_t gdt_entries[5];    // A array with 5 GDT segments usually:(Null, Code, Dat, User-cod, User-data). (Håndeterer minnesegmentering og definerer kode/data segmenter)
gdt_Ptr_t   gdt_ptr;           // A pointer to the completed GDT that will be loaded

idt_Ent_t idt_entries[256];  // An array with 256 IDT enteries(det er en tabell med 256 oppføringer), one for each interrupt handler, (er en datastruktur i x86-arkitektur, som brukes til å definere hvordan maskinvaren håndterer avbrudd og unntak).
idt_Ptr_t   idt_ptr;           // A pointer to the IDT tabell

void init_desc_tables() {
    init_gdt
}

void initGDT(){

    setGDTGate()
}

static void init_gdt(){
    gdt_ptr.limit = (sizeof(struct gdt_Ent_t) * 5)-1;
    gdt_Ptr_t.base = &gdt_entries;

    gdt_set_gate(0,0,0,0,0);  //NULL segment 
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Index is 1, its going to start wirh a base 0 , the limit is going to be 8 f's, its defining the limit of a segment. The access is 0x9A, and the granuality is 0xCF    //code segment
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment

    gdt_flush((uint32_t)&gdt_ptr);

}


static void setGDTGate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity){
    gdt_entries[num].base_low    = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high   = (base >> 24) & 0xFF;
 
    gdt_entries[num].limit_low   = (limit & 0xFFFF);
    gdt_entries[num].granularity = (limit >> 16) & 0x0F;
 
    gdt_entries[num].granularity |= gran & 0xF0;
    gdt_entries[num].access      = access;
}










