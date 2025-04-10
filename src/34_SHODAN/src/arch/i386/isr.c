#include "isr.h"
#include "terminal.h"

void isr_handler(int int_no, int err_code) {
    terminal_write("ISR triggered: ");
    terminal_putchar('0' + int_no); // basic numeric output
    terminal_write("\n");
}
