#ifndef ISR_H
#define ISR_H

#include "libc/stdint.h"
#include "libc/gdt_idt_table.h"

#ifdef __cplusplus
extern "C" {
#endif

#define IDT_ENTRIES 256
extern void* isr_stub_table[256];


// Provide access to isr_stub_table from assembly
extern void* isr_stub_table[IDT_ENTRIES];

// PIC Remapped IRQ Base
#define IRQ_BASE 0x20

// IRQ Definitions
enum {
    IRQ0 = 32,  // Timer
    IRQ1 = 33,  // Keyboard
    IRQ2 = 34,  // Cascade (never raised)
    IRQ3 = 35,  // COM2
    IRQ4 = 36,  // COM1
    IRQ5 = 37,  // LPT2
    IRQ6 = 38,  // Floppy Disk
    IRQ7 = 39,  // LPT1 (Spurious interrupt)
    IRQ8 = 40,  // CMOS Real-Time Clock
    IRQ9 = 41,  // Free for peripherals
    IRQ10 = 42, // Free for peripherals
    IRQ11 = 43, // Free for peripherals
    IRQ12 = 44, // PS/2 Mouse
    IRQ13 = 45, // FPU/Coprocessor
    IRQ14 = 46, // Primary ATA Hard Disk
    IRQ15 = 47  // Secondary ATA Hard Disk
};

typedef struct registers {
    uint32_t ds;
    uint32_t edi, esi, ebp;
    uint32_t esp;
    uint32_t ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
} registers_t;

typedef void (*isr_t)(registers_t*, void*);

void register_interrupt_handler(uint8_t n, isr_t handler, void* context);
void register_irq_handler(uint8_t irq, isr_t handler, void* context);

void isr_handler(registers_t* regs);
void irq_handler(registers_t* regs);

#ifdef __cplusplus
}
#endif

#endif // ISR_H
