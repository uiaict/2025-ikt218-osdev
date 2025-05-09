#pragma once

#ifndef INPUT_H
#define INPUT_H

#include "interrupts.h" // <-- This makes sure registers_t is known

void keyboard_handler(registers_t* regs, void* ctx);
char scancode_to_ascii(uint8_t scan_code);

#endif
