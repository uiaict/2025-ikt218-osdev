#ifndef GDT_H
#define GDT_H

// GDT Entry struktur
struct gdt_entry {
    unsigned short limit_low;   // De første 16 bitene av segmentgrensen
    unsigned short base_low;    // De første 16 bitene av baseadressen
    unsigned char base_middle;  // De neste 8 bitene av baseadressen
    unsigned char access;       // Tilgangsbyte (bestemmer type segment)
    unsigned char granularity;  // Inneholder granularity flagg og limit høyeste bits
    unsigned char base_high;    // De siste 8 bitene av baseadressen
} __attribute__((packed));

// GDT register (brukes av `lgdt`)
struct gdt_ptr {
    unsigned short limit;  // Størrelse på GDT-tabellen - 1
    unsigned int base;     // Baseadresse til GDT-tabellen
} __attribute__((packed));

void gdt_init(void); // Funksjon for å initialisere GDT

#endif // GDT_H
