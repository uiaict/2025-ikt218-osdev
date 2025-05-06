// interrupts.c
#include "libc/interrupts.h"
#include "libc/io.h"  // For port I/O (you can remove it if not using it)
#include "libc/idt.h"  // For IDT (Interrupt Descriptor Table)
#include "libc/pit.h"  // If you're working with PIT (Programmable Interval Timer)

// Define a global array of interrupt handlers
#define MAX_INTERRUPTS 256
void (*interrupt_handlers[MAX_INTERRUPTS])(void);

// Function to register an interrupt handler for a given IRQ
void register_interrupt_handler(int irq, void (*handler)()) {
    if (irq < 0 || irq >= MAX_INTERRUPTS) {
        // Invalid IRQ number
        return;
    }
    interrupt_handlers[irq] = handler;
}
// Just register the handler in the appropriate function
void register_handlers() {
    register_interrupt_handler(32, pit_handler);  // Register the PIT handler
}

// Example interrupt handler function (you can create your own specific handlers)
void interrupt_handler() {
    // Handle interrupt here
    // You can use interrupt_handlers[interrupt_number] to call specific ISRs
}

// Initialize the IDT and set up basic interrupt handling
void init_interrupts() {
    // Initialize the IDT (Interrupt Descriptor Table)
    //init_idt();  // This function should be defined in your idt.c file

    // Register handlers for specific IRQs
    // For example, IRQ 0 for the PIT (Programmable Interval Timer)
    register_interrupt_handler(32, pit_handler);  // IRQ 0 is mapped to interrupt 32 in the IDT
    // Register more interrupt handlers as needed
}

// Example function to handle a specific interrupt (e.g., PIT)
void pit_handler() {
    // This is an example handler for the PIT interrupt (IRQ 0)
    // You can implement code to handle the timer interrupt here
    // For example, you could increment a tick counter or handle scheduling
}
