#ifndef IRQ_H
#define IRQ_H

#include "isr.h"

void irq_install();
void irq_handler(struct regs *r);

#endif
