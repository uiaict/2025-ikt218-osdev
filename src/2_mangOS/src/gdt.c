#include "gdt.h"
#include "libc/stdint.h"

// Descriptor Privilege Level (DPL) flags.
#define GDT_CODE_EXEC_READ 0x9A // Binary: 10011010

// Readable and writable data segment, not accessed, with Ring 0 privilege.
#define GDT_DATA_READ_WRITE 0x92 // Binary: 10010010

// Descriptor Privilege Level (DPL) flags.
#define GDT_FLAG_RING0 0x00 // Ring 0: Highest level of privilege.
#define GDT_FLAG_RING3 0x60 // Ring 3: Lowest level, used for user-space applications.

// Granularity and operation size flags.
#define GDT_GRANULARITY_4K 0x80 // Granularity: 1 = 4KB, 0 = 1 byte.
#define GDT_32_BIT_MODE 0x40    // 32-bit opcode size

struct gdt_entry gdt_entries[5];
struct gdt_ptr gdt_ptr;

struct gdt_entry create_gdt_entry(uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity)
{
    struct gdt_entry entry;

    // Bits 0-15
    entry.base_low = base & 0xFFFF;
    // Bits 16-23
    entry.base_middle = (base >> 16) & 0xFF;
    // Bits 24-31
    entry.base_high = (base >> 24) & 0xFF;
    // Lowest 16 bits
    entry.limit_low = limit & 0xFFFF;

    // Upper 4 bits = granularity flags
    // Lower 4 bits = high bits of the segment limit
    entry.granularity = ((limit >> 16) & 0x0F) | (granularity & 0xF0);

    // This byte contains several bits (present bit, read/write, executable bit, ...)
    entry.access = access;
    return entry;
}

void init_gdt()
{
    gdt_entries[0] = create_gdt_entry(0, 0, 0, 0);

    gdt_entries[1] = create_gdt_entry(0, 0xFFFFFFFF, GDT_CODE_EXEC_READ, GDT_FLAG_RING0 | GDT_GRANULARITY_4K | GDT_32_BIT_MODE);

    // Entry 2: Kernel mode data segment (Ring 0, readable/writable)
    gdt_entries[2] = create_gdt_entry(0, 0xFFFFFFFF, GDT_DATA_READ_WRITE, GDT_FLAG_RING0 | GDT_GRANULARITY_4K | GDT_32_BIT_MODE);

    // Entry 3: User mode code segment (Ring 3, executable, readable)
    gdt_entries[3] = create_gdt_entry(0, 0xFFFFFFFF, GDT_CODE_EXEC_READ, GDT_FLAG_RING3 | GDT_GRANULARITY_4K | GDT_32_BIT_MODE);

    // Entry 4: User mode data segment (Ring 3, readable/writable)
    gdt_entries[4] = create_gdt_entry(0, 0xFFFFFFFF, GDT_DATA_READ_WRITE, GDT_FLAG_RING3 | GDT_GRANULARITY_4K | GDT_32_BIT_MODE);

    gdt_ptr.limit = sizeof(gdt_entries) - 1;
    gdt_ptr.base = (uint32_t)&gdt_entries;

    load_gdt();
}