// isr.c

#include <driver/include/terminal.h>
#include "port_io.h"
#include <kernel/interrupt/pit.h>

////////////////////////////////////////
// Integer to ASCII Conversion
////////////////////////////////////////

// Convert integer to string (base 10)
void itoa(int value, char* buffer) {
    char* p = buffer;
    if (value == 0) {
        *p++ = '0';
    } else {
        int digits[10], i = 0;
        while (value && i < 10) {
            digits[i++] = value % 10;
            value /= 10;
        }
        while (--i >= 0) {
            *p++ = '0' + digits[i];
        }
    }
    *p++ = '\n';
    *p   = '\0';
}

////////////////////////////////////////
// Interrupt Service Dispatcher
////////////////////////////////////////

extern void keyboard_handler(void);

void isr_handler(int interrupt) {
    if (interrupt == 32) {
        return;
    }

    if (interrupt == 33) {
        keyboard_handler();
        return;
    }

    terminal_write("Received interrupt: ");
    char buffer[16];
    itoa(interrupt, buffer);
    terminal_write(buffer);
}
