#ifndef IDT_H
#define IDT_H

#include <libc/stdint.h>


void idt_flush(uint32_t);
void idt_set_gate(uint8_t , uint32_t , uint16_t , uint8_t );

extern void isr0();
extern void isr1();
extern void isr2();

void init_idt();
#endif
