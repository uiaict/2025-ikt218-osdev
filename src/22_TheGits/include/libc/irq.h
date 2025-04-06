#include "libc/idt.h"
#include "libc/scrn.h"
#include "libc/stdint.h"
#include "libc/isr_handlers.h"
#include "libc/io.h"

#define IRQ_COUNT 16

static void (*irq_handlers[IRQ_COUNT])(void) = {0};

void register_irq_handler(int irq, void (*handler)(void));

void unregister_irq_handler(int irq);


void irq_handler(int irq);
void send_eoi(uint8_t irq);

void init_irq();