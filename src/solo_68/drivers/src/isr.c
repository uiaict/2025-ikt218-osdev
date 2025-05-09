#include "terminal.h"

void isr_handler(int interrupt_number) {
    terminal_write("[ISR] Exception occurred: ");

    char buf[4];
    buf[0] = '0' + (interrupt_number / 10);
    buf[1] = '0' + (interrupt_number % 10);
    buf[2] = '\n';
    buf[3] = '\0';
    terminal_write(buf);

}
