#ifndef _MISCFUNCS_H
#define _MISCFUNCS_H

#include "libc/stdint.h"  // Standard heltallstyper
#include "libc/stdbool.h"
#include "multiboot2.h"
#include "display.h"

/**
 * Hjelpefunksjoner
 * 
 * Generelle verktøy for systemet
 */
void hexToString(uint32_t num, char* str);  // Konverterer hex til streng
void delay(uint32_t ms);                    // Vent i millisekunder
void int_to_string(int num, char* str);     // Konverterer heltall til streng

/**
 * Boot verifisering
 * 
 * Sjekker om systemet ble riktig startet
 */
bool verify_boot_magic(uint32_t magic);     // Verifiserer boot magisk nummer

/**
 * Minnehåndtering
 * 
 * Funksjoner for å håndtere minneoppsett
 */
void print_multiboot_memory_layout(struct multiboot_tag* tag); // Skriver ut minneoppsett

/**
 * Systemkontroll
 * 
 * Funksjoner for å kontrollere systemtilstand
 */
void halt(void);                // Stopper systemet
void initialize_system(void);   // Initialiserer systemet
void disable_interrupts(void);  // Deaktiverer interrupts
void enable_interrupts(void);   // Aktiverer interrupts

#endif /* _MISCFUNCS_H */
