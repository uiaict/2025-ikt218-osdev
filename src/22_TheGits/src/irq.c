#include "libc/irq.h"
#include"libc/isr_handlers.h"



void register_irq_handler(int irq, void (*handler)(void)) {
    if (irq >= 0 && irq < IRQ_COUNT) {
        irq_handlers[irq] = handler;
    }
}

void unregister_irq_handler(int irq) {
    if (irq >= 0 && irq < IRQ_COUNT) {
        irq_handlers[irq] = 0;
    }
}


void irq_handler(int irq) {

    if (irq < IRQ_COUNT && irq_handlers[irq]) {
        irq_handlers[irq]();
    } else {
        printf("Unhandled IRQ %d\n", irq);
    }

    send_eoi(irq);
}


void init_irq() {
    remap_pic(); 
    register_irq_handler(1, handle_keyboard_interrupt); 
    register_irq_handler(0, handle_timer_interrupt);
    //register_irq_handler(2, handle_syscall);
}