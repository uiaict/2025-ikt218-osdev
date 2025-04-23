#pragma once

#include "interrupts.h"

void keyboard_handler(registers_t* regs, void* context);
char scancode_to_ascii(unsigned char* scan_code);