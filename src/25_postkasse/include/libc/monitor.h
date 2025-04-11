// monitor.h -- Basic screen output functions
#ifndef MONITOR_H
#define MONITOR_H

#include "libc/stdint.h"
void monitor_put(char c);
void monitor_write(const char *string);
void monitor_newline();
void monitor_backspace();

#endif