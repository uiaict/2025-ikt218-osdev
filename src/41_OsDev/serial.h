#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>

// Initialize serial port (COM1)
void init_serial(void);

// Check if serial transmitter is empty
int serial_transmit_empty(void);

// Write a single character to the serial port
void serial_putchar(char c);

// Write a string to the serial port
void serial_write(const char* str);

// Printf-like function for serial output
void serial_printf(const char* fmt, ...);

#endif // SERIAL_H