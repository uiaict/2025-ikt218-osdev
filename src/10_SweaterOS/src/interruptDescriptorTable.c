#include "descriptorTables.h" // Inkluderer header for IDT-definisjoner

// IDT with 256 entries
#define IDT_SIZE 256
struct idt_entries idt[IDT_SIZE];

// Struktur som peker til IDT-tabellen og lagrer størrelse
struct idt_pointer idt_info;

// Funksjon i assembly som kaster IDT inn i CPU-en
extern void TOSS_IDT(uint32_t);

/**
 * Legger inn en oppføring i IDT-tabellen.
 * Dette brukes til å registrere en interrupt service-rutine (ISR).
 */
static void idt_add_entry(int index, uint32_t base, uint16_t selector, uint8_t type_attr) 
{
    // Split the base address into two 16-bit parts
    idt[index].isr_address_low = base & 0xFFFF;  // Laveste 16 bit av ISR-adressen
    idt[index].isr_address_high = (base >> 16) & 0xFFFF; // Øverste 16 bit av ISR-adressen
    
    idt[index].segment_selector = selector; // Hvilket segment avbruddet skal bruke
    idt[index].zero = 0; // Skal alltid være 0 ifølge CPU-spesifikasjoner
    idt[index].type_and_flags = type_attr; // Flagg for type av avbrudd og rettigheter
}

/**
 * Setter opp og aktiverer IDT (Interrupt Descriptor Table).
 */
void initializer_IDT() 
{
    // Set up IDT pointer
    idt_info.table_size = (sizeof(struct idt_entries) * IDT_SIZE) - 1;
    idt_info.table_address = (uint32_t)&idt;

    // Clear entire IDT - set all entries to empty/disabled
    for (int i = 0; i < IDT_SIZE; i++) {
        // Use a null handler and disabled state (not present)
        // 0x8E but with present bit (bit 7) set to 0 = 0x0E
        idt_add_entry(i, 0, 0x08, 0x0E);
    }

    // Load IDT
    TOSS_IDT((uint32_t)&idt_info);
}