#include "interrupts.h"
#include "libc/system.h"

// Fetches terminal printf function
extern void terminal_printf(const char* format, ...);

// Division by zero controller
// This function is called when a division by zero interrupt occurs
void division_by_zero_controller(registers_t* regs, void* context) {
    terminal_printf("Interrupt 0: Division by Zero Error\n");
    return;
}
// Debug controller
// This function is called when a debug interrupt occurs
void debug_controller(registers_t* regs, void* context) {
    terminal_printf("Interrupt 1: Debug Exception\n");
    return;
}
// Non-Maskable Interrupt controller
// This function is called when a non-maskable interrupt occurs
void nmi_controller(registers_t* regs, void* context) {
    terminal_printf("Interrupt 2: Non-Maskable Interrupt\n");
    return;
}

// Starts the interrupt service routine (ISR) controllers
void start_isr_controllers() {
    load_interrupt_controller(0, division_by_zero_controller, NULL);
    load_interrupt_controller(1, debug_controller, NULL);
    load_interrupt_controller(2, nmi_controller, NULL);
    
    terminal_printf("ISR controllers initialized.\n");
}

// Function to test ISRs
void test_interrupts() {
    terminal_printf("Testing interrupts...\n");
    
    asm volatile("int $0x0");
    
    asm volatile("int $0x1");
    
    asm volatile("int $0x2");
    
    terminal_printf("Interrupt testing complete.\n");
}