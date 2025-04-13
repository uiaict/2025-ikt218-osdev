#include "interrupts.h"
#include "libc/system.h"

// Forward declaration of terminal_printf from your kernel
extern void terminal_printf(const char* format, ...);

// controllers for specific interrupts
void division_by_zero_controller(registers_t* regs, void* context) {
    terminal_printf("Interrupt 0: Division by Zero Error\n");
}

void debug_controller(registers_t* regs, void* context) {
    terminal_printf("Interrupt 1: Debug Exception\n");
}

void nmi_controller(registers_t* regs, void* context) {
    terminal_printf("Interrupt 2: Non-Maskable Interrupt\n");
}

// Initialize the ISR controllers
void start_isr_controllers() {
    // Register at least three specific controllers
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