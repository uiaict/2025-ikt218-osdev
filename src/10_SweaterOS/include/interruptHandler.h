#ifndef INTERRUPT_HANDLER_H
#define INTERRUPT_HANDLER_H

#include "libc/stdint.h"

/**
 * Konstanter for PIC (Programmable Interrupt Controller)
 * PIC er ansvarlig for å håndtere hardware-interrupts og sende dem til CPU-en
 */
#define PIC1_COMMAND_PORT 0x20    // Command port for master PIC
#define PIC1_DATA_PORT    0x21    // Data port for master PIC
#define PIC2_COMMAND_PORT 0xA0    // Command port for slave PIC
#define PIC2_DATA_PORT    0xA1    // Data port for slave PIC

#define PIC_EOI           0x20    // End of Interrupt kommando

// Keyboard ports and commands
#define KEYBOARD_DATA_PORT    0x60    // Keyboard data port
#define KEYBOARD_STATUS_PORT  0x64    // Keyboard status port
#define KEYBOARD_COMMAND_PORT 0x64    // Keyboard command port

// ICW = Initialization Command Word
#define ICW1_ICW4         0x01    // ICW4 følger
#define ICW1_SINGLE       0x02    // Single modus (ikke cascade)
#define ICW1_INTERVAL4    0x04    // Call address interval 4 (8086 modus)
#define ICW1_LEVEL        0x08    // Level triggered modus
#define ICW1_INIT         0x10    // Initialization kommando

#define ICW4_8086         0x01    // 8086/88 modus
#define ICW4_AUTO         0x02    // Auto EOI
#define ICW4_BUF_SLAVE    0x08    // Buffered modus slave
#define ICW4_BUF_MASTER   0x0C    // Buffered modus master
#define ICW4_SFNM         0x10    // Special fully nested modus

/**
 * Skriver en byte til en I/O-port
 * 
 * I/O-porten (0-65535)
 * Verdien som skal skrives (0-255)
 */
void outb(uint16_t port, uint8_t value);

/**
 * Skriver et 16-bit ord til en I/O-port
 * 
 * I/O-porten (0-65535)
 * Verdien som skal skrives (0-65535)
 */
void outw(uint16_t port, uint16_t value);

/**
 * Leser en byte fra en I/O-port
 * 
 * I/O-porten (0-65535)
 * Verdien som ble lest (0-255)
 */
uint8_t inb(uint16_t port);

/**
 * Leser et 16-bit ord fra en I/O-port
 */
uint16_t inw(uint16_t port);

/**
 * Gir en kort forsinkelse, brukes ofte etter I/O-operasjoner
 */
void io_wait(void);

/**
 * Exception handler - håndterer CPU exceptions (0-31)
 * 
 * @param esp Peker til stack frame som inneholder alle registers og exception info
 */
void isr_handler(uint32_t esp);

/**
 * IRQ handler - håndterer hardware interrupts (32-47)
 * 
 * @param esp Peker til stack frame som inneholder alle registers og interrupt info
 */
void irq_handler(uint32_t esp);

/**
 * Initialiserer PIC (Programmable Interrupt Controller)
 */
void pic_initialize(void);

/**
 * Sender End-of-Interrupt (EOI) signal til PIC
 * 
 * IRQ-nummeret (0-15)
 */
void pic_send_eoi(uint8_t irq);

/**
 * Initialiserer interrupt-systemet
 */
void interrupt_initialize(void);

/**
 * Keyboard handler - håndterer tastatur-interrupts (IRQ 1)
 */
void keyboard_handler(void);

/**
 * Sjekker om det er tegn tilgjengelig i tastatur-bufferen
 * 
 * Returnerer 1 hvis det er tegn tilgjengelig, 0 ellers
 */
int keyboard_data_available(void);

/**
 * Leser et tegn fra tastatur-bufferen
 * 
 * Returnerer ASCII-tegnet, eller 0 hvis bufferen er tom
 */
char keyboard_getchar(void);

/**
 * Checks if interrupts are currently enabled
 * 
 * Returnerer 1 hvis interrupts er aktivert, 0 hvis deaktivert
 */
int interrupts_enabled(void);

/**
 * Get the ASCII mapping for a given scancode
 * 
 * Returnerer ASCII-tegnet for en scancode, eller 0 hvis ingen mapping finnes
 */
char get_ascii_for_scancode(uint8_t scancode, int shift);

/**
 * Initialize the keyboard controller
 */
void keyboard_initialize(void);

/**
 * Timer interrupt handler function
 * Called when a timer interrupt (IRQ0) occurs
 */
void timer_handler(void);

// Convert scancode to ASCII character
char scancode_to_ascii(uint8_t scancode);

#endif /* INTERRUPT_HANDLER_H */ 