#ifndef ISR_H
#define ISR_H

// IRQ hander dispatcher
void irq_handler(int irq);

extern void irq0_stub();

#endif // ISR_H