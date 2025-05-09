#include <libc/irq.h>
#include <libc/common.h>
#include <print.h>

// Array of function pointers for IRQ handlers
static irq_handler_func_t irq_handlers[IRQ_COUNT];

void init_irq(void) {
    // Initialize all handlers to NULL
    for (uint8_t i = 0; i < IRQ_COUNT; i++) {
        irq_handlers[i] = 0;
    }
    
    printf("IRQ system initialized\n");
}

void register_irq_handler(uint8_t irq, irq_handler_func_t handler) {
    if (irq < IRQ_COUNT) {
        irq_handlers[irq] = handler;
        printf("IRQ handler registered for IRQ %d\n", irq);
    }
}

void irq_ack(uint8_t irq) {
    // If this interrupt involved the slave PIC (IRQs 8-15),
    // send EOI to the slave PIC as well
    if (irq >= 8) {
        outb(0xA0, 0x20); // Send EOI to slave PIC
    }
    
    // Send EOI to master PIC
    outb(0x20, 0x20);
}

void handle_irq(registers_t regs) {
    // Get the IRQ number (subtract 32 as IRQs start at 32 in the IDT)
    uint8_t irq_num = regs.int_no - 32;
    
    // Call the registered handler if it exists
    if (irq_num < IRQ_COUNT && irq_handlers[irq_num] != 0) {
        irq_handler_func_t handler = irq_handlers[irq_num];
        handler(regs);
    } else {
        // Only print for unhandled IRQs (optional, for debugging)
        if (irq_num < IRQ_COUNT) {
            //printf("Received unhandled IRQ: %d\n", irq_num);
        }
    }
    
    // Send EOI to acknowledge the interrupt
    irq_ack(irq_num);
}