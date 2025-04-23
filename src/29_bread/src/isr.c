// isr.c
#include <libc/stdio.h>
#include <libc/stdint.h>
#include <print.h>
#include <libc/isr.h>
#include <libc/common.h>

isr_t interrupt_handlers[256];

void register_interrupt_handler(uint8_t n, isr_t handler)
{
  interrupt_handlers[n] = handler;
}

// Match the declaration in isr.h
void isr_handler(registers_t regs) {
    printf("Received interrupt: %d, Error code: %d\n", regs.int_no, regs.err_code);
    
    // Halt the system if a critical exception occurs
    if (regs.int_no <= 31) {
        printf("SYSTEM HALTED: Exception %d\n", regs.int_no);
        for(;;); // Halt the CPU
    }
}

void irq_handler(registers_t regs)
{
    // Call our handle_irq function
    handle_irq(regs);
    
    // Note: Don't send EOI here, as it's now handled in handle_irq
}
