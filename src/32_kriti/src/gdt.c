#include <libc/stdint.h>
#include <gdt.h>

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

// Declare GDT entries - without initialization // CHANGED
struct GDTEntry gdt[3]; // CHANGED - removed initialization
struct GDTDescriptor gdt_descriptor; // CHANGED - removed initialization

// Improved version of load_gdt using better inline assembly techniques // CHANGED
void load_gdt() {
    // CHANGED - Improved inline assembly with better constraints and memory clobber
    asm volatile (
        "lgdt %0\n"                // CHANGED - Use memory operand instead of register
        "mov $0x10, %%ax\n"
        "mov %%ax, %%ds\n"
        "mov %%ax, %%es\n"
        "mov %%ax, %%fs\n"
        "mov %%ax, %%gs\n"
        "mov %%ax, %%ss\n"
        "ljmp $0x08, $gdt_flush\n"  // CHANGED - Use a global label instead of local label
        "gdt_flush:\n"              // CHANGED - Global label is more reliable
        :: "m"(gdt_descriptor)      // CHANGED - Use memory constraint
        : "ax", "memory"            // CHANGED - Added memory clobber
    );
}

void init_gdt() {
    // Null segment
    gdt[0].limit_low = 0;
    gdt[0].base_low = 0;
    gdt[0].base_middle = 0;
    gdt[0].access = 0;
    gdt[0].granularity = 0;
    gdt[0].base_high = 0;
    
    // Code segment
    gdt[1].limit_low = 0xFFFF;
    gdt[1].base_low = 0;
    gdt[1].base_middle = 0;
    gdt[1].access = 0x9A;    // Present, Ring 0, Code, Executable, Readable
    gdt[1].granularity = 0xCF; // 4KB granularity, 32-bit mode, limit bits 16-19
    gdt[1].base_high = 0;
    
    // Data segment
    gdt[2].limit_low = 0xFFFF;
    gdt[2].base_low = 0;
    gdt[2].base_middle = 0;
    gdt[2].access = 0x92;    // Present, Ring 0, Data, Writable
    gdt[2].granularity = 0xCF; // 4KB granularity, 32-bit mode, limit bits 16-19
    gdt[2].base_high = 0;
    
    // Set up the GDT descriptor
    gdt_descriptor.limit = sizeof(gdt) - 1;
    gdt_descriptor.base = (uint32_t)&gdt;
    
    // Load the GDT
    load_gdt();
}