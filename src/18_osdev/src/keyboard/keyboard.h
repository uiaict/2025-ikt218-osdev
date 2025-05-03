#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../../include/libc/monitor.h"
#include "../../include/libc/common.h"
#include "../gdt/isr.h"
#include "../ui/shell.h"

void init_keyboard();
void keyboard_handler(registers_t regs);
void read_line(char* buffer);
#endif