#ifndef GDT_H
#define GDT_H

#include <stdint.h>

// GDT Entry struktur
struct gdt_entry {
    uint16_t limit_low;   // De første 16 bitene av segmentgrensen
    uint16_t base_low;    // De første 16 bitene av baseadressen
    uint8_t base_middle;  // De neste 8 bitene av baseadressen
    uint8_t access;       // Tilgangsbyte (bestemmer type segment)
    uint8_t granularity;  // Inneholder granularity flagg og limit høyeste bits
    uint8_t base_high;    // De siste 8 bitene av baseadressen
} __attribute__((packed));

// GDT register (brukes av `lgdt`)
struct gdt_ptr {
    uint16_t limit;  // Størrelse på GDT-tabellen - 1
    uint32_t base;   // Baseadresse til GDT-tabellen
} __attribute__((packed));

void gdt_init(); // Funksjon for å initialisere GDT

#endif
