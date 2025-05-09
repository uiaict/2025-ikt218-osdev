#ifndef IRQ_H
#define IRQ_H

#include <stdint.h>

struct isr_regs;

void irq_handler(struct isr_regs* regs);
void irq_install(void);

#endif
