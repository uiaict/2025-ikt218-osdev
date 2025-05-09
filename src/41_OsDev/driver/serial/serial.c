#include "serial.h"
#include "port_io.h"
#include <stdarg.h>

////////////////////////////////////////
// Serial Port Configuration (COM1)
////////////////////////////////////////

#define COM1 0x3F8

// Initialize COM1 serial port (38400 baud, 8N1)
void init_serial() {
    outb(COM1 + 1, 0x00);      // Disable interrupts
    outb(COM1 + 3, 0x80);      // Enable DLAB
    outb(COM1 + 0, 0x03);      // Set divisor (lo byte)
    outb(COM1 + 1, 0x00);      // Set divisor (hi byte)
    outb(COM1 + 3, 0x03);      // 8 bits, no parity, 1 stop bit
    outb(COM1 + 2, 0xC7);      // Enable and clear FIFO (14-byte threshold)
    outb(COM1 + 4, 0x0B);      // IRQs enabled, RTS/DSR set
}

////////////////////////////////////////
// Serial Output Primitives
////////////////////////////////////////

// Return non-zero if transmit buffer is ready
int serial_transmit_empty() {
    return inb(COM1 + 5) & 0x20;
}

// Send a character over serial (with optional CR for LF)
void serial_putchar(char c) {
    while (serial_transmit_empty() == 0);
    outb(COM1, c);

    if (c == '\n') {
        while (serial_transmit_empty() == 0);
        outb(COM1, '\r');
    }
}

// Send a null-terminated string over serial
void serial_write(const char* str) {
    while (*str) {
        serial_putchar(*str++);
    }
}

////////////////////////////////////////
// Formatted Serial Output
////////////////////////////////////////

// Basic printf-like function (implementation continues)
void serial_printf(const char* fmt, ...) {
    // To be continued...
}
