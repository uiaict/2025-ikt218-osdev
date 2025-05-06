#pragma once

#include "libc/stdint.h"

// Function pointer type for an interrupt handler
typedef void (*interrupt_handler_t)(void);

// Registers a custom interrupt handler for a specific interrupt number
void register_interrupt_handler(uint8_t n, interrupt_handler_t handler);
