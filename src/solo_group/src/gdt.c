#include "gdt.h"

// Defines theGDT and pointer
struct gdtEntry gdt[GDT_ENTRIES];
struct gdtPtr gp;

void initGdt() {
    // Set the GDT limit
    gp.limit = sizeof(struct gdtEntry) * GDT_ENTRIES - 1;
    gp.base = (uint32_t) &gdt;                  // Base addres of GDT array
    
    // Sets each GDT entry:
    gdtSetGate(0, 0, 0, 0, 0);                  // Null segment (required first entry)
    gdtSetGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);   // Code segment: ring 0, executable, readable
    gdtSetGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);   // Data segment: ring 0, writable
    gdtLoad(&gp);                               // Load the GDT
    gdtFlush((uint32_t)&gp);                    // Flush GDT pointer
}

void gdtLoad(struct gdtPtr *gpt) {
    asm volatile("lgdt %0" : : "m" (*gpt));     // Inline assembly
}

void gdtSetGate(int32_t num, uint32_t base, uint32_t limit, uint8_t access,
    uint8_t gran)
{
    // Set thebase address:
    gdt[num].baseLow = (base & 0xFFFF);
    gdt[num].baseMiddle = (base >> 16) & 0xFF;
    gdt[num].baseHigh = (base >> 24) & 0xFF;

    // Set segment limit (low + high):
    gdt[num].limitLow = (limit & 0xFFFF);
    gdt[num].granularity = (limit >> 16) & 0x0F;

    // Merge in 4 bits of granularity and size bits
    gdt[num].granularity |= gran & 0xF0;

    // Set acces byte (tupe, ring level, present)
    gdt[num].access = access;
}

// Runs assembly code to flush pointer
void gdtFlush(uint32_t gdt_ptr) {
    asm volatile(
      "lgdt (%0)\n"                     // Load GDT pointer
      "mov $0x10, %%ax\n"               // Load data segment selector (index 2 << 3 = 0x10)
      "mov %%ax, %%ds\n"
      "mov %%ax, %%es\n"
      "mov %%ax, %%fs\n"
      "mov %%ax, %%gs\n"
      "mov %%ax, %%ss\n"
      "ljmp $0x08, $flush_segment\n"    // Far jump to code segment selector (index 1 << 3 = 0x08)
      "flush_segment:\n"                // After jump: now using new code segment
      : : "r" (gdt_ptr) : "ax"          
    );
  }