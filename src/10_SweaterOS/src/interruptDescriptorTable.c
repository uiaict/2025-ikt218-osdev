#include "libc/stdint.h"
#include "descriptorTables.h"
#include "miscFuncs.h"
#include "interruptHandler.h"
#include "display.h"

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

// IDT size (256 entries - maximum number of interrupts in x86)
#define IDT_SIZE 256

// IDT table - array of IDT entries
static struct idt_entries idt_entries[IDT_SIZE];

// IDT pointer - tells CPU where IDT is in memory
static struct idt_pointer idt_ptr;

// Assembly function that loads IDT into CPU
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
extern void isr0(void);  // Division by Zero
extern void isr1(void);  // Debug Exception
extern void isr2(void);  // Non-maskable Interrupt
extern void isr3(void);  // Breakpoint
extern void isr4(void);  // Overflow
extern void isr5(void);  // Bound Range Exceeded
extern void isr6(void);  // Invalid Opcode
extern void isr7(void);  // Device Not Available
extern void isr8(void);  // Double Fault
extern void isr9(void);  // Coprocessor Segment Overrun
extern void isr10(void); // Invalid TSS
extern void isr11(void); // Segment Not Present
extern void isr12(void); // Stack-Segment Fault
extern void isr13(void); // General Protection Fault
extern void isr14(void); // Page Fault
extern void isr15(void); // Reserved
extern void isr16(void); // x87 Floating-Point Exception
extern void isr17(void); // Alignment Check
extern void isr18(void); // Machine Check
extern void isr19(void); // SIMD Floating-Point Exception
extern void isr20(void); // Reserved
extern void isr21(void); // Reserved
extern void isr22(void); // Reserved
extern void isr23(void); // Reserved
extern void isr24(void); // Reserved
extern void isr25(void); // Reserved
extern void isr26(void); // Reserved
extern void isr27(void); // Reserved
extern void isr28(void); // Reserved
extern void isr29(void); // Reserved
extern void isr30(void); // Reserved
extern void isr31(void); // Reserved

// Hardware Interrupts (IRQ 0-15)
extern void irq0(void);  // Timer (PIT)
extern void irq1(void);  // Keyboard
extern void irq2(void);  // Cascade for 8259A Slave controller
extern void irq3(void);  // COM2
extern void irq4(void);  // COM1
extern void irq5(void);  // LPT2
extern void irq6(void);  // Floppy Disk
extern void irq7(void);  // LPT1 / Unreliable "spurious" interrupt
extern void irq8(void);  // CMOS Real Time Clock
extern void irq9(void);  // Free for peripherals
extern void irq10(void); // Free for peripherals
extern void irq11(void); // Free for peripherals
extern void irq12(void); // PS2 Mouse
extern void irq13(void); // FPU / Coprocessor / Inter-processor
extern void irq14(void); // Primary ATA Hard Disk
extern void irq15(void); // Secondary ATA Hard Disk

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
    if (index < 0 || index >= IDT_SIZE) {
        display_write_color("ERROR: Ugyldig IDT-indeks\n", COLOR_RED);
        return;
    }
    
    idt_entries[index].isr_address_low = base & 0xFFFF;
    idt_entries[index].segment_selector = selector;
    idt_entries[index].zero = 0;
    idt_entries[index].type_and_flags = type_attr;
    idt_entries[index].isr_address_high = (base >> 16) & 0xFFFF;
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
    idt_ptr.table_address = (uint32_t)&idt_entries;
    idt_ptr.table_size = (sizeof(struct idt_entries) * IDT_SIZE) - 1;
    
    // Nullstill IDT-tabellen
    for (int i = 0; i < IDT_SIZE; i++) {
        idt_add_entry(i, 0, 0x08, 0x8E);
    }
    
    // Legger til CPU exception handlers (0-31)
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
    
    idt_flush((uint32_t)&idt_ptr);
    
    display_write_color("IDT initialized with 48 handlers\n", COLOR_GREEN);
}