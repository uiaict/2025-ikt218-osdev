#include "gdt.h"

extern void gdt_flush(uint32_t);

struct gdt_entry_struct gdt_entries[5];
struct gdt_ptr_struct gdt_ptr;

void initGdt() {
    gdt_ptr.limit = (sizeof(struct gdt_entry_struct) * 5 - 1);
    gdt_ptr.base = (unsigned)&gdt_entries;

    setGdtGate(0,0,0,0,0); // Null segment
    setGdtGate(1,0,0xFFFFFFFF,0x9A,0xCF); // Kernel Code Segment
    setGdtGate(2,0,0xFFFFFFFF,0x92,0xCF); // Kernel Data Segment
    setGdtGate(3,0,0xFFFFFFFF,0xFA,0xCF); // User Code Segment
    setGdtGate(4,0,0xFFFFFFFF,0xF2,0xCF); // User Data Segment

    gdt_flush((uint32_t)&gdt_ptr);
}

void setGdtGate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt_entries[num].base_low = (base & 0xFFFF); // Isolate lower 16 bits of base - 0-15
    gdt_entries[num].base_middle = (base >> 16) & 0xFF; // Isolate the middle 8 bits - 16-23
    gdt_entries[num].base_hight = (base >> 24) & 0xFF; // Isolate the top 8 bits - 24-32

    gdt_entries[num].limit = (limit & 0xFFFF); // Isolate the lower 16 bits - 0-15
    gdt_entries[num].flags = (limit >> 16) & 0x0F; // Set the flag to the lower limit first - middle 4 bits
    gdt_entries[num].flags |= (gran & 0xF0); // Sets the upper flag bits to the actual flag values

    gdt_entries[num].access_byte = access;
}