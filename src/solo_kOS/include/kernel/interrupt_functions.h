#ifndef INTERRUPT_FUNCTIONS_H
#define INTERRUPT_FUNCTIONS_H


#include "kernel/interrupts.h"


void irq_keyboard_handler(registers_t* regs, void* context);

void init_interrupt_functions();  // Call this from kernel_main to register all your handlers
void set_keyboard_handler_mode(int mode);

#endif