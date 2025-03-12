#include "descriptorTables.h" // Inkluderer header for GDT-definisjoner
#include "miscFuncs.h"      // For terminal_write functions

// GDT-tabellen med tre entries: null-segment, kode-segment og data-segment
struct gdt_entries gdt[GDT_SIZE];

// Struktur som peker til GDT-tabellen og lagrer størrelse
struct gdt_pointer gdt_info;

// Funksjon i assembly som kaster GDT inn i CPU
extern void gdt_flush(uint32_t);

// Setter opp en GDT-entry med baseadresse, størrelse, tilgangsrettigheter og flagg.
static void gdt_add_entry(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
    // Sjekk at indeksen er gyldig
    if (index < 0 || index >= GDT_SIZE) {
        terminal_write_color("ERROR: Invalid GDT index\n", COLOR_LIGHT_RED);
        delay(100);
        return;
    }
    
    // Set base address (split into 3 parts)
    gdt[index].segment_start_low = (base & 0xFFFF);
    gdt[index].segment_start_middle = (base >> 16) & 0xFF;
    gdt[index].segment_start_high = (base >> 24) & 0xFF;

    // Set segment limit (split into 2 parts)
    gdt[index].segment_size_low = (limit & 0xFFFF);
    gdt[index].size_and_flags = (limit >> 16) & 0x0F;
    
    // Set granularity flags in the upper 4 bits
    gdt[index].size_and_flags |= (granularity & 0xF0);

    // Set access flags (present, privilege level, type, etc.)
    gdt[index].access_flags = access;
}

// Setter opp og installerer GDT.
void initializer_GDT() 
{
    terminal_write_color("Setting up Global Descriptor Table (GDT)...\n", COLOR_WHITE);
    
    // Set up the GDT pointer
    gdt_info.table_size = (sizeof(struct gdt_entries) * GDT_SIZE) - 1;
    gdt_info.table_address = (uint32_t)&gdt;

    // Null segment (required by CPU)
    // Base=0, Limit=0, Access=0, Granularity=0
    gdt_add_entry(0, 0, 0, 0, 0);
    terminal_write_color("  - NULL descriptor added\n", COLOR_GRAY);

    // Code segment: 
    // Access=0x9A (Present=1, Ring=0, Type=1, Code=1, Conforming=0, Readable=1, Accessed=0)
    // Granularity=0xCF (Granularity=1 [4KB], Size=1 [32-bit])
    gdt_add_entry(1, 0, 0xFFFFF, 0x9A, 0xCF);
    terminal_write_color("  - Code segment added\n", COLOR_GRAY);
    
    // Data segment:
    // Access=0x92 (Present=1, Ring=0, Type=1, Code=0, Writable=1, Accessed=0)
    // Granularity=0xCF (Granularity=1 [4KB], Size=1 [32-bit])
    gdt_add_entry(2, 0, 0xFFFFF, 0x92, 0xCF);
    terminal_write_color("  - Data segment added\n", COLOR_GRAY);
    
    terminal_write_color("Loading GDT into CPU...\n", COLOR_WHITE);
    
    // Load the GDT
    gdt_flush((uint32_t)&gdt_info);
    
    terminal_write_color("GDT loaded successfully!\n", COLOR_LIGHT_GREEN);
    
    // Liten pause for å kunne lese output
    delay(50);
}