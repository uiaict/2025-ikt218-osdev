#include "libc/stdint.h"
#include "descriptorTables.h"
#include "miscFuncs.h"
#include "interruptHandler.h"

/**
 * Interrupt Descriptor Table (IDT) implementasjon
 * 
 * Dette er en kritisk del av operativsystemet som håndterer interrupts.
 * IDT forteller CPU-en hvilke funksjoner som skal kjøres når ulike
 * interrupts oppstår, som hardware-interrupts eller CPU exceptions.
 * 
 * Hver inngang i IDT-tabellen peker til en Interrupt Service Routine (ISR)
 * som håndterer det spesifikke interruptet.
 */

// Størrelsen på IDT-tabellen (Interrupt Descriptor Table)
// Vi definerer 256 innganger, som er maksimalt antall interrupts i x86
#define IDT_SIZE 256

// IDT-tabellen - array av IDT-entries
// Hver entry inneholder informasjon om en interrupt handler
struct idt_entries idt[IDT_SIZE];

// IDT-peker - forteller CPU hvor IDT er i minnet
// Denne strukturen lastes inn i IDTR-registeret med lidt-instruksjonen
struct idt_pointer idt_info;

// Importerer funksjonen for å laste IDT inn i CPU-en
// Denne er definert i assembly fordi vi trenger lidt-instruksjonen
extern void idt_flush(uint32_t);

/**
 * Ekstern referanse til ISR-handlere (Interrupt Service Routines)
 * 
 * Disse funksjonene er definert i interruptServiceRoutines.asm
 * og er de faktiske rutinene som kjøres når et interrupt oppstår.
 * 
 * isr0-isr31: CPU exceptions (f.eks. division by zero, page fault)
 * irq0-irq15: Hardware interrupts (f.eks. tastatur, timer)
 */

// CPU Exceptions (0-31)
extern void isr0(void);  // Division by Zero - Divisjon med null
extern void isr1(void);  // Debug Exception - Debugging-relatert exception
extern void isr2(void);  // Non-maskable Interrupt - Kan ikke maskeres/ignoreres
extern void isr3(void);  // Breakpoint - Brukt for debugging
extern void isr4(void);  // Overflow - Aritmetisk overflow
extern void isr5(void);  // Bound Range Exceeded - Array-indeks utenfor grenser
extern void isr6(void);  // Invalid Opcode - Ugyldig instruksjon
extern void isr7(void);  // Device Not Available - FPU/MMX/SSE ikke tilgjengelig
extern void isr8(void);  // Double Fault - Feil under håndtering av en annen exception
extern void isr9(void);  // Coprocessor Segment Overrun - Sjelden brukt
extern void isr10(void); // Invalid TSS - Problem med Task State Segment
extern void isr11(void); // Segment Not Present - Segment eksisterer ikke
extern void isr12(void); // Stack-Segment Fault - Problem med stack segment
extern void isr13(void); // General Protection Fault - Minnebeskyttelsesfeil
extern void isr14(void); // Page Fault - Feil ved aksess til en side i minnet
extern void isr15(void); // Reserved - Reservert av Intel
extern void isr16(void); // x87 Floating-Point Exception - FPU-feil
extern void isr17(void); // Alignment Check - Minneaksess ikke riktig justert
extern void isr18(void); // Machine Check - Intern CPU-feil
extern void isr19(void); // SIMD Floating-Point Exception - SSE/AVX-feil
extern void isr20(void); // Reserved - Reservert av Intel
extern void isr21(void); // Reserved - Reservert av Intel
extern void isr22(void); // Reserved - Reservert av Intel
extern void isr23(void); // Reserved - Reservert av Intel
extern void isr24(void); // Reserved - Reservert av Intel
extern void isr25(void); // Reserved - Reservert av Intel
extern void isr26(void); // Reserved - Reservert av Intel
extern void isr27(void); // Reserved - Reservert av Intel
extern void isr28(void); // Reserved - Reservert av Intel
extern void isr29(void); // Reserved - Reservert av Intel
extern void isr30(void); // Reserved - Reservert av Intel
extern void isr31(void); // Reserved - Reservert av Intel

