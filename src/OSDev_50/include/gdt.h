#ifndef GDT_H
#define GDT_H


#include <stdint.h>

struct gdt_entry_struct

struct gdt_entry_t {
    uint16_t limit_low;
    uint16_t base_low;
    unit8_t base_middle;
    unit8_t access;
    unit8_t flags;
    unit8_t base_high;
}__attribute__((packed))

struct gdt_entry_struct{
    uint16_t limit;
    unsigned int base; 
}__attribute__((packed));

void init_gdt(void);

#endif

