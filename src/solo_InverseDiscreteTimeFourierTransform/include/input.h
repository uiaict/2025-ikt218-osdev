#ifndef INPUT_H
#define INPUT_H

#include "libc/stdint.h"
#include "libc/stdbool.h"
#include "interrupts.h"    // brings in registers_t, isr_t, register_irq_handler

/**
 * Initialize the input subsystem (keyboard).
 * Must be called after PIC and IRQs are set up.
 */
void init_input(void);

#endif
