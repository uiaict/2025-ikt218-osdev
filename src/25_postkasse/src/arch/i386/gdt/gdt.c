#include "gdt.h"

//Defines the number of entries in the GDT
#define GDT_ENTRIES 5 //null, code, data, user code, user data

//array of 5 GDT entries
struct gdt_entry gdt[GDT_ENTRIES];

//struct to hold the addess of the GDT and its size
struct gdt_ptr gdt_ptr;

void init_gdt() {
    // Set the GDT limit
    //Total size of GDT -1
    gdt_ptr.limit = sizeof(struct gdt_entry) * GDT_ENTRIES - 1;

    //Holds the memory address where the GDT array starts
    gdt_ptr.base = (uint32_t) &gdt;

    //Each call to gdt_set_gate configures one segment in the GDT
    // num, base, limit, access, granularity
    gdt_set_gate(0, 0, 0, 0, 0); // Null segment, unused, always set to zero
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Kernel code segment, stats at address 0, size 4GB, access is executablem readable and in kernel mode
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Kernel Data segment, similar to code segment but with different access value.
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment

    //Loads GDT pointer into CPU register
    gdt_load(&gdt_ptr);

    // Calls an external function in assembly(gdt.asm), to flush and update CPU GDT register
    gdt_flush((uint32_t)&gdt_ptr);
}

//Ensures CPU knows where new GDT is and what its size is.
//insert the lgdt instruction, using the memory pointed to by gdt_ptr
void gdt_load(struct gdt_ptr *gdt_ptr) {
    asm volatile("lgdt %0" : : "m" (*gdt_ptr));
}

// Set a GDT entry at index 'num'
void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    // Split the 32 bit base into 3 parts
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    //Split the limit into lower 16 bits and upper 4 bits
    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = (limit >> 16) & 0x0F;

    //Add granularity flags
    gdt[num].granularity |= gran & 0xF0;

    // Set the access flags.
    gdt[num].access = access;
}
