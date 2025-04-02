#ifndef IDT_H
#define IDT_H

#include <libc/stdint.h>
#include <libc/stdbool.h>

void idt_init(void);
void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags);
void idt_register_handler(uint8_t vector, void* isr_handler);
void idt_set_interrupt_gate(uint8_t vector, void* isr_handler);
#endif /*IDT_H*/ 
