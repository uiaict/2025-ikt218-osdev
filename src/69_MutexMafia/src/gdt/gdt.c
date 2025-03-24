#include "gdt.h"
#include "libc/stdint.h"

extern void gdt_flush(addr_t);

struct GDT gdt_entries[5];
struct GDT_ptr gdt_ptr;

void initGdt(){
    gdt_ptr.limit = (sizeof(struct GDT) * 5) - 1;
    gdt_ptr.base = (uint32_t)&gdt_entries;

    setGdtGate(0, 0, 0, 0, 0); // Null segment
    setGdtGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Kernel code
    setGdtGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Kernel data
    setGdtGate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User code
    setGdtGate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User data

    gdt_flush(&gdt_ptr); 

}
void setGdtGate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran){

    gdt_entries[num].lower_base = (base & 0xFFFF);
    gdt_entries[num].middle_base = (base >> 16) & 0xFF;
    gdt_entries[num].high_base = (base >> 24) & 0xFF;
    
    gdt_entries[num].limit = (limit & 0xFFFF);
    gdt_entries[num].flags = (limit >> 16) & 0x0F;
    gdt_entries[num].flags |= (gran & 0xF0);
    gdt_entries[num].access = access;
    
    
    }