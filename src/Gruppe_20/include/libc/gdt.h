#ifndef GDT_H
#define GDT_H

#include "libc/stdint.h"
#include "gdt_idt_table.h"

#ifdef __cplusplus
extern "C" {
#endif

// Access Byte Flags
#define GDT_ACCESS_PRESENT        (1 << 7)
#define GDT_ACCESS_PRIVILEGE_RING0 (0 << 5)
#define GDT_ACCESS_PRIVILEGE_RING3 (3 << 5)
#define GDT_ACCESS_EXECUTABLE    (1 << 3)
#define GDT_ACCESS_READWRITE     (1 << 1)
#define GDT_ACCESS_DC            (1 << 2)  // Direction/Conforming bit

// Granularity Flags
#define GDT_GRAN_4KB             (1 << 3)
#define GDT_GRAN_32BIT           (1 << 2)

// Segment Selectors
#define KERNEL_CS 0x08
#define KERNEL_DS 0x10
#define USER_CS   0x18
#define USER_DS   0x20

// Initialize GDT
void init_gdt();

// Set individual GDT entry
void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);

#ifdef __cplusplus
}
#endif

#endif // GDT_H