#include "libc/isr.h"
#include "libc/common.h"
#include "libc/stdio.h"
#include "libc/system.h"
#include "libc/stdint.h"
#include "libc/print.h"
#include "io.h"

// Array of registered interrupt handlers
static isr_t interrupt_handlers[256];
// Array to store context pointers for each handler
static void* handler_contexts[256];

void isr_handler(registers_t* regs) {
    // Get the context for this interrupt if it exists
    void* context = handler_contexts[regs->int_no];
    
    if (interrupt_handlers[regs->int_no] != NULL) {
        // Call the registered handler with context
        interrupt_handlers[regs->int_no](regs, context);
    } else {
        printf("Unhandled interrupt: %d\n", regs->int_no);
        // You might want to halt or handle unregistered interrupts here
    }
}

void irq_handler(registers_t* regs) {
    // Get the context for this IRQ if it exists
    void* context = handler_contexts[regs->int_no];
    
    if (interrupt_handlers[regs->int_no] != NULL) {
        // Call the registered handler with context
        interrupt_handlers[regs->int_no](regs, context);
    }
    
    // Send End of Interrupt (EOI) to PICs
    if (regs->int_no >= 40) {
        outb(0xA0, 0x20); // Send EOI to slave PIC
    }
    outb(0x20, 0x20); // Always send EOI to master PIC
}

void register_interrupt_handler(uint8_t n, isr_t handler, void* context) {
    if (n >= 255) {  // Changed from >= 256
        return;
    }
    interrupt_handlers[n] = handler;
    handler_contexts[n] = context;
}

void register_irq_handler(uint8_t irq, isr_t handler, void* context) {
    // IRQs 0-15 map to interrupts 32-47
    uint8_t interrupt_num = irq + 32;
    
    // Validate the IRQ number
    if (irq > 15) {
        printf("Invalid IRQ number: %d\n", irq);
        return;
    }
    
    register_interrupt_handler(interrupt_num, handler, context);
}