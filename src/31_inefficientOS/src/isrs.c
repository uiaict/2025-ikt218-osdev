#include "interrupts.h"
#include "terminal.h"

// Custom ISR handler for interrupt 40
void custom_isr_40_handler(registers_t* regs, void* data) {
    terminal_write_colored("Custom ISR 40 triggered!\n", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
}

// Custom ISR handler for interrupt 41
void custom_isr_41_handler(registers_t* regs, void* data) {
    terminal_write_colored("Custom ISR 41 triggered!\n", VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
}

// Custom ISR handler for interrupt 42
void custom_isr_42_handler(registers_t* regs, void* data) {
    terminal_write_colored("Custom ISR 42 triggered!\n", VGA_COLOR_LIGHT_MAGENTA, VGA_COLOR_BLACK);
}

// Function to initialize our custom ISRs
void custom_isrs_init() {
    // Register our three custom ISR handlers
    register_interrupt_handler(40, custom_isr_40_handler, NULL);
    register_interrupt_handler(41, custom_isr_41_handler, NULL);
    register_interrupt_handler(42, custom_isr_42_handler, NULL);
    
    terminal_writestring("Custom ISRs initialized successfully!\n");
}