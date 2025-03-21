
#include "idt.h"
#include "isr.h"
#include "kprint.h"

// Implementation of exception_handler with noreturn attribute
// Make sure this matches the declaration in isr.h
__attribute__((noreturn))
void exception_handler(uint8_t int_no) {
    kprint("Exception occurred! Interrupt: ");
    kprint_hex(int_no);  // Print interrupt number
    kprint("\n");

    __asm__ volatile ("cli; hlt");  // Halt the system
    // This function doesn't actually return, as we've halted the system
    for(;;); // Add an infinite loop to satisfy the noreturn attribute
}

void isr_divide_by_zero() {
    kprint("Divide by zero error!\n");
    __asm__ volatile ("cli; hlt"); // Halt the system
    for(;;); // Infinite loop
}

void isr_invalid_opcode() {
    kprint("Invalid Opcode Exception!\n");
    __asm__ volatile ("cli; hlt"); // Halt the system
    for(;;); // Infinite loop
}

void isr_keyboard() {
    kprint("Keyboard interrupt received!\n");
    // Acknowledge the interrupt (for PIC)
    outb(0x20, 0x20);
}

// Add the outb function if it's not defined elsewhere
void outb(uint16_t port, uint8_t data) {
    __asm__ volatile("outb %0, %1" : : "a" (data), "Nd" (port));
}