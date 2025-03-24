#ifndef IDT_STRUCTS_H
#define IDT_STRUCTS_H

#include "libc/stdint.h"

struct idt_entry {
    uint16_t offset_low;    // Lower 16 bits of the handler function address
    uint16_t selector;      // Segment selector reference in GDT
    uint8_t zero;           // Reserved, set to 0
    uint8_t type_attr;      // Descriptor type (e.g. present, ring level, gate type)
    uint16_t offset_high;   // Higher 16 bits of the handler function address
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;         // Size of the IDT table - 1
    uint32_t base;          // Base address of the IDT table
} __attribute__((packed));

#endif // IDT_STRUCTS_H 