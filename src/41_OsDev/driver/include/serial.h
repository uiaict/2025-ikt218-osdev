#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>

////////////////////////////////////////
// Serial Port Interface (COM1)
////////////////////////////////////////

// Initialize the serial port (default: COM1)
void init_serial(void);

// Check if the transmit buffer is ready for a new byte
int serial_transmit_empty(void);

// Send a single character over the serial port
void serial_putchar(char c);

// Send a null-terminated string over the serial port
void serial_write(const char* str);

// Formatted output to the serial port (like printf)
void serial_printf(const char* fmt, ...);

#endif // SERIAL_H
