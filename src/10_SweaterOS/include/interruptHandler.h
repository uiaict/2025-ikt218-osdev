#ifndef __INTERRUPT_HANDLER_H
#define __INTERRUPT_HANDLER_H

#include "libc/stdint.h"

// PIC (Programmerbar Interrupt Controller) porter
#define PIC1_COMMAND    0x20
#define PIC1_DATA       0x21
#define PIC2_COMMAND    0xA0
#define PIC2_DATA       0xA1

// Tastaturkontroller porter
#define KEYBOARD_DATA      0x60
#define KEYBOARD_STATUS    0x64

/**
 * I/O operasjoner
 * 
 * Funksjoner for å kommunisere med maskinvare via porter
 */
void outb(uint16_t port, uint8_t value);    // Skriver 8-bit verdi til port
void outw(uint16_t port, uint16_t value);   // Skriver 16-bit verdi til port
uint8_t inb(uint16_t port);                 // Leser 8-bit verdi fra port
uint16_t inw(uint16_t port);                // Leser 16-bit verdi fra port
void io_wait(void);                         // Vent på I/O operasjon

/**
 * Interrupt håndterere
 * 
 * Håndterer system interrupts (ISR) og maskinvare interrupts (IRQ)
 */
void isr_handler(uint32_t esp);  // Håndterer system interrupts
void irq_handler(uint32_t esp);  // Håndterer maskinvare interrupts

/**
 * System initialisering
 * 
 * Setter opp interrupt systemet og PIC
 */
void interrupt_initialize(void);  // Initialiserer interrupt systemet
void pic_initialize(void);        // Initialiserer Programmerbar Interrupt Controller

/**
 * Tastatur funksjoner
 * 
 * Håndterer tastatur input og konvertering av scancodes
 */
char keyboard_getchar(void);              // Henter neste tastetrykk
int keyboard_data_available(void);        // Sjekker om det er tastetrykk tilgjengelig
char scancode_to_ascii(uint8_t scancode); // Konverterer scancode til ASCII

/**
 * Hjelpefunksjoner
 * 
 * Verktøy for å håndtere interrupt status
 */
int interrupts_enabled(void);  // Sjekker om interrupts er aktivert

#endif /* __INTERRUPT_HANDLER_H */ 