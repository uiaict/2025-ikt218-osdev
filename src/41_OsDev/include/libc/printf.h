//printf.h

#ifndef KERNEL_PRINTF_H
#define KERNEL_PRINTF_H

int printf(const char* fmt, ...);
void serial_printf(const char* fmt, ...);

#endif