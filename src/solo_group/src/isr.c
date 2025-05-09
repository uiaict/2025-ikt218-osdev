#include "interupts.h"
#include "libc/stdint.h"
#include "libc/stddef.h"


void registerInterruptHandler(uint8_t n, isr_t handler, void* context)
{
    intHandlers[n].handler = handler;
    intHandlers[n].data = context;
}

// This gets called from our ASM interrupt handler stub.
void isrHandler(registers_t regs)
{
    // This line is important. When the processor extends the 8-bit interrupt number
    // to a 32bit value, it sign-extends, not zero extends. So if the most significant
    // bit (0x80) is set, regs.int_no will be very large (about 0xffffff80).
    uint8_t int_no = regs.int_no & 0xFF;
    struct interuptHandlerT intrpt = intHandlers[int_no];
    if (intrpt.handler != 0)
    {

        intrpt.handler(&regs, intrpt.data);
    }
    else
    {
        printf("unhandled interrupt: %x\n", int_no);
        for(;;);
    }
}


