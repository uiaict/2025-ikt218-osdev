// gdt.h
#ifndef GDT_H
#define GDT_H

#include <libc/stdint.h>

extern void gdt_flush(uint32_t gdt_ptr);

void init_gdt();

void gdt_set_gate(uint32_t , uint32_t , uint32_t , uint8_t , uint8_t );
#endif
