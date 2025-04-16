#ifndef GDT_IDT_TABLE_H
#define GDT_IDT_TABLE_H

#include "libc/stdint.h"

struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
}__attribute__((packed));

typedef struct gdt_entry gdt_entry_t;


struct gdt_ptr
{
    uint16_t limit;
    uint32_t base;
}__attribute__((packed));

typedef struct gdt_ptr gdt_ptr_t;

struct idt_entry
{
    uint16_t base_low;
    uint16_t select;
    uint8_t always0;
    uint8_t flags;
    uint16_t base_high;
}__attribute__((packed));

typedef struct idt_entry idt_entry_t;


struct idt_ptr
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

typedef struct idt_ptr idt_ptr_t;

void gdt_idt_table();
#endif
