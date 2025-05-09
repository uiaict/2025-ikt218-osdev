#include "libc/stdint.h"      // Standard integer types
#include "miscFuncs.h"        // Function declarations
#include "descriptorTables.h"
#include "interruptHandler.h"
#include "display.h"
#include "libc/string.h"
#include "libc/stdbool.h"
#include "multiboot2.h"
#include "programmableIntervalTimer.h"
#include "memory_manager.h"   // For memory management functions

// Dette er definert i linker scriptet
extern uint32_t end;

// Multiboot2 magic number - dette er verdien multiboot2-kompatible bootloadere
// vil sende som parameter til kernel_main()
#define MULTIBOOT2_MAGIC 0x36d76289

// Utility functions
/**
 * Konverterer et tall til en heksadesimal streng.
 * Resultatet blir lagret i str-parameteren, som må ha plass til minst 11 tegn.
 */
void hexToString(uint32_t num, char* str) {
    const char hex_chars[] = "0123456789ABCDEF";
    int i = 0;
    
    // Handle zero case
    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }
    
    // Convert to hex string
    while (num > 0) {
        str[i++] = hex_chars[num & 0xF];
        num >>= 4;
    }
    str[i] = '\0';
    
    // Reverse the string
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

/**
 * Konverterer et heltall til en desimal streng.
 * Resultatet blir lagret i str-parameteren.
 */
void int_to_string(int num, char* str) {
    int i = 0;
    bool is_negative = false;
    
    // Handle zero case
    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }
    
    // Handle negative numbers
    if (num < 0) {
        is_negative = true;
        num = -num;
    }
    
    // Convert to decimal string (reversed)
    while (num > 0) {
        str[i++] = '0' + (num % 10);
        num /= 10;
    }
    
    // Add minus sign if negative
    if (is_negative) {
        str[i++] = '-';
    }
    
    str[i] = '\0';
    
    // Reverse the string
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

/**
 * En mer nøyaktig forsinkelsesrutine
 * Bruker PIT-basert sleep for presis timing
 * 
 * ms er antall millisekunder å vente
 */
void delay(uint32_t ms) {
    // Mye mer effektiv busy-wait løkke med drastisk redusert iterasjoner
    // Dette reduserer belastningen på CPU men gir fortsatt presis timing
    for (volatile uint32_t i = 0; i < ms * 1000; i++) {
        __asm__ volatile("pause");  // CPU hint for å være mer effektiv i venting
    }
}

/**
 * Verifiserer at vi har blitt startet av en Multiboot2-kompatibel bootloader
 * 
 * magic er verdien i EAX-registeret ved oppstart
 * true hvis magic er gyldig Multiboot2-magic, false ellers
 */
bool verify_boot_magic(uint32_t magic) {
    return magic == MULTIBOOT2_MAGIC;
}

/**
 * Skriver ut minnekartet fra Multiboot2-informasjonen
 * 
 * tag er peker til Multiboot2 memory map tag
 */
void print_multiboot_memory_layout(struct multiboot_tag* tag) {
    if (tag->type != MULTIBOOT_TAG_TYPE_MMAP) {
        display_write("Invalid memory map tag\n");
        return;
    }
    struct multiboot_tag_mmap* mmap = (struct multiboot_tag_mmap*)tag;
    multiboot_memory_map_t* entry = mmap->entries;
    display_write("Memory Map:\n");
    display_write("Address         Length          Type\n");
    while ((uint8_t*)entry < (uint8_t*)mmap + mmap->size) {
        char addr_str[16];
        char len_str[16];
        hexToString(entry->addr, addr_str);
        hexToString(entry->len, len_str);
        const char* type;
        switch (entry->type) {
            case MULTIBOOT_MEMORY_AVAILABLE:
                type = "Available";
                break;
            case MULTIBOOT_MEMORY_RESERVED:
                type = "Reserved";
                break;
            case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE:
                type = "ACPI";
                break;
            case MULTIBOOT_MEMORY_NVS:
                type = "NVS";
                break;
            case MULTIBOOT_MEMORY_BADRAM:
                type = "Bad RAM";
                break;
            default:
                type = "Unknown";
        }
        display_write(addr_str);
        display_write("  ");
        display_write(len_str);
        display_write("  ");
        display_write(type);
        display_write("\n");
        entry = (multiboot_memory_map_t*)((uint8_t*)entry + mmap->entry_size);
    }
}

/**
 * Stopper CPU fullstendig - brukes i kritiske feilsituasjoner
 */
void halt(void) {
    __asm__ volatile("hlt");
}

/**
 * Initialiserer systemets grunnleggende komponenter som GDT og IDT.
 * Dette setter opp segmentering og interrupt-håndtering, som er nødvendig for operativsystemet.
 */
void initialize_system(void) {
    display_write_color("\n=== Initializing System Components ===\n", COLOR_LIGHT_CYAN);
    
    // Initialiser skjermen først for å vise meldinger
    display_initialize();
    
    // Initialiser Global Descriptor Table
    display_write_color("Initializing Global Descriptor Table...\n", COLOR_WHITE);
    initializer_GDT();
    display_write_color("GDT initialized successfully\n", COLOR_LIGHT_GREEN);
    
    // Initialiser Interrupt Descriptor Table
    display_write_color("Initializing Interrupt Descriptor Table...\n", COLOR_WHITE);
    initializer_IDT();
    display_write_color("IDT initialized successfully\n", COLOR_LIGHT_GREEN);
    
    // Initialiser interrupt-håndtering og tastatur
    display_write_color("Initializing interrupt handling...\n", COLOR_WHITE);
    interrupt_initialize();
    display_write_color("Interrupt handling initialized successfully\n", COLOR_LIGHT_GREEN);
    
    // Initialiser timer
    display_write_color("Initializing system timer (PIT)...\n", COLOR_WHITE);
    init_programmable_interval_timer();
    display_write_color("System timer initialized successfully\n", COLOR_LIGHT_GREEN);
    
    // Initialiser minnehåndtering
    display_write_color("Initializing memory management...\n", COLOR_WHITE);
    init_kernel_memory(&end);
    display_write_color("Memory management initialized successfully\n", COLOR_LIGHT_GREEN);

    // Initialiser Paging (setter bare flagg i denne implementasjonen)
    display_write_color("Initializing paging...\n", COLOR_WHITE);
    init_paging(); 
    display_write_color("Paging initialized (flag set)\n", COLOR_LIGHT_GREEN);

    // Aktiver interrupts til slutt
    enable_interrupts();
    display_write_color("Enabled interrupts\n", COLOR_LIGHT_GREEN);
    
    display_write_color("\nSystem initialization completed!\n", COLOR_LIGHT_GREEN);
}

/**
 * Deaktiverer interrupts
 */
void disable_interrupts(void) {
    __asm__ volatile("cli");
}

void enable_interrupts(void) {
    __asm__ volatile("sti");
}