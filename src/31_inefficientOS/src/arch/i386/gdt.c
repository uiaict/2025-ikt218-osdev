// src/arch/i386/gdt.c
#include "gdt.h"

#define GDT_ENTRIES 3  // Null, Kernel Code, Kernel Data

// GDT entry array and pointer
static struct gdt_entry gdt_entries[GDT_ENTRIES];
static struct gdt_ptr gdt_pointer;

// GDT access flags
#define GDT_PRESENT        0x80 // Must be 1 for valid selector
#define GDT_RING0          0x00 // Ring 0 (kernel)
#define GDT_RING3          0x60 // Ring 3 (user)
#define GDT_TYPE_CODE      0x1A // Code segment, readable
#define GDT_TYPE_DATA      0x12 // Data segment, writable

// GDT granularity flags
#define GDT_GRANULARITY    0x80 // Use 4KB granularity
#define GDT_32BIT          0x40 // 32-bit selector
#define GDT_16BIT          0x00 // 16-bit selector

static void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt_entries[num].base_low = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low = (limit & 0xFFFF);
    gdt_entries[num].granularity = ((limit >> 16) & 0x0F);

    gdt_entries[num].granularity |= gran & 0xF0;
    gdt_entries[num].access = access;
}

void gdt_init(void) {
    // Setup GDT pointer
    gdt_pointer.limit = (sizeof(struct gdt_entry) * GDT_ENTRIES) - 1;
    gdt_pointer.base = (uint32_t)&gdt_entries;

    // Null descriptor - required by CPU
    gdt_set_gate(0, 0, 0, 0, 0);

    // Kernel Code Segment
    gdt_set_gate(1, 0, 0xFFFFFFFF,
                 GDT_PRESENT | GDT_RING0 | GDT_TYPE_CODE,
                 GDT_GRANULARITY | GDT_32BIT);

    // Kernel Data Segment
    gdt_set_gate(2, 0, 0xFFFFFFFF,
                 GDT_PRESENT | GDT_RING0 | GDT_TYPE_DATA,
                 GDT_GRANULARITY | GDT_32BIT);

    // Load GDT and reload segments
    gdt_flush((uint32_t)&gdt_pointer);
}