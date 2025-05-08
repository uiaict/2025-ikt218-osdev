#ifndef GDT_H
#define GDT_H

#include "libc/stdint.h"
#include "gdt_idt_table.h"

#ifdef __cplusplus
extern "C" {
#endif

// Initialize GDT
void init_gdt();

// Set individual GDT entry
void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);

#ifdef __cplusplus
}
#endif

#endif // GDT_H