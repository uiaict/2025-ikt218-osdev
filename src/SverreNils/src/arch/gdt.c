#include "gdt.h"

struct gdt_entry {
    uint16_t limit_low;     
    uint16_t base_low;      
    uint8_t  base_middle;   
    uint8_t  access;        
    uint8_t  granularity;   
    uint8_t  base_high;     
} __attribute__((packed));

struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

extern void gdt_load(struct gdt_ptr* gdt_descriptor); 

static struct gdt_entry gdt[3];
static struct gdt_ptr gdt_descriptor;

static void gdt_set_entry(int i, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
    gdt[i].limit_low = limit & 0xFFFF;
    gdt[i].base_low = base & 0xFFFF;
    gdt[i].base_middle = (base >> 16) & 0xFF;
    gdt[i].access = access;
    gdt[i].granularity = ((limit >> 16) & 0x0F) | (granularity & 0xF0);
    gdt[i].base_high = (base >> 24) & 0xFF;
}

void gdt_init() {
    gdt_set_entry(0, 0, 0, 0, 0);               
    gdt_set_entry(1, 0, 0xFFFFF, 0x9A, 0xCF);     
    gdt_set_entry(2, 0, 0xFFFFF, 0x92, 0xCF);     

    gdt_descriptor.limit = sizeof(gdt) - 1;
    gdt_descriptor.base = (uint32_t)&gdt;

    gdt_load(&gdt_descriptor);
}