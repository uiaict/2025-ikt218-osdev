#ifndef ISR_H
#define ISR_H

#include "libc/stdint.h"

// Declare ISRs
void isr0();
void isr1();
void isr2();

// A function to handle the interrupt and print a message
void isr_handler(uint32_t isr_number);

#endif
