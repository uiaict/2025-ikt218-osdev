#include "gdt.h"
#include "libc/stdint.h"

// Assembly function to flush/load the GDT
extern void gdt_flush(uint32_t gdt_ptr);

static struct gdt_entry_t gdt[GDT_ENTRIES];
static struct gdt_ptr_t gdt_ptr;

void init_gdt()
{
    gdt_ptr.limit = sizeof(struct gdt_entry_t) * GDT_ENTRIES - 1;
    gdt_ptr.base = (uint32_t)&gdt;

    gdt_set_gate(0, 0, 0, 0, 0); // Null segment

    gdt_set_gate(1, 0, 0xFFFFFFFF, GDT_ACCESS_CODE_EXEC_READ, GDT_FLAG_USE_FLAT_MODEL);  // Kernel code
    gdt_set_gate(2, 0, 0xFFFFFFFF, GDT_ACCESS_DATA_READ_WRITE, GDT_FLAG_USE_FLAT_MODEL); // Kernel data

    gdt_set_gate(3, 0, 0xFFFFFFFF, GDT_ACCESS_USER_CODE_EXEC_READ, GDT_FLAG_USE_FLAT_MODEL);  // User code
    gdt_set_gate(4, 0, 0xFFFFFFFF, GDT_ACCESS_USER_DATA_READ_WRITE, GDT_FLAG_USE_FLAT_MODEL); // User data

    gdt_flush((uint32_t)&gdt_ptr);
}

void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    gdt[num].base_low = (base & 0xFFFF);                           // Lowest base 16 bits (0-15)
    gdt[num].base_middle = (base >> 16) & 0xFF;                    // Next base 8 bits(16-23)
    gdt[num].base_high = (base >> 24) & 0xFF;                      // Highest base 8 bits (24-31)
    gdt[num].limit_low = (limit & 0xFFFF);                         // Lowest limit 16 bits (0-15)
    gdt[num].granularity = ((limit >> 16) & 0x0F) | (gran & 0xF0); // Next limit 4 bits (16-19) and granularity

    gdt[num].access = access;
}
