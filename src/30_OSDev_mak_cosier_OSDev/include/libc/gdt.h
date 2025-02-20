
#ifndef GDT_H
#define GDT_H
#include "stdint.h"

 // Each GDT entry is 8 bytes long and describes either a code/data segment
 // or a system segment (LDT, TSS, etc.). We use 'packed' to prevent structure
 // padding by the compiler.
 
struct gdt_entry_struct {
    uint16_t limit_low;   // The lower 16 bits of the limit.
    uint16_t base_low;    // The lower 16 bits of the base.
    uint8_t  base_middle; // The next 8 bits of the base.
    uint8_t  access;      // Access flags (ring, present bit, read/write, etc.).
    uint8_t  granularity; // Granularity, 32/16-bit mode, page gran, etc.
    uint8_t  base_high;   // The last 8 bits of the base.
} __attribute__((packed));
//typedef struct gdt_entry_struct gdt_entry_t;


 // The GDTR (GDT Register) pointer structure, which tells the CPU the base
 // and limit of our GDT.
 
struct gdt_ptr_struct {
    uint16_t limit; // Size of the GDT minus one.
    uint32_t base;  // Base address of the first gdt_entry_t struct.
} __attribute__((packed));
//typedef struct gdt_ptr_struct gdt_ptr_t;


 // We declare this function in an assembly file (e.g. gdt_flush.asm).
 // It loads our new GDT using the lgdt instruction and then does a far jump
 // to reset CS, DS, ES, etc.
 extern void gdt_flush(uint32_t gdt_ptr);  


 // Initialize our GDT:
 //  - Set up five descriptors (Null, Kernel Code, Kernel Data,
 //    User Code, User Data).
 //  - Call gdt_flush to tell the CPU to use our GDT.
 
void init_gdt();
void gdt_set_gate(int32_t idx, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);

#endif 
