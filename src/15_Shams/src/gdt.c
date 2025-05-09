#include <libc/gdt.h>

// This function tells the processor where the GDT is located in memory
extern void gdt_flush(uint32_t gdt_ptr);

// This function fills in the info for one "box" in the GDT, telling it where the memory starts,
// how much memory it can access, and what permissions it has
void gdt_set_gate(int32_t index, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
    
    // Set the starting address (base) of this segment
    gdt[index].base_low = base & 0xFFFF;           // Take the lowest 16 bits of the base address
    gdt[index].base_middle = (base >> 16) & 0xFF;  // Take the next 8 bits of the base address
    gdt[index].base_high = (base >> 24) & 0xFF;    // Take the highest 8 bits of the base address
    
    // Set how big this segment is (the segment limit)
    gdt[index].limit_low = limit & 0xFFFF;         // Take the lowest 16 bits of the limit
    gdt[index].granularity = (limit >> 16) & 0x0F; // Take the next 4 bits of the limit
    
    // Set how this segment works (size and permissions)
    gdt[index].granularity |= granularity & 0xF0;  // Set the size and granularity settings
    gdt[index].access = access;                    // Set the permissions for this segment (who can access it and how)
}

// This function sets up the GDT and tells the CPU to use it
void init_gdt() {
    
    // Calculate the size of the GDT (how many bytes it needs)
    gdt_ptr.limit = (sizeof(struct gdt_entry_t) * GDT_ENTRIES) - 1;
    
    // Set the location of the GDT in memory
    gdt_ptr.base = (uint32_t)&gdt;

    // Set up the entries (segments) inside the GDT.

    // Entry 0: Null segment (must be empty, always included in the GDT).
    gdt_set_gate(0, 0, 0, 0, 0);  // Always required and must be zero
    
    // Entry 1: Kernel code segment (used to run the main system code).
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);  // Code that can access the entire memory
    
    // Entry 2: Kernel data segment (used to store and access system data).
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);  // Data that can use the entire memory
    
    // Entry 3: User mode code segment (used to run programs created by users).
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);  // Code run by user programs
    
    // Entry 4: User mode data segment (used to store data for user programs).
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);  // Data for user programs
    
    // Tell the CPU to start using the new GDT.
    gdt_flush((uint32_t)&gdt_ptr);
}
