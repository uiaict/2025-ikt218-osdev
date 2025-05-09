// gdt.h
// This will prevent multiple inclusions of the header file during compilation to avoid redefinition errors.
#ifndef GDT_H
#define GDT_H

// GDT needs integer types for its structures as sizes must be exact.
#include "../libc/stdint.h"

// To keep things simple at first we will only define 3 GDT entries, the null descriptor, the code segment, and the data segment.
// This will be enough for a minimal kernel.
#define GDT_ENTRIES 3 

// An GDT entry is a 8 byte (or 64 bit) structure, which is divided into several fields.
struct gdt_entry {
    // The first 2 bytes are the limit low field, which is the lower 16 bits of the descriptor’s limit.
    uint16_t limit_low;
    // The next 2 bytes are the base low field, which is the lower 16 bits of the descriptor’s base address.
    uint16_t base_low;
    // The next byte is the base middle field, which is the next 8 bits of the base address.
    uint8_t  base_middle;
    // The next byte is the access byte, which contains flags defining who can access the memory referenced by the descriptor.
    uint8_t  access;
    // The next byte is for granularity, this was a little tricky to understand, but the first 4 bits are the upper 4 bits of the descriptor’s limit field.
    // The next 4 bits are 4 flags that influence the segment size.
    // One of these flags and probably the most vital one is the granularity bit (bit 7), which tells the CPU how to interpret the limit field:
    // - If set to 0, the limit is in bytes (max ~1MB)
    // - If set to 1, the limit is in 4KB blocks (max ~4GB)
    // For typical kernel segments, granularity is set to 1 to allow access to the full 4GB address space which is what i will do in my gdt.c file.
    uint8_t  granularity;
    // The last byte is the base high field, which is the upper 8 bits of the base address.
    uint8_t  base_high;
    // For this to be a valid GDT entry, the structure must be packed to prevent the compiler from adding padding bytes between the fields.
    // This is done using the __attribute__((packed)) directive.
} __attribute__((packed));

// The GDT pointer is a 6 byte structure, which contains the size of the GDT and the address of the GDT.
struct gdt_ptr {
    // The first 2 bytes are the size of the GDT, which is the size of the GDT entries minus 1.
    uint16_t limit;
    // The next 4 bytes are the address of the GDT, which is the address of the first GDT entry.
    uint32_t base;
    // For this to be a valid GDT pointer, the structure must also be packed to prevent the compiler from adding padding bytes between the fields.
} __attribute__((packed));

// Now that all the nessesary structures are defined, we can define the functions that will be used to initialize the GDT and load it into the CPU.
// These are explained in the gdt.c and gdt.asm files.
void init_gdt();
void gdt_load(struct gdt_ptr *gdt_ptr);
void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity);
void gdt_flush(uint32_t gdt_ptr); 

#endif
