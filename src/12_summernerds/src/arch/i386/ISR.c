#include "i386/interruptRegister.h"
#include "libc/stdint.h"
#include "libc/stddef.h"

void register_interrupt_handler(uint8_t n, isr_t handler, void *context)
{
    int_handlers[n].handler = handler;
    int_handlers[n].data = context;
}

// This is the main interrupt handler function. It is called by the ASM interrupt handler stub
void isr_handler(registers_t regs)
{
    uint8_t int_no = regs.int_no & 0xFF; // Sign extend the interrupt number
    struct int_handler_t intrpt = int_handlers[int_no];
    if (intrpt.handler != 0)
    {
        intrpt.handler(&regs, intrpt.data);
    }
    else
    {
        printf("Unhandled interrupt.%d", int_no);
        while (true)
            ; // Infinite loop if no handler is registered
    }
}

void testISR3(registers_t *regs, void *context)
{
    printf("this is printed when isr 3 is activated\n");
}
void testISR4(registers_t *regs, void *context)
{
    printf("this is printed when isr 4 is activated\n");
}
void testISR5(registers_t *regs, void *context)
{
    printf("this is printed when isr 5 is activated\n");
}

void testThreeISRs()
{
    register_interrupt_handler(ISR3, testISR3, 0);
    register_interrupt_handler(ISR4, testISR4, 0);
    register_interrupt_handler(ISR5, testISR5, 0);

    asm volatile("sti");
    asm volatile("int $0x3");
    asm volatile("int $0x4");
    asm volatile("int $0x5");
}
