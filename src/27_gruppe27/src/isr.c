#include "interrupts.h"
#include "libc/stdint.h"
#include "libc/stddef.h"
#include "monitor.h"

struct int_handler_t int_handlers[IDT_ENTRIES];
struct int_handler_t irq_handlers[IRQ_COUNT];


void register_interrupt_handler(uint8_t n, isr_t handler, void* context)
{
    int_handlers[n].handler = handler;
    int_handlers[n].data = context;
}

void isr_custom0(registers_t* regs, void* ctx) {
    printf("ISR triggered: 0\n");
}

void isr_custom1(registers_t* regs, void* ctx) {
    printf("ISR triggered: 1\n");
}

void isr_custom2(registers_t* regs, void* ctx) {
    printf("ISR triggered: 2\n");
}

// This gets called from our ASM interrupt handler stub.
void isr_handler(registers_t regs)
{
    // This line is important. When the processor extends the 8-bit interrupt number
    // to a 32bit value, it sign-extends, not zero extends. So if the most significant
    // bit (0x80) is set, regs.int_no will be very large (about 0xffffff80).
    uint8_t int_no = regs.int_no & 0xFF;
    struct int_handler_t intrpt = int_handlers[int_no];
    if (intrpt.handler != 0)
    {

        intrpt.handler(&regs, intrpt.data);
    }
    else
    {
        //monitor_write("unhandled interrupt: ", 24);
        //monitor_write_hex(int_no);
        //monitor_put('\n');

        //for(;;);
    }
}


