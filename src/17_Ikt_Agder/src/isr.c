#include "libc/isr.h"
#include "libc/stdio.h"

// ISR 0
void isr0() {
    isr_handler(0);
}

// ISR 1
void isr1() {
    isr_handler(1);
}

// ISR 2
void isr2() {
    isr_handler(2);
}

// General ISR handler
void isr_handler(uint32_t isr_number) {
    printf("ISR triggered: %d\n", isr_number);
}
