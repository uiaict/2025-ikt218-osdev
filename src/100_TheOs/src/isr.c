#include "interrupts.h"
#include "libc/stdint.h"
#include "libc/stddef.h"


void load_interrupt_controller(uint8_t n, isr_t controller, void* context)
{
    int_controllers[n].controller = controller;
    int_controllers[n].data = context;
}


void isr_controller(registers_t regs)
{
    uint8_t int_no = regs.int_no & 0xFF;
    struct int_controller_t intrpt = int_controllers[int_no];
    if (intrpt.controller != 0)
    {
        intrpt.controller(&regs, intrpt.data);
    }
    else
    {
        for(;;);
    }
}


