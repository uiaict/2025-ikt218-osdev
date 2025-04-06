#pragma once
#include "libc/stdint.h"

#define GDT_ENTRIES 5

// GDT segment selectors
typedef enum
{
    GDT_NULL_SELECTOR = 0x00,
    GDT_KERNEL_CODE_SELECTOR = 0x08, // Entry 1 << 3
    GDT_KERNEL_DATA_SELECTOR = 0x10, // Entry 2 << 3
    GDT_USER_CODE_SELECTOR = 0x18,   // Entry 3 << 3
    GDT_USER_DATA_SELECTOR = 0x20    // Entry 4 << 3
} gdt_selector_t;

// GDT access flags
typedef enum
{
    GDT_ACCESS_CODE_EXEC_READ = 0x9A,      // 10011010b
    GDT_ACCESS_DATA_READ_WRITE = 0x92,     // 10010010b
    GDT_ACCESS_USER_CODE_EXEC_READ = 0xFA, // 11111010b
    GDT_ACCESS_USER_DATA_READ_WRITE = 0xF2 // 11110010b
} gdt_access_t;

// GDT granularity and size flags
typedef enum
{
    GDT_FLAG_GRANULARITY_1B = 0x00,
    GDT_FLAG_GRANULARITY_4K = 0x80,
    GDT_FLAG_MODE_16_BIT = 0x00,
    GDT_FLAG_MODE_32_BIT = 0x40,
    GDT_FLAG_USE_FLAT_MODEL = GDT_FLAG_GRANULARITY_4K | GDT_FLAG_MODE_32_BIT
} gdt_flags_t;

// GDT entry structure (8 bytes)
struct gdt_entry_t
{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed));

// GDTR structure
struct gdt_ptr_t
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

// Public interface
void init_gdt();
void gdt_load();
void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);

// GDT table and pointer (used in .c file)
static struct gdt_entry_t gdt[GDT_ENTRIES];
static struct gdt_ptr_t gdt_ptr;
