#ifndef GDT_H
#define GDT_H

#include "stdint.h" // Inkluderer standard integer-typer for konsistens

// Struktur for en enkelt GDT (Global Descriptor Table) entry
// Dette definerer ett segment i minnet (f.eks. kode- eller datasegment)
struct gdt_entry {
    uint16_t limit_low;   // De nederste 16 bitene av segmentets grense (size)
    uint16_t base_low;    // De nederste 16 bitene av basen (startadresse)
    uint8_t base_middle;  // De neste 8 bitene av basen
    uint8_t access;       // Tilgangsbiter (definerer rettigheter og type segment)
    uint8_t granularity;  // Inneholder de øverste 4 bitene av grensen + flags
    uint8_t base_high;    // De øverste 8 bitene av basen
} __attribute__((packed)); // Sørger for at strukturen ikke får ekstra padding

// Struktur for GDT-ptr, som peker til selve GDT-tabellen
struct gdt_ptr {
    uint16_t limit; // Størrelsen på GDT-tabellen i bytes - 1
    uint32_t base;  // Minneadressen til starten av GDT-tabellen
} __attribute__((packed));

// Setter opp og installerer GDT
void gdt_install();

#endif // GDT_H
