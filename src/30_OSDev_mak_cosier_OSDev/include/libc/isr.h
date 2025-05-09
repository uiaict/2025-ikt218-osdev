#ifndef ISR_H
#define ISR_H

#include "libc/stdint.h"
#include "libc/common.h"  // For inb() and outb()

// Define registers_t with typedef so you can use it directly
typedef struct registers_t 
{
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
} registers_t;

// IRQ definitions (hardware interrupts remapped from 32 to 47)
#define IRQ0  32
#define IRQ1  33
#define IRQ2  34
#define IRQ3  35
#define IRQ4  36
#define IRQ5  37
#define IRQ6  38
#define IRQ7  39
#define IRQ8  40
#define IRQ9  41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

// Function pointer type for ISR/IRQ handlers
typedef void (*isr_t)(registers_t);  // Use registers_t *regs consistently

// Function declarations
void isr_handler(registers_t regs);
void irq_handler(registers_t regs);
void register_interrupt_handler(u8int n, isr_t handler);

#endif
