#include "gdt.h"

/* Three descriptors: null, kernel code, kernel data */
static struct gdt_entry gdt[3];
static struct gdt_ptr  gp;

/* External assembly helper (see gdt.asm) */
extern void gdt_flush(uint32_t gp_addr);

/* -----------------------------------------------------------------------
 * Encode a single 8-byte descriptor
 * --------------------------------------------------------------------- */
static void set_gate(int i, uint32_t base, uint32_t limit,
                     uint8_t access, uint8_t gran)
{
    gdt[i].base_low    =  base        & 0xFFFF;
    gdt[i].base_mid    = (base >> 16) & 0xFF;
    gdt[i].base_high   = (base >> 24) & 0xFF;

    gdt[i].limit_low   =  limit       & 0xFFFF;
    gdt[i].granularity = (limit >> 16) & 0x0F;

    gdt[i].granularity |= gran & 0xF0;
    gdt[i].access       = access;
}

/* -----------------------------------------------------------------------
 * Public interface
 * --------------------------------------------------------------------- */
void gdt_init(void)
{
    /* Null descriptor — must be first */
    set_gate(0, 0, 0, 0, 0);

    /* 0x08: ring-0 32-bit code segment, base 0, limit 4 GiB */
    set_gate(1, 0, 0xFFFFF, 0x9A, 0xCF);

    /* 0x10: ring-0 32-bit data segment, base 0, limit 4 GiB */
    set_gate(2, 0, 0xFFFFF, 0x92, 0xCF);

    gp.limit = sizeof(gdt) - 1;
    gp.base  = (uint32_t)&gdt;

    /* Load GDTR + reload segment registers (assembly) */
    gdt_flush((uint32_t)&gp);
}
