#include "gdt.h" // Inkluderer GDT-headeren for definisjoner

// GDT-tabellen, som inneholder tre entries: null-segment, kode-segment og data-segment
struct gdt_entry gdt[3];

// Struktur som peker til GDT-tabellen og lagrer størrelsen
struct gdt_ptr gdt_pointer;

// Funksjon i assembly som laster GDT inn i CPU-en
extern void gdt_flush(uint32_t);

/**
 * Setter opp én GDT-entry med riktig baseadresse, grense, tilgang og flagg.
 */
static void gdt_set_entry(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    // Setter base-adressen (adressen til segmentets start i minnet)
    gdt[num].base_low = (base & 0xFFFF);        // De laveste 16 bitene av basen
    gdt[num].base_middle = (base >> 16) & 0xFF; // De neste 8 bitene av basen
    gdt[num].base_high = (base >> 24) & 0xFF;   // De øverste 8 bitene av basen

    // Setter segmentgrensen (størrelsen på segmentet)
    gdt[num].limit_low = (limit & 0xFFFF);      // De laveste 16 bitene av grensen
    gdt[num].granularity = (limit >> 16) & 0x0F; // De øverste 4 bitene av grensen

    // Setter flagg for granularitet (4K sider) og andre innstillinger
    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access = access; // Definerer rettigheter og type segment
}

/**
 * Setter opp GDT-tabellen og laster den inn i CPU-en.
 */
void gdt_install() {
    // Setter opp GDT-peker med størrelse og baseadresse
    gdt_pointer.limit = (sizeof(struct gdt_entry) * 3) - 1;
    gdt_pointer.base = (uint32_t)&gdt;

    // Definerer segmentene i GDT
    gdt_set_entry(0, 0, 0, 0, 0);                // Null-segment (må være der)
    gdt_set_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Kode-segment (for kjøring av program)
    gdt_set_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data-segment (for lagring av data)

    // Laster GDT ved å kalle gdt_flush (som er definert i assembly)
    gdt_flush((uint32_t)&gdt_pointer);
}