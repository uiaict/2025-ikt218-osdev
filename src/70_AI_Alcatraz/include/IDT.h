#ifndef IDT_H
#define IDT_H

#include "libc/stdint.h"

// IDT entry structure
struct idt_entry {
    uint16_t base_low;     // Lower 16 bits of handler function address
    uint16_t selector;     // Kernel segment selector
    uint8_t zero;          // Always zero
    uint8_t flags;         // Flags
    uint16_t base_high;    // Upper 16 bits of handler function address
} __attribute__((packed));

// IDT pointer structure
struct idt_ptr {
    uint16_t limit;       // Size of IDT - 1
    uint32_t base;        // Base address of IDT
} __attribute__((packed));

// Define ISR functions that will be implemented in assembly
extern void isr0();  // Division by zero
extern void isr1();  // Debug
extern void isr2();  // Non-maskable interrupt
extern void isr3();  // Breakpoint
extern void isr4();  // Overflow
extern void isr5();  // Bound range exceeded
extern void isr6();  // Invalid opcode
extern void isr7();  // Device not available
extern void isr8();  // Double fault
extern void isr9();  // Coprocessor segment overrun
extern void isr10(); // Invalid TSS
extern void isr11(); // Segment not present
extern void isr12(); // Stack-segment fault
extern void isr13(); // General protection fault
extern void isr14(); // Page fault
extern void isr15(); // Reserved
extern void isr16(); // x87 FPU error
extern void isr17(); // Alignment check
extern void isr18(); // Machine check
extern void isr19(); // SIMD floating-point exception
extern void isr20(); // Virtualization exception
extern void isr21(); // Reserved
extern void isr22(); // Reserved
extern void isr23(); // Reserved
extern void isr24(); // Reserved
extern void isr25(); // Reserved
extern void isr26(); // Reserved
extern void isr27(); // Reserved
extern void isr28(); // Reserved
extern void isr29(); // Reserved
extern void isr30(); // Reserved
extern void isr31(); // Reserved

// Define IRQ functions that will be implemented in assembly
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

// Struct for registers saved by interrupt
typedef struct {
    uint32_t ds;                                     // Data segment selector
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha
    uint32_t int_no, err_code;                       // Interrupt number and error code
    uint32_t eip, cs, eflags, useresp, ss;           // Pushed by the processor automatically
} registers_t;

// Function pointer for interrupt handlers
typedef void (*isr_t)(registers_t*);

// Function prototypes
void idt_init();
void irq_init();
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);
void register_interrupt_handler(uint8_t n, isr_t handler);

// Assembly function to load the IDT
extern void idt_flush(uint32_t idt_ptr);

#endif // IDT_H
