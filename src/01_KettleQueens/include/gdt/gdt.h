#ifndef GDT_H
#define GDT_H

#include "libc/stdint.h"

void gdt_init();                 // Initializes the GDT
extern void gdt_flush(uint32_t); // Defined in assembly, loads GDT and reloads segments

#endif
