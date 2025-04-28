#pragma once
#include "libc/stdint.h"
#include "libc/isr.h"

typedef void (*irq_handler_t)(void);

void pic_remap(void);
void irq_acknowledge(uint8_t irq);
void irq_install_handler(int irq, irq_handler_t handler);
void irq_handler(registers_t regs);

extern irq_handler_t irq_handlers[16];  // <--- ADD THIS LINE!!
