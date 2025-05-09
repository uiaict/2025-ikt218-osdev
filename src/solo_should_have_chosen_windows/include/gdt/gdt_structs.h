#ifndef GDT_H
#define GDT_H

#include "libc/stdint.h"


/* 
GDT ENTRY STRUCTURE

Base address is 32 bits, split amongst 3 fields
Limit is 20 bits, split amongst 2 fields
Access byte: 7 bits of access rights, 1 bit of granularity
Granularity byte: Used to determine whether the limit is in bytes or 4KB pages */
struct gdt_entry {
    uint16_t limit_low; 
    uint16_t base_low; 
    uint8_t base_middle; 
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high; 
} __attribute__((packed));

/* 
GDT POINTER
----------------
Contains the size of the GDT and the address of the first entry */

struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));



#endif // GDT_H