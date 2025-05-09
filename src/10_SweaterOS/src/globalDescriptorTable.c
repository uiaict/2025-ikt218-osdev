#include "descriptorTables.h"
#include "display.h"
#include "miscFuncs.h"

// GDT-tabell med tre oppføringer: null-segment, kodesegment og datasegment
struct gdt_entries gdt[GDT_SIZE];

// Struktur som peker til GDT-tabellen og lagrer størrelsen
struct gdt_pointer gdt_info;

// Assemblerfunksjon som laster GDT inn i CPU
extern void gdt_flush(uint32_t);

/**
 * Setter opp en GDT-oppføring med baseadresse, størrelse, tilgangsrettigheter og flagg
 */
static void gdt_add_entry(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
    if (index < 0 || index >= GDT_SIZE) {
        display_write_color("ERROR: Invalid GDT index\n", COLOR_LIGHT_RED);
        delay(100);
        return;
    }
    
    // Setter baseadresse
    gdt[index].segment_start_low = (base & 0xFFFF);
    gdt[index].segment_start_middle = (base >> 16) & 0xFF;
    gdt[index].segment_start_high = (base >> 24) & 0xFF;

    // Setter segmentgrense
    gdt[index].segment_size_low = (limit & 0xFFFF);
    gdt[index].size_and_flags = (limit >> 16) & 0x0F;
    
    // Setter granularitetsflagg
    gdt[index].size_and_flags |= (granularity & 0xF0);

    // Setter tilgangsflagg
    gdt[index].access_flags = access;
}

/**
 * Setter opp og installerer GDT
 */
void initializer_GDT() 
{
    display_write_color("Setting up Global Descriptor Table (GDT)...\n", COLOR_WHITE);
    
    // Setter opp GDT-peker
    gdt_info.table_size = (sizeof(struct gdt_entries) * GDT_SIZE) - 1;
    gdt_info.table_address = (uint32_t)&gdt;

    // Null-segment (kreves av CPU)
    gdt_add_entry(0, 0, 0, 0, 0);
    display_write_color("  - NULL descriptor added\n", COLOR_DARK_GREY);

    // Kodesegment
    gdt_add_entry(1, 0, 0xFFFFF, 0x9A, 0xCF);
    display_write_color("  - Code segment added\n", COLOR_DARK_GREY);
    
    // Datasegment
    gdt_add_entry(2, 0, 0xFFFFF, 0x92, 0xCF);
    display_write_color("  - Data segment added\n", COLOR_DARK_GREY);
    
    display_write_color("Loading GDT into CPU...\n", COLOR_WHITE);
    
    // Laster GDT
    gdt_flush((uint32_t)&gdt_info);
    
    display_write_color("GDT loaded successfully!\n", COLOR_LIGHT_GREEN);
    
    delay(50);
}