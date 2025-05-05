#ifndef GDT_H
#define GDT_H

#include <libc/stdint.h>

// Struktur for en GDT-segmentbeskrivelse (totalt 8 bytes)
struct gdt_entry {
    uint16_t limit_low;    // laveste 16 bits av limit
    uint16_t base_low;     // laveste 16 bits av base
    uint8_t  base_mid;     // midtre 8 bits av base
    uint8_t  access;       // tilgangsbiter
    uint8_t  granularity;  // flags og høyeste 4 bits av limit
    uint8_t  base_high;    // høyeste 8 bits av base
} __attribute__((packed));

// Struktur som sendes til LGDT-instruksjonen
struct gdt_ptr {
    uint16_t limit;       // størrelse på GDT - 1
    uint32_t base;        // adresse til GDT-tabellen
} __attribute__((packed));

// Initialiser GDT og bytt til flat protected mode
void gdt_init(void);

#endif // GDT_H
