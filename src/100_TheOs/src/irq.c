// irq.c
#include "irq.h"
#include "common.h"
#include "libc/stddef.h"

#define IRQ_COUNT 16

// IRQ handler structure
struct irq_handler_t {
    isr_t handler;
    void* data;
    uint8_t num;
};

// Array of IRQ handlers
static struct irq_handler_t irq_handlers[IRQ_COUNT];

// Initialize IRQ subsystem
void init_irq() {
    for (int i = 0; i < IRQ_COUNT; i++) {
        irq_handlers[i].handler = NULL;
        irq_handlers[i].data = NULL;
        irq_handlers[i].num = i;
    }
    
    // Initialize the PIC
    // ICW1: Initialize both PICs
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    
    // ICW2: Set interrupt offsets
    outb(0x21, 0x20);  // Master PIC: IRQ0-7 -> INT 0x20-0x27 (32-39)
    outb(0xA1, 0x28);  // Slave PIC: IRQ8-15 -> INT 0x28-0x2F (40-47)
    
    // ICW3: Configure master/slave relationship
    outb(0x21, 0x04);  // Tell master PIC slave is at IRQ2
    outb(0xA1, 0x02);  // Tell slave PIC its identity (2)
    
    // ICW4: Set mode
    outb(0x21, 0x01);  // 8086/88 mode
    outb(0xA1, 0x01);  // 8086/88 mode
    
    // OCW1: Unmask keyboard IRQ
    outb(0x21, inb(0x21) & ~(1 << 1));  // Enable IRQ1 (keyboard)
    outb(0xA1, 0xFF);  // Mask all slave interrupts for now
}

// Register an IRQ handler
void register_irq_handler(uint8_t irq, isr_t handler, void* context) {
    if (irq < IRQ_COUNT) {
        irq_handlers[irq].handler = handler;
        irq_handlers[irq].data = context;
    }
}

// Alias for backward compatibility
void register_irq_controller(int irq, isr_t controller, void* ctx) {
    register_irq_handler(irq, controller, ctx);
}

// IRQ controller function
// This function is called when an IRQ is triggered
void irq_controller(registers_t* regs) {
    // For IRQs, the value should be between 32-47 (32+IRQ number)
    // If it's outside this range, something is wrong
    if (regs->int_no < 32 || regs->int_no > 47) {
        outb(0x20, 0x20);
        return;
    }
   
    // Send EOI to PICs
    if (regs->int_no >= 40) {
        outb(0xA0, 0x20);  // Send EOI to slave PIC
    }
    outb(0x20, 0x20);      // Send EOI to master PIC

    // Get IRQ number (interrupt number - 32)
    uint8_t irq = regs->int_no - 32;
   
    // Call handler if registered
    if (irq < IRQ_COUNT && irq_handlers[irq].handler != NULL) {
        irq_handlers[irq].handler(regs, irq_handlers[irq].data);
    }
}

// Start IRQ system - public API function
void start_irq() {
    init_irq();
}