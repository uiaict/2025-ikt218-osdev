#include "irq.h"

// External function declaration for our common handler
extern void irq_common_handler(uint32_t irq_num);

// Function to create an appropriate stack frame and call our C handler
// These functions will be called by the processor when an interrupt occurs
void irq0_handler(void) { irq_common_handler(0x20); }
void irq1_handler(void) { irq_common_handler(0x21); }
void irq2_handler(void) { irq_common_handler(0x22); }
void irq3_handler(void) { irq_common_handler(0x23); }
void irq4_handler(void) { irq_common_handler(0x24); }
void irq5_handler(void) { irq_common_handler(0x25); }
void irq6_handler(void) { irq_common_handler(0x26); }
void irq7_handler(void) { irq_common_handler(0x27); }
void irq8_handler(void) { irq_common_handler(0x28); }
void irq9_handler(void) { irq_common_handler(0x29); }
void irq10_handler(void) { irq_common_handler(0x2A); }
void irq11_handler(void) { irq_common_handler(0x2B); }
void irq12_handler(void) { irq_common_handler(0x2C); }
void irq13_handler(void) { irq_common_handler(0x2D); }
void irq14_handler(void) { irq_common_handler(0x2E); }
void irq15_handler(void) { irq_common_handler(0x2F); }

// Table of IRQ handler addresses to be used when setting up the IDT
void* irq_handlers_table[16] = {
    irq0_handler,  irq1_handler,  irq2_handler,  irq3_handler,
    irq4_handler,  irq5_handler,  irq6_handler,  irq7_handler,
    irq8_handler,  irq9_handler,  irq10_handler, irq11_handler,
    irq12_handler, irq13_handler, irq14_handler, irq15_handler
};