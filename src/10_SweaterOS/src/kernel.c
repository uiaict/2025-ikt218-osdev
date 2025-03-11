#include "libc/stdint.h"
#include "miscFuncs.h"
#include "descriptorTables.h"
#include "interruptHandler.h"
#include "testFuncs.h"

/**
 * Starter operativsystemet
 * 
 * Denne funksjonen initialiserer alle nødvendige komponenter
 * og kjører tester for å verifisere at alt fungerer.
 */
void startOS() {
    // Initialiser terminal for output
    terminal_initialize();
    
    // Vis velkomstmelding
    terminal_write_color("SweaterOS - Interrupt System\n", COLOR_YELLOW);
    terminal_write_color("===========================\n\n", COLOR_YELLOW);
    
    // Initialiser GDT (Global Descriptor Table)
    terminal_write_color("Initializing Global Descriptor Table (GDT)...\n", COLOR_WHITE);
    initializer_GDT();
    terminal_write_color("GDT initialized successfully!\n\n", COLOR_GREEN);
    
    // Initialiser IDT (Interrupt Descriptor Table)
    terminal_write_color("Initializing Interrupt Descriptor Table (IDT)...\n", COLOR_WHITE);
    initializer_IDT();
    terminal_write_color("IDT initialized successfully!\n\n", COLOR_GREEN);
    
    // Initialiser PIC (Programmable Interrupt Controller)
    terminal_write_color("Initializing Programmable Interrupt Controller (PIC)...\n", COLOR_WHITE);
    pic_initialize();
    terminal_write_color("PIC initialized successfully!\n\n", COLOR_GREEN);
    
    // Aktiver interrupts
    terminal_write_color("Enabling interrupts...\n", COLOR_WHITE);
    __asm__ volatile("sti");
    terminal_write_color("Interrupts enabled!\n\n", COLOR_GREEN);
    
    // Kjør tester
    run_all_tests();
    
    // Systemet er nå initialisert og klart
    terminal_write_color("\n\nSystem initialization complete!\n", COLOR_LIGHT_GREEN);
    terminal_write_color("System is now in idle state.\n", COLOR_LIGHT_GREEN);
}

/**
 * Hovedinngang til operativsystemet
 * 
 * @param magic Magisk tall fra bootloader
 * @param mb_info_addr Peker til multiboot-informasjon
 * @return Returnerer aldri
 */
int main(uint32_t magic, void* mb_info_addr) {
    // Verifiser multiboot magic number
    verify_boot_magic(magic);
    
    // Start operativsystemet
    startOS();
    
    // Idle loop - systemet vil aldri nå hit under normal drift
    while(1) {
        __asm__ volatile("hlt");
    }
    
    return 0;  // Nås aldri
}