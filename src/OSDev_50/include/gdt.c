#include <stdint.h>
#include "gdt.h"

#define NUM_GDT_ENTRIES 3

/* We declare our GDT entries and a GDT pointer */
static struct gdt_entry_struct gdt_entries[NUM_GDT_ENTRIES];
static struct gdt_ptr_struct   gdt_ptr;

/*
 * gdt_flush is an assembly function (defined in gdt.asm) that:
 *  1) Loads our GDT with 'lgdt'
 *  2) Performs a far jump to reload CS
 *  3) Updates DS, ES, FS, GS, SS
 */
extern void gdt_flush(uint32_t gdt_ptr_addr);

/*
 * Helper to set up one GDT entry
 */
static void setGate(uint32_t num,
                    uint32_t base,
                    uint32_t limit,
                    uint8_t access,
                    uint8_t gran)
{
    gdt_entries[num].base_low    = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high   = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low   = (limit & 0xFFFF);
    gdt_entries[num].granularity = ((limit >> 16) & 0x0F) | (gran & 0xF0);
    gdt_entries[num].access      = access;
}


void init_gdt(void)
{
    // Point the GDT pointer to our array
    gdt_ptr.limit = (sizeof(struct gdt_entry_struct) * NUM_GDT_ENTRIES) - 1;
    gdt_ptr.base  = (uint32_t)&gdt_entries;

    // 1. Null Descriptor (always zero)
    setGate(0, 0, 0, 0, 0);

    // 2. Kernel Code Segment
    //    - base = 0, limit = 0xFFFFF (4 GB in 4K pages)
    //    - access = 0x9A (present, ring 0, executable, readable)
    //    - gran   = 0xCF (4K granularity, 32-bit mode, limit high bits)
    setGate(1, 0, 0xFFFFF, 0x9A, 0xCF);

    // 3. Kernel Data Segment
    //    - base = 0, limit = 0xFFFFF
    //    - access = 0x92 (present, ring 0, data, writable)
    //    - gran   = 0xCF
    setGate(2, 0, 0xFFFFF, 0x92, 0xCF);

    // Load the new GDT and update segment registers
    gdt_flush((uint32_t)&gdt_ptr);
}
