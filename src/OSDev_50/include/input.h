#pragma once

#include "interrupts.h"

void keyboard_handler(registers_t* regs, void* context);
void keyboard_logger(registers_t* regs, void* context);
void print_key_log();
void test_outb();
char scancode_to_ascii(unsigned char* scan_code);