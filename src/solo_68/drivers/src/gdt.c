#include <stdint.h>
#include "gdt.h"

struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  base_high;
} __attribute__((packed));

struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

extern void gdt_flush(uint32_t);

static struct gdt_entry gdt[3];
static struct gdt_ptr gp;

static void set_gdt_entry(int i, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt[i].base_low = base & 0xFFFF;
    gdt[i].base_middle = (base >> 16) & 0xFF;
    gdt[i].base_high = (base >> 24) & 0xFF;

    gdt[i].limit_low = limit & 0xFFFF;
    gdt[i].granularity = (limit >> 16) & 0x0F;
    gdt[i].granularity |= gran & 0xF0;
    gdt[i].access = access;
}

void init_gdt() {
    gp.limit = (sizeof(struct gdt_entry) * 3) - 1;
    gp.base = (uint32_t)&gdt;

    set_gdt_entry(0, 0, 0, 0, 0);                // Null segment
    set_gdt_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
    set_gdt_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment

    gdt_flush((uint32_t)&gp);
}