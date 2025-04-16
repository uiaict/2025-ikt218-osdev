#include "gdt.h"

struct gdtEntry gdt[GDT_ENTRIES];
struct gdtPtr gp;

void initGdt() {
    // Set the GDT limit
    gp.limit = sizeof(struct gdtEntry) * GDT_ENTRIES - 1;
    gp.base = (uint32_t) &gdt;
    // num, base, limit, access, granularity
    gdtSetGate(0, 0, 0, 0, 0); // Null segment
    gdtSetGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
    gdtSetGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
    // Load the GDT
    gdtLoad(&gp);
    // Flush GDT pointer
    gdtFlush((uint32_t)&gp);
}

void gdtLoad(struct gdtPtr *gpt) {
    asm volatile("lgdt %0" : : "m" (*gpt));
}

void gdtSetGate(int32_t num, uint32_t base, uint32_t limit, uint8_t access,
    uint8_t gran)
{
    gdt[num].baseLow = (base & 0xFFFF);
    gdt[num].baseMiddle = (base >> 16) & 0xFF;
    gdt[num].baseHigh = (base >> 24) & 0xFF;
    gdt[num].limitLow = (limit & 0xFFFF);
    gdt[num].granularity = (limit >> 16) & 0x0F;

    gdt[num].granularity |= gran & 0xF0;
    gdt[num].access = access;
}

void gdtFlush(uint32_t gdt_ptr) {
    asm volatile(
      "lgdt (%0)\n"
      "mov $0x10, %%ax\n"
      "mov %%ax, %%ds\n"
      "mov %%ax, %%es\n"
      "mov %%ax, %%fs\n"
      "mov %%ax, %%gs\n"
      "mov %%ax, %%ss\n"
      "ljmp $0x08, $flush_segment\n"
      "flush_segment:\n"
      : : "r" (gdt_ptr) : "ax"
    );
  }