#ifndef GDT_H // Sjekker om GDT_H er definert
#define GDT_H // Definerer GDT_H for å unngå dobbel inkludering

// #include "libc/stdint.h"
#include "libc/system.h"

// gdt.h

#define GDT_entries 5
#define IDT_entries 256

////////////////////////////////////// GDT-struktur

struct GDTEntry
{
    uint16_t limit_low;    // The lower 16 bits of the limit (lagrer de første minste 16 bitene av limit)
    uint16_t base_low;     // The lower 16 bits of the base (the 16 first bits of the base adress)
    uint8_t base_middle;   // The next 8 bits of the base (the 8 nest bits of the base adress)
    uint8_t access;        // It bestemmer type of segment (example; kode, data, ring-level)
    uint8_t granularity;   // The last 4 bits of the limit in the memory
    uint8_t base_high;     // the highest 8 bits of the base (the last 8 bits of the base adress)
} __attribute__((packed)); // sikrer at kompilatoren ikke legger til ekstra bytes

struct GDTPointer
{                   // a pointer to our gdt
    uint16_t limit; // The limit is the table -1(Forteller hvor stor tabellen er (antall bytes - 1)); så (* 5 -1)
    uint32_t base;  // Holder minneaddressen til starten av GDT tabellen
} __attribute__((packed));

// GDT funskjoner
void init_gdt();
void set_gdt_gate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity);
extern void gdt_flush(uint32_t); // Ekstern ASM-funksjon

/////////////////////////// IDT-struktur -  skriver både gdt og idt i en header fil fo å bli minde forvirret og trenger bare å inkludere en fil

// IDT funskjoner

// Struktur for én IDT-entry
struct IDTEntry
{
    uint16_t base_low;
    uint16_t sel;
    uint8_t zero;
    uint8_t flags;
    uint16_t base_high;
} __attribute__((packed));

// Struktur for IDT peker
struct IDTPointer
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

void init_idt();
void idt_load();

static struct GDTEntry gdt[GDT_entries];
static struct GDTPointer gdt_ptr;
static struct IDTEntry idt[IDT_entries];
static struct IDTPointer idt_ptr;

#endif // sikrer at filen kun inkluderes én gang.
