#include "serial.h"
#include "port_io.h"
#include <stdarg.h>

// COM1 port base address
#define COM1 0x3F8

void init_serial() {
    // Disable interrupts
    outb(COM1 + 1, 0x00);
    
    // Set baud rate to 38400 bps
    outb(COM1 + 3, 0x80);    // Enable DLAB
    outb(COM1 + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    outb(COM1 + 1, 0x00);    // (hi byte)
    
    // 8 bits, no parity, one stop bit
    outb(COM1 + 3, 0x03);
    
    // Enable FIFO, clear them, with 14-byte threshold
    outb(COM1 + 2, 0xC7);
    
    // IRQs enabled, RTS/DSR set
    outb(COM1 + 4, 0x0B);
}

int serial_transmit_empty() {
    // Check if transmit buffer is empty (bit 5 of line status register)
    return inb(COM1 + 5) & 0x20;
}

void serial_putchar(char c) {
    // Wait for the transmit buffer to be empty
    while (serial_transmit_empty() == 0);
    
    // Send the character
    outb(COM1, c);
    
    // If newline, also send carriage return
    if (c == '\n') {
        while (serial_transmit_empty() == 0);
        outb(COM1, '\r');
    }
}

void serial_write(const char* str) {
    while (*str) {
        serial_putchar(*str++);
    }
}

// A very basic printf-like function for serial output
void serial_printf(const char* fmt, ...) {
    char buf[256];  // Buffer for formatted text
    va_list args;
    int i = 0;
    
    va_start(args, fmt);
    
    // Process format string
    while (fmt[0] != '\0' && i < sizeof(buf) - 1) {
        if (fmt[0] == '%') {
            fmt++;
            // Handle format specifiers
            switch (fmt[0]) {
                case 'd': {
                    // Simple integer conversion
                    int val = va_arg(args, int);
                    int digits = 1;
                    int temp = val;
                    if (val < 0) {
                        buf[i++] = '-';
                        val = -val;
                        temp = val;
                    }
                    
                    // Count digits
                    while (temp >= 10) {
                        digits++;
                        temp /= 10;
                    }
                    
                    // Convert digits
                    i += digits;
                    temp = i;
                    while (digits-- > 0) {
                        buf[--temp] = '0' + (val % 10);
                        val /= 10;
                    }
                    break;
                }
                case 'x': {
                    // Hexadecimal conversion
                    unsigned int val = va_arg(args, unsigned int);
                    const char hex[] = "0123456789abcdef";
                    int digits = 1;
                    unsigned int temp = val;
                    
                    // Count digits
                    while (temp >= 16) {
                        digits++;
                        temp /= 16;
                    }
                    
                    // Convert digits
                    i += digits;
                    temp = i;
                    while (digits-- > 0) {
                        buf[--temp] = hex[val & 0xF];
                        val >>= 4;
                    }
                    break;
                }
                case 's': {
                    // String output
                    const char* s = va_arg(args, const char*);
                    while (*s && i < sizeof(buf) - 1) {
                        buf[i++] = *s++;
                    }
                    break;
                }
                case 'c': {
                    // Character output
                    buf[i++] = (char)va_arg(args, int);
                    break;
                }
                case '%': {
                    // Literal %
                    buf[i++] = '%';
                    break;
                }
                default:
                    buf[i++] = '?';  // Unknown format specifier
            }
        } else {
            buf[i++] = fmt[0];  // Regular character
        }
        fmt++;
    }
    
    va_end(args);
    
    // Null-terminate the string
    buf[i] = '\0';
    
    // Output the formatted string
    serial_write(buf);
}