// Hardware Interrupts (IRQ 0-15)
extern void irq0(void);  // Timer (PIT) - Programmable Interval Timer
extern void irq1(void);  // Keyboard - PS/2 tastatur
extern void irq2(void);  // Cascade for 8259A Slave controller - Kobling til slave PIC
extern void irq3(void);  // COM2 - Serieport 2
extern void irq4(void);  // COM1 - Serieport 1
extern void irq5(void);  // LPT2 - Parallellport 2
extern void irq6(void);  // Floppy Disk - Diskettstasjon
extern void irq7(void);  // LPT1 / Unreliable "spurious" interrupt - Parallellport 1
extern void irq8(void);  // CMOS Real Time Clock - Sanntidsklokke
extern void irq9(void);  // Free for peripherals - Ledig for periferienheter
extern void irq10(void); // Free for peripherals - Ledig for periferienheter
extern void irq11(void); // Free for peripherals - Ledig for periferienheter
extern void irq12(void); // PS2 Mouse - PS/2 mus
extern void irq13(void); // FPU / Coprocessor / Inter-processor - Flyttallsprosessor
extern void irq14(void); // Primary ATA Hard Disk - Primær harddisk
extern void irq15(void); // Secondary ATA Hard Disk - Sekundær harddisk

/**
 * Legger til en inngang i IDT-tabellen
 * 
 * Denne funksjonen setter opp en enkelt inngang i IDT-tabellen med
 * informasjon om hvilken funksjon som skal håndtere et spesifikt interrupt.
 * 
 * Hver IDT-inngang inneholder:
 * - Adressen til interrupt handler-funksjonen (delt i høy og lav del)
 * - Segment selector (hvilket kodesegment handler-funksjonen ligger i)
 * - Type og attributter (f.eks. om det er en interrupt gate eller trap gate)
 * 
 * Indeksen i IDT-tabellen (0-255)
 * Adressen til handler-funksjonen
 * Segment selector (vanligvis 0x08 for kernel code segment)
 * Type og attributter (f.eks. 0x8E for interrupt gate)
 */
static void idt_add_entry(int index, uint32_t base, uint16_t selector, uint8_t type_attr) 
{
    // Valider parametrene for å unngå korrupsjon av IDT-tabellen
    if (index < 0 || index >= IDT_SIZE) {
        terminal_write_color("ERROR: Ugyldig IDT-indeks\n", COLOR_RED);
        return;
    }
    
    // Sett opp IDT-inngangen
    idt[index].isr_address_low = base & 0xFFFF;          // Nedre 16 bit av handler-adressen
    idt[index].segment_selector = selector;              // Segment selector (vanligvis 0x08)
    idt[index].zero = 0;                                 // Alltid 0 (reservert felt)
    idt[index].type_and_flags = type_attr;               // Type og attributter
    idt[index].isr_address_high = (base >> 16) & 0xFFFF; // Øvre 16 bit av handler-adressen
}

/**
 * Initialiserer Interrupt Descriptor Table (IDT)
 * 
 * Denne funksjonen setter opp hele IDT-tabellen ved å:
 * 1. Sette opp IDT-pekeren
 * 2. Legge til alle CPU exception handlers (0-31)
 * 3. Legge til alle hardware interrupt handlers (32-47)
 * 4. Laste IDT-tabellen inn i CPU-en
 */
