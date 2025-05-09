// irq.c
#include "irq.h"
#include "common.h"
#include "libc/stddef.h"

#define IRQ_COUNT 16

struct irq_handler_t {
    isr_t handler;
    void* data;
    uint8_t num;
};

static struct irq_handler_t irq_handlers[IRQ_COUNT];

void init_irq() {
    for (int i = 0; i < IRQ_COUNT; i++) {
        irq_handlers[i].handler = NULL;
        irq_handlers[i].data = NULL;
        irq_handlers[i].num = i;
    }
    
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    
    outb(0x21, 0x20);  
    outb(0xA1, 0x28);  
    
    outb(0x21, 0x04);  
    outb(0xA1, 0x02);  
    
    outb(0x21, 0x01); 
    outb(0xA1, 0x01); 
    
    outb(0x21, inb(0x21) & ~(1 << 1));  // Enable IRQ1 (keyboard)
    outb(0xA1, 0xFF);  //Mask all slave IRQs
}

void register_irq_handler(uint8_t irq, isr_t handler, void* context) {
    if (irq < IRQ_COUNT) {
        irq_handlers[irq].handler = handler;
        irq_handlers[irq].data = context;
    }
}

void register_irq_controller(int irq, isr_t controller, void* ctx) {
    register_irq_handler(irq, controller, ctx);
}

void irq_controller(registers_t* regs) {
    if (regs->int_no < 32 || regs->int_no > 47) {
        outb(0x20, 0x20);
        return;
    }
   
    if (regs->int_no >= 40) {
        outb(0xA0, 0x20);  
    }
    outb(0x20, 0x20);     

    uint8_t irq = regs->int_no - 32;
   
    if (irq < IRQ_COUNT && irq_handlers[irq].handler != NULL) {
        irq_handlers[irq].handler(regs, irq_handlers[irq].data);
    }
}

void start_irq() {
    init_irq();
}