#pragma once

#include "libc/stdint.h"

typedef void (*interrupt_handler_t)(void);

void register_interrupt_handler(uint8_t n, interrupt_handler_t handler);
