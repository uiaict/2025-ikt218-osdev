#ifndef DESCRIPTOR_TABLES_H
#define DESCRIPTOR_TABLES_H

#include "libc/stdint.h" // For uint8_t, uint16_t, uint32_t

#define IDT_SIZE 256 // Antall avbruddstabell-oppføringer
#define GDT_SIZE 3   // Antall segmenter i GDT

// En oppføring i GDT (Global Descriptor Table)
struct gdt_entries {
    uint16_t segment_size_low;   // Nedre 16 bit av segmentstørrelse
    uint16_t segment_start_low;  // Nedre 16 bit av startadresse
    uint8_t segment_start_middle;   // Midtre 8 bit av startadresse
    uint8_t access_flags;        // Rettigheter og type segment
    uint8_t size_and_flags;      // Størrelsesbiter og flagg
    uint8_t segment_start_high;  // Øvre 8 bit av startadresse
} __attribute__((packed)); // Hindrer at det blir lagt til padding, slik at CPU får et nøyaktig minneområde å jobbe på.

// GDT peker (forteller hvor GDT er i minnet)
struct gdt_pointer {
    uint16_t table_size;    // Størrelse på GDT - 1
    uint32_t table_address; // Startadresse til GDT i minnet
} __attribute__((packed));

// En oppføring i IDT (Interrupt Descriptor Table)
struct idt_entries {
    uint16_t isr_address_low;   // Nedre 16 bit av ISR (interrupt service routine) adresse
    uint16_t segment_selector;  // Hvilket segment ISR ligger i
    uint8_t zero;           // Alltid 0
    uint8_t type_and_flags;     // Type avbrudd og flagg
    uint16_t isr_address_high;  // Øvre 16 bit av ISR adresse
} __attribute__((packed));

// IDT peker (forteller hvor IDT er i minnet)
struct idt_pointer {
    uint16_t table_size;    // Størrelse på IDT - 1
    uint32_t table_address; // Startadresse til IDT i minnet
} __attribute__((packed));

// Loads and initializes the descriptor tables
void initializer_GDT();
void initializer_IDT();

// Assembly functions to load the descriptor tables
extern void TOSS_GDT(uint32_t);
extern void TOSS_IDT(uint32_t);

#endif // DESCRIPTOR_TABLES_H