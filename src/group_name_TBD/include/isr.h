#ifndef ISR_H
#define ISR_H

#include "libc/stdint.h"

#define M_PIC 0x20  // IO base address for master PIC
#define M_PIC_COMMAND M_PIC
#define M_PIC_DATA (M_PIC+1)

#define S_PIC 0xA0  // IO base address for slave PIC
#define S_PIC_COMMAND S_PIC
#define S_PIC_DATA (S_PIC+1)

#define PIC_EOI 0x20

// Map IRQ to ISR number so that we can call
// register_interrupt_handler(IRQ1, keyboard_handler)
#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40 // Slave starts here
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47


struct registers{
    uint32_t ds;                                        // Data segment selector
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;    // Pushed by pusha.
    uint32_t int_no, err_code;                          // Interrupt number and error code (if applicable)
    uint32_t eip, cs, eflags, useresp, ss;              // Pushed by the processor automatically.
};


void register_interrupt_handler(uint8_t, void (*)(struct registers)); 
void isr_handler(struct registers);
void irq_handler(struct registers);

#endif // ISR_H