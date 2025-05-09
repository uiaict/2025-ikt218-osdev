#include "interrupts.h"
#include "common.h"
#include "libc/stdint.h"
#include "libc/stddef.h"
#include "terminal.h"
#include "idt.h"

void register_interrupt_handler(uint8_t n, isr_t handler, void* context)
{
    int_handlers[n].handler = handler;
    int_handlers[n].data = context;
}

char* exception_messages[] = {
    "Division Error",
    "Debug Exception",
    "NMI interrupt",
    "Breakpoint",
    "Overflow",
    "BOUND Range Exceeded",
    "Invalid Opcode",
    "Device Not available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not present",
    "Stack-Segment Fault",
    "General Protection",
    "Page fault",
    "Reserved",
    "Floating Point Error",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating Point Exception",
    "Virtualization Exception",
    "Control Protection Exception",
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

// This gets called from our ASM interrupt handler stub.
void isr_handler(registers_t regs)
{
    if(regs.int_no < 32)
    {
        terminal_write_colored("CPU Exception: ", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
        terminal_writestring(exception_messages[regs.int_no]);
        terminal_putchar('\n');
    }

    // This line is important. When the processor extends the 8-bit interrupt number
    // to a 32bit value, it sign-extends, not zero extends. So if the most significant
    // bit (0x80) is set, regs.int_no will be very large (about 0xffffff80).
    /*uint8_t int_no = regs.int_no & 0xFF;
    struct int_handler_t intrpt = int_handlers[int_no];
    if (intrpt.handler != 0)
    {
        intrpt.handler(&regs, intrpt.data);
    }
    else
    {
        /*monitor_write("unhandled interrupt: ");
        monitor_write_hex(int_no);
        monitor_put('\n');*/
     /*   for(;;);
    }*/
}

