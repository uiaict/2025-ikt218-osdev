#include "../include/libc/isr.h"
#include "../include/libc/interupts.h"
#include "../include/libc/common.h"
#include "../include/libc/idt.h"

// Include your custom print function header if necessary
#include "libc/teminal.h"  // If kprint is defined here, or create your own header

#include "libc/stdint.h"

#define MAX_IRQ_HANDLERS 256

// The array to store interrupt handlers (ISRs)
isr_t interrupt_handlers[256];

// Exception messages for the first 32 interrupts
char* exception_messages[] = 
{
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",
    "Double fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment not present",
    "Stack fault",
    "General protection fault",
    "Page fault",
    "Unknown Interrupt",
    "Coprocessor Fault",
    "Alignment Fault",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

// The function to handle general ISRs (Interrupt Service Routines)
void isr_handler(registers_t regs)
{
    if (regs.int_no < 32) {
        kprint("Received interrupt %d: %s\n", regs.int_no, exception_messages[regs.int_no]);
    } else {
        kprint("Received interrupt %d\n", regs.int_no);
    }
}

// The function to handle IRQs
void irq_handler(registers_t regs)
{
    if (regs.int_no >= 40)
    {
        outb(0xA0, 0x20);  // Reset the slave PIC
    }
    outb(0x20, 0x20);  // Reset the master PIC

    if (interrupt_handlers[regs.int_no])
    {
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(regs);  // Call the handler for the interrupt
    }
}


// Register an interrupt handler
void register_interrupt_handler(u8int n, isr_t handler)
{
    interrupt_handlers[n] = handler;
}
