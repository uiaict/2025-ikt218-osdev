#ifndef ISR_H
#define ISR_H

// IRQ hander dispatcher
void irq_handler(int irq);

extern void* isr_stubs[256];


#endif // ISR_H