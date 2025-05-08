#ifndef IRQ_H
#define IRQ_H

#include "libc/stdint.h"

// Extern declarations for IRQ stubs (in irq_asm.asm)
extern void irq0(void);
extern void irq1(void);
extern void irq2(void);
extern void irq3(void);
extern void irq4(void);
extern void irq5(void);
extern void irq6(void);
extern void irq7(void);
extern void irq8(void);
extern void irq9(void);
extern void irq10(void);
extern void irq11(void);
extern void irq12(void);
extern void irq13(void);
extern void irq14(void);
extern void irq15(void);

// Initialize PICs and remap IRQs 0–15 to IDT entries 32–47
void irq_install(void);

// Common C handler for all IRQs (sends EOI and prints a message)
void irq_handler(int irq_number);

// PS/2 keyboard handler for IRQ1
void keyboard_handler(void);

#endif // IRQ_H
