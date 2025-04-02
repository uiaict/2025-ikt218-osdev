#pragma once
#include "libc/stdint.h"

// High-level ISR handler (called from assembly stubs)
void isr_handler(int interrupt_number);
