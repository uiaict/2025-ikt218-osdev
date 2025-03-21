#ifndef ISR_H
#define ISR_H

#include <libc/stdint.h>

// Exception handler declaration with noreturn attribute
__attribute__((noreturn))
void exception_handler(uint8_t int_no);

// ISR handler function declarations
void isr_divide_by_zero(void);
void isr_invalid_opcode(void);
void isr_keyboard(void);

// I/O port functions
void outb(uint16_t port, uint8_t data);
uint8_t inb(uint16_t port);

#endif /* ISR_H */