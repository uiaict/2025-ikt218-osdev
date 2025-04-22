/* Adapted from the James Molloy's GDT/IDT implementation totorial at https://archive.is/L3pyA */
#ifndef IDT_H
#define IDT_H

#include "libc/stdint.h"

/* IDT structures */
struct idt_entry_struct {
    uint16_t base_low;     // Lower 16 bits of handler address
    uint16_t sel;          // Kernel segment selector
    uint8_t always0;       // Always zero
    uint8_t flags;         // Flags
    uint16_t base_high;    // Upper 16 bits of handler address
} __attribute__((packed));
typedef struct idt_entry_struct idt_entry_t;

struct idt_ptr_struct {
    uint16_t limit;        // Size of IDT minus 1
    uint32_t base;         // Address of first IDT entry
} __attribute__((packed));
typedef struct idt_ptr_struct idt_ptr_t;

// Function to initialize the IDT
void init_idt();

// ISRs for CPU exceptions
extern void isr0();  // Division by zero
extern void isr1();  // Debug
extern void isr2();  // Non-maskable interrupt
extern void isr3();  // Breakpoint exception
extern void isr4();  // 'Into detected overflow'
extern void isr5();  // Out of bounds exception
extern void isr6();  // Invalid opcode
extern void isr7();  // No coprocessor
extern void isr8();  // Double fault
extern void isr9();  // Coprocessor segment overrun
extern void isr10(); // Bad TSS
extern void isr11(); // Segment not present
extern void isr12(); // Stack fault
extern void isr13(); // General protection fault
extern void isr14(); // Page fault
extern void isr15(); // Unknown interrupt exception
extern void isr16(); // Coprocessor fault
extern void isr17(); // Alignment check exception
extern void isr18(); // Machine check exception
extern void isr19(); // 19-31 reserved
extern void isr20();  
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

// IRQs
extern void irq0();  // IRQ0
extern void irq1();  // IRQ1
extern void irq2();  // IRQ2
extern void irq3();  // IRQ3
extern void irq4();  // IRQ4
extern void irq5();  // IRQ5
extern void irq6();  // IRQ6
extern void irq7();  // IRQ7
extern void irq8();  // IRQ8
extern void irq9();  // IRQ9
extern void irq10(); // IRQ10
extern void irq11(); // IRQ11  
extern void irq12(); // IRQ12
extern void irq13(); // IRQ13
extern void irq14(); // IRQ14
extern void irq15(); // IRQ15

// External assembly function
extern void idt_flush(uint32_t);

#endif // IDT_H