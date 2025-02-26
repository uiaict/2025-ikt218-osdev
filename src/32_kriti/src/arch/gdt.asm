#include <stdint.h>

// Define the GDT entries
struct GDTEntry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed));

struct GDTDescriptor {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

struct GDTEntry gdt[] = {
    {0, 0, 0, 0, 0, 0},                              // Null segment
    {0xFFFF, 0, 0, 0x9A, 0xCF, 0},                   // Code segment
    {0xFFFF, 0, 0, 0x92, 0xCF, 0}                    // Data segment
};

struct GDTDescriptor gdt_descriptor = {
    sizeof(gdt) - 1,
    (uint32_t) &gdt
};

void load_gdt() {
    asm volatile (
        "lgdt (%0)\n"
        "mov $0x10, %%ax\n"
        "mov %%ax, %%ds\n"
        "mov %%ax, %%es\n"
        "mov %%ax, %%fs\n"
        "mov %%ax, %%gs\n"
        "mov %%ax, %%ss\n"
        "jmp $0x08, $.flush\n"
        ".flush:\n"
        "mov $0x10, %%ax\n"
        "ret"
        :
        : "r"(&gdt_descriptor)
        : "ax"
    );
}
