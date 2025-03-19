#include "idt.h"
#include "isr.h"
#include "kprint.h"

__attribute__((noreturn))
void exception_handler(void);
void exception_handler(uint8_t int_no) {
    kprint("Exception occurred! Interrupt: ");
    kprint_hex(int_no);  // Print interrupt number
    kprint("\n");

    __asm__ volatile ("cli; hlt");  // Halt the system
}

void isr_divide_by_zero() {
    kprint("Divide by zero error!\n");
    __asm__ volatile ("cli; hlt"); // Halt the system
}

void isr_invalid_opcode() {
    kprint("Invalid Opcode Exception!\n");
    __asm__ volatile ("cli; hlt"); // Halt the system
}

void isr_keyboard() {
    kprint("Keyboard interrupt received!\n");
    // Acknowledge the interrupt (for PIC)
    outb(0x20, 0x20);
}
