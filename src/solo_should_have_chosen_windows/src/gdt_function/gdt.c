#include "gdt/gdt_structs.h"
#include "gdt/gdt_function.h"
#include "terminal/clear.h"
#include "libc/string.h"
#include "libc/stdint.h"
#include "libc/stddef.h"

// Define the size of the GDT
#define GDT_SIZE 3

// Define the GDT
struct gdt_entry gdt[GDT_SIZE];
struct gdt_ptr gdt_ptr;

// Function to write a character to video memory (for visual feedback)
static void write_char(int x, int y, char c, uint8_t attr) {
    uint16_t *video_memory = (uint16_t*)0xB8000;
    int offset = (y * 80) + x;
    video_memory[offset] = (attr << 8) | c;
}

// Function to write a string to video memory
static void write_string(int x, int y, const char* str, uint8_t attr) {
    int i = 0;
    while (str[i] != '\0') {
        write_char(x + i, y, str[i], attr);
        i++;
    }
}

// Function to set a GDT entry
static void gdt_set_entry(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
    gdt[index].base_low = (base & 0xFFFF);
    gdt[index].base_middle = (base >> 16) & 0xFF;
    gdt[index].base_high = (base >> 24) & 0xFF;

    gdt[index].limit_low = (limit & 0xFFFF);
    gdt[index].granularity = (limit >> 16) & 0x0F;  // Upper limit bits
    gdt[index].granularity |= granularity & 0xF0;  // Granularity flags

    gdt[index].access = access;
}

// Load GDT using inline assembly
void gdt_load(struct gdt_ptr* gp) {
    __asm__ __volatile__ (
        "lgdt %0;"         // Load GDT using the pointer
        "mov $0x10, %%ax;"   // 0x10 is offset to data segment
        "mov %%ax, %%ds;"    // Load all data segment selectors
        "mov %%ax, %%es;"
        "mov %%ax, %%fs;"
        "mov %%ax, %%gs;"
        "mov %%ax, %%ss;"
        "ljmp $0x08, $1f;"   // Far jump to update CS register (0x08 is code segment)
        "1:"                 // Local label for jump target
        : : "m" (*gp) : "memory", "eax"
    );
}

// Function to initialize the GDT
void gdt_init() {

    memset(&gdt, 0, sizeof(gdt)); // Clear GDT memory

    // Clear screen (first few lines)
    clearTerminal();

    // Display initial message
    write_string(0, 0, "Setting up GDT...", 0x0F); // White text on black

    // Setup GDT pointer
    gdt_ptr.limit = (sizeof(struct gdt_entry) * GDT_SIZE) - 1;
    gdt_ptr.base = (uint32_t)&gdt;

    // NULL descriptor (must be all zeroes)
    gdt_set_entry(0, 0, 0, 0, 0);
    write_string(0, 1, "- NULL descriptor set", 0x07);

    // Code Segment descriptor (execute/read)
    gdt_set_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // 0x9A = Present, Ring 0, Code segment
    write_string(0, 2, "- Code segment set", 0x07);
    
    // Data Segment descriptor (read/write)
    gdt_set_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // 0x92 = Present, Ring 0, Data segment
    write_string(0, 3, "- Data segment set", 0x07);

    // Load GDT
    write_string(0, 4, "Loading GDT...", 0x0E); // Yellow text
    gdt_load(&gdt_ptr);
    
    // If execution reaches here, GDT loading was successful
    write_string(0, 4, "GDT loaded successfully!", 0x0A); // Green text for success

    clearTerminal();
}
