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
 * CPU state structure - inneholder alle CPU-registre
 * 
 * Denne strukturen representerer tilstanden til CPU-en når et interrupt oppstår.
 * Rekkefølgen på feltene er viktig og må matche rekkefølgen i assembly-koden.
 * 
 * Merk: Denne strukturen fylles av 'pusha' instruksjonen i assembly-koden,
 * så rekkefølgen på feltene må være: edi, esi, ebp, esp, ebx, edx, ecx, eax.
 */
struct cpu_state {
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
} __attribute__((packed));

/**
 * Stack state structure - inneholder informasjon om stack-tilstanden
 * 
 * Denne strukturen representerer tilstanden til stack-en når et interrupt oppstår.
 * Rekkefølgen på feltene er viktig og må matche rekkefølgen i assembly-koden.
 * 
 * Merk: Denne strukturen fylles delvis av CPU-en (eip, cs, eflags) og delvis
 * av vår assembly-kode (error_code).
 */
struct stack_state {
    uint32_t error_code;
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
} __attribute__((packed));

/**
 * Exception handler - håndterer CPU exceptions (interrupt 0-31)
 * 
 * @param cpu CPU-tilstand når exception oppsto
 * @param int_no Interrupt-nummer
 * @param stack Stack-tilstand når exception oppsto
 */
void exception_handler(struct cpu_state cpu, uint32_t int_no, struct stack_state stack);

/**
 * IRQ handler - håndterer hardware interrupts (IRQ)
 * 
 * @param cpu CPU-tilstand når interrupt oppsto
 * @param int_no Interrupt-nummer
 * @param stack Stack-tilstand når interrupt oppsto
 */
void irq_handler(struct cpu_state cpu, uint32_t int_no, struct stack_state stack);

/**
 * Initialiserer PIC (Programmable Interrupt Controller)
 * Dette er nødvendig for å håndtere hardware-interrupts som keyboard-input
 */
void pic_initialize(void);

/**
 * Sender End-of-Interrupt (EOI) signal til PIC
 * Må kalles etter at en interrupt er håndtert for å signalisere at
 * vi er klare til å motta nye interrupts
 */
void pic_send_eoi(uint8_t irq);

/**
 * Initialiserer alle interrupts (både CPU exceptions og hardware interrupts)
 */
void interrupt_initialize(void);

/**
 * Keyboard interrupt handler
 * Håndterer tastatur interrupts (IRQ1)
 */
void keyboard_handler(void);

/**
 * Returnerer om en tast er trykket ned
 * Brukes for å sjekke om tastatur-input er tilgjengelig
 */
int keyboard_is_key_pressed(void);

/**
 * Leser et tegn fra tastaturet
 * Returnerer ASCII-verdien for tasten som ble trykket
 */
char keyboard_read_char(void);

#endif /* INTERRUPT_HANDLER_H */ 