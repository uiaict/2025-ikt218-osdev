// isr.c
#include "terminal.h"
#include "port_io.h"


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
    *p = '\0';
}

extern void keyboard_handler();

void isr_handler(int interrupt) {
    if (interrupt == 32) {
        // IRQ0: Timer — do nothing (or throttle if needed)
        return;
    }

    if (interrupt == 33) {
        // IRQ1: Keyboard
        keyboard_handler();
        return;
    }

    terminal_write("Received interrupt: ");
    char buffer[16];
    itoa(interrupt, buffer);
    terminal_write(buffer);
}
