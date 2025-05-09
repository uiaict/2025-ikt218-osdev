#include "terminal.h"
#include "isr.h"

void isr_handler(registers_t* regs) {
    terminal_write("Interrupt ");
    terminal_putint(regs->int_no);
    terminal_write(" triggered!\n");
}
