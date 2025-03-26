#include "libc/gdt.h"

struct gdt_entry gdt[3]; // Null, Code, Data
struct gdt_ptr gdt_descriptor;

// Funksjon for å sette opp en GDT-oppføring
void set_gdt_entry(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
    gdt[index].base_low = base & 0xFFFF;
    gdt[index].base_middle = (base >> 16) & 0xFF;
    gdt[index].base_high = (base >> 24) & 0xFF;

    gdt[index].limit_low = limit & 0xFFFF;
    gdt[index].granularity = (limit >> 16) & 0x0F;
    gdt[index].granularity |= granularity & 0xF0;
    gdt[index].access = access;
}

void load_gdt(struct gdt_ptr* gdt_descriptor) {
    __asm__ volatile (
        "lgdt (%0)\n"          // Last GDT med lgdt-instruksjonen
        "mov $0x10, %%ax\n"    // Data-segmentets selector (indeks 2)
        "mov %%ax, %%ds\n"     // Oppdater DS
        "mov %%ax, %%es\n"     // Oppdater ES
        "mov %%ax, %%fs\n"     // Oppdater FS
        "mov %%ax, %%gs\n"     // Oppdater GS
        "mov %%ax, %%ss\n"     // Oppdater SS
        "ljmp $0x08, $1f\n"    // Lang hopp til kode-segmentets selector (indeks 1)
        "1:\n"                 // Merkepunkt for hopp
        :
        : "r"(gdt_descriptor)  // Input: pekeren til GDT-deskriptoren
        : "memory", "rax"      // Clobbered: memory og rax
    );
}

// Initialiserer GDT
void init_gdt() {
    gdt_descriptor.limit = (sizeof(struct gdt_entry) * 3) - 1;
    gdt_descriptor.base = (uint32_t)&gdt;

    set_gdt_entry(0, 0, 0, 0, 0);                // NULL segment
    set_gdt_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
    set_gdt_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment

    load_gdt(&gdt_descriptor); // Last GDT med inline assembly
}