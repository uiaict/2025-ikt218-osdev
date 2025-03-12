#ifndef DESCRIPTOR_TABLES_H
#define DESCRIPTOR_TABLES_H

/**
 * Descriptor Tables Header
 * 
 * Dette headerfilen definerer strukturene for to kritiske tabeller i x86-arkitekturen:
 * 1. Global Descriptor Table (GDT) - Håndterer minnesegmentering
 * 2. Interrupt Descriptor Table (IDT) - Håndterer interrupts
 * 
 * Disse tabellene er fundamentale for hvordan CPU-en håndterer minne og interrupts,
 * og er blant de første tingene som må settes opp i et operativsystem.
 */

#include "libc/stdint.h" // For uint8_t, uint16_t, uint32_t

// Konstanter for tabellstørrelser
#define IDT_SIZE 256 // Antall avbruddstabell-oppføringer (maksimalt antall interrupts i x86)
#define GDT_SIZE 3   // Antall segmenter i GDT (null-segment, kode-segment, data-segment)

/**
 * Global Descriptor Table (GDT) Entry
 * 
 * GDT er en datastruktur som definerer minneområder (segmenter) i systemet.
 * Hver oppføring beskriver et segment med informasjon om:
 * - Startadresse (hvor segmentet begynner i minnet)
 * - Størrelse (hvor stort segmentet er)
 * - Rettigheter (hvem som kan aksessere segmentet og hvordan)
 * - Type (kode, data, etc.)
 * 
 * I moderne operativsystemer brukes GDT hovedsakelig for å skille mellom
 * kernel-modus og bruker-modus, ikke for faktisk minneinndeling (som nå
 * håndteres av paging).
 */
struct gdt_entries {
    uint16_t segment_size_low;     // Nedre 16 bit av segmentstørrelse
    uint16_t segment_start_low;    // Nedre 16 bit av startadresse
    uint8_t segment_start_middle;  // Midtre 8 bit av startadresse
    uint8_t access_flags;          // Rettigheter og type segment (ring-nivå, executable, etc.)
    uint8_t size_and_flags;        // Øvre 4 bit av størrelse + granularity, 32-bit modus, etc.
    uint8_t segment_start_high;    // Øvre 8 bit av startadresse
} __attribute__((packed)); // Hindrer at kompilatoren legger til padding mellom feltene

/**
 * GDT Pointer
 * 
 * Denne strukturen lastes inn i GDTR-registeret med lgdt-instruksjonen.
 * Den forteller CPU-en hvor GDT-tabellen er i minnet og hvor stor den er.
 */
struct gdt_pointer {
    uint16_t table_size;    // Størrelse på GDT minus 1 (i bytes)
    uint32_t table_address; // Fysisk adresse til GDT i minnet
} __attribute__((packed));

/**
 * Interrupt Descriptor Table (IDT) Entry
 * 
 * IDT er en datastruktur som forteller CPU-en hvilke funksjoner som skal
 * kjøres når ulike interrupts oppstår. Hver oppføring peker til en
 * Interrupt Service Routine (ISR) som håndterer et spesifikt interrupt.
 * 
 * Interrupts kan være:
 * - CPU exceptions (f.eks. division by zero, page fault)
 * - Hardware interrupts (f.eks. tastatur, timer)
 * - Software interrupts (generert av int-instruksjonen)
 */
struct idt_entries {
    uint16_t isr_address_low;   // Nedre 16 bit av ISR-adressen
    uint16_t segment_selector;  // Hvilket kodesegment ISR ligger i (vanligvis 0x08 for kernel)
    uint8_t zero;               // Alltid 0 (reservert felt)
    uint8_t type_and_flags;     // Type gate (interrupt/trap) og attributter (present, ring-nivå)
    uint16_t isr_address_high;  // Øvre 16 bit av ISR-adressen
} __attribute__((packed));

/**
 * IDT Pointer
 * 
 * Denne strukturen lastes inn i IDTR-registeret med lidt-instruksjonen.
 * Den forteller CPU-en hvor IDT-tabellen er i minnet og hvor stor den er.
 */
struct idt_pointer {
    uint16_t table_size;    // Størrelse på IDT minus 1 (i bytes)
    uint32_t table_address; // Fysisk adresse til IDT i minnet
} __attribute__((packed));

/**
 * Funksjoner for å initialisere deskriptortabellene
 * 
 * Disse funksjonene setter opp GDT og IDT med riktige verdier
 * og laster dem inn i CPU-en.
 */
void initializer_GDT(); // Initialiserer Global Descriptor Table
void initializer_IDT(); // Initialiserer Interrupt Descriptor Table

/**
 * Assembly-funksjoner for å laste deskriptortabellene
 * 
 * Disse funksjonene er definert i assembly fordi de bruker spesielle
 * instruksjoner (lgdt og lidt) som ikke er tilgjengelige i C.
 */
extern void gdt_flush(uint32_t); // Laster GDT inn i GDTR-registeret
extern void idt_flush(uint32_t); // Laster IDT inn i IDTR-registeret

#endif // DESCRIPTOR_TABLES_H