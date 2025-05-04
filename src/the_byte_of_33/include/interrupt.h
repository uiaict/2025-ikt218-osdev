#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <libc/stdint.h>

// IDT entry structure
struct idt_entry {
    uint16_t base_low;   // Lower 16 bits of handler address
    uint16_t sel;        // Kernel segment selector
    uint8_t  zero;       // Always 0
    uint8_t  flags;      // Flags (e.g., present, privilege level)
    uint16_t base_high;  // Upper 16 bits of handler address
} __attribute__((packed));

// IDT pointer structure
struct idt_ptr {
    uint16_t limit;      // Size of IDT - 1
    uint32_t base;       // Base address of IDT
} __attribute__((packed));

typedef struct registers {
    uint32_t ds;         // Data segment selector
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // General-purpose registers
    uint32_t int_no;     // Interrupt number
    uint32_t err_code;   // Error code (if applicable)
    uint32_t eip;        // Instruction pointer
    uint32_t cs;         // Code segment selector
    uint32_t eflags;     // Flags register
    uint32_t useresp;    // User stack pointer (if applicable)
    uint32_t ss;         // Stack segment selector
} registers_t;

#define IRQ0 32

void init_idt(void);
void init_irq(void);
void isr_handler(uint8_t interrupt);
void irq_handler(uint8_t irq);
void register_interrupt_handler(uint8_t n, void (*handler)(registers_t* r));

// ISR handler declarations for test
void isr0_handler(registers_t* r);
void isr1_handler(registers_t* r);
void isr2_handler(registers_t* r);

#endif