void initializer_IDT() 
{
    // Sett opp IDT-pekeren
    idt_info.table_address = (uint32_t)&idt;
    idt_info.table_size = (sizeof(struct idt_entries) * IDT_SIZE) - 1;
    
    // Nullstill IDT-tabellen først
    for (int i = 0; i < IDT_SIZE; i++) {
        idt_add_entry(i, 0, 0x08, 0x8E);
    }
    
    // Legg til CPU exception handlers (0-31)
    // Type 0x8E = 10001110b
    // - Bit 7: P (Present) = 1 (handler er tilgjengelig)
    // - Bit 6-5: DPL (Descriptor Privilege Level) = 00 (kernel-nivå)
    // - Bit 4: S (Storage Segment) = 0 (ikke en storage segment)
    // - Bit 3-0: Type = 1110 (32-bit interrupt gate)
    idt_add_entry(0, (uint32_t)isr0, 0x08, 0x8E);
    idt_add_entry(1, (uint32_t)isr1, 0x08, 0x8E);
    idt_add_entry(2, (uint32_t)isr2, 0x08, 0x8E);
    idt_add_entry(3, (uint32_t)isr3, 0x08, 0x8E);
    idt_add_entry(4, (uint32_t)isr4, 0x08, 0x8E);
    idt_add_entry(5, (uint32_t)isr5, 0x08, 0x8E);
    idt_add_entry(6, (uint32_t)isr6, 0x08, 0x8E);
    idt_add_entry(7, (uint32_t)isr7, 0x08, 0x8E);
    idt_add_entry(8, (uint32_t)isr8, 0x08, 0x8E);
    idt_add_entry(9, (uint32_t)isr9, 0x08, 0x8E);
    idt_add_entry(10, (uint32_t)isr10, 0x08, 0x8E);
    idt_add_entry(11, (uint32_t)isr11, 0x08, 0x8E);
    idt_add_entry(12, (uint32_t)isr12, 0x08, 0x8E);
    idt_add_entry(13, (uint32_t)isr13, 0x08, 0x8E);
    idt_add_entry(14, (uint32_t)isr14, 0x08, 0x8E);
    idt_add_entry(15, (uint32_t)isr15, 0x08, 0x8E);
    idt_add_entry(16, (uint32_t)isr16, 0x08, 0x8E);
    idt_add_entry(17, (uint32_t)isr17, 0x08, 0x8E);
    idt_add_entry(18, (uint32_t)isr18, 0x08, 0x8E);
    idt_add_entry(19, (uint32_t)isr19, 0x08, 0x8E);
    idt_add_entry(20, (uint32_t)isr20, 0x08, 0x8E);
    idt_add_entry(21, (uint32_t)isr21, 0x08, 0x8E);
    idt_add_entry(22, (uint32_t)isr22, 0x08, 0x8E);
    idt_add_entry(23, (uint32_t)isr23, 0x08, 0x8E);
    idt_add_entry(24, (uint32_t)isr24, 0x08, 0x8E);
    idt_add_entry(25, (uint32_t)isr25, 0x08, 0x8E);
    idt_add_entry(26, (uint32_t)isr26, 0x08, 0x8E);
    idt_add_entry(27, (uint32_t)isr27, 0x08, 0x8E);
    idt_add_entry(28, (uint32_t)isr28, 0x08, 0x8E);
    idt_add_entry(29, (uint32_t)isr29, 0x08, 0x8E);
    idt_add_entry(30, (uint32_t)isr30, 0x08, 0x8E);
    idt_add_entry(31, (uint32_t)isr31, 0x08, 0x8E);
    
    // Legg til hardware interrupt handlers (32-47)
    // Disse er mappet til IRQ 0-15 fra PIC
    idt_add_entry(32, (uint32_t)irq0, 0x08, 0x8E);
    idt_add_entry(33, (uint32_t)irq1, 0x08, 0x8E);
    idt_add_entry(34, (uint32_t)irq2, 0x08, 0x8E);
    idt_add_entry(35, (uint32_t)irq3, 0x08, 0x8E);
    idt_add_entry(36, (uint32_t)irq4, 0x08, 0x8E);
    idt_add_entry(37, (uint32_t)irq5, 0x08, 0x8E);
    idt_add_entry(38, (uint32_t)irq6, 0x08, 0x8E);
    idt_add_entry(39, (uint32_t)irq7, 0x08, 0x8E);
    idt_add_entry(40, (uint32_t)irq8, 0x08, 0x8E);
    idt_add_entry(41, (uint32_t)irq9, 0x08, 0x8E);
    idt_add_entry(42, (uint32_t)irq10, 0x08, 0x8E);
    idt_add_entry(43, (uint32_t)irq11, 0x08, 0x8E);
    idt_add_entry(44, (uint32_t)irq12, 0x08, 0x8E);
    idt_add_entry(45, (uint32_t)irq13, 0x08, 0x8E);
    idt_add_entry(46, (uint32_t)irq14, 0x08, 0x8E);
    idt_add_entry(47, (uint32_t)irq15, 0x08, 0x8E);
    
    // Last IDT-tabellen inn i CPU-en
    idt_flush((uint32_t)&idt_info);
    
    terminal_write_color("IDT initialisert med 48 handlers\n", COLOR_GREEN);
}