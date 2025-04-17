#ifndef ISR_HANDLERS_H
#define ISR_HANDLERS_H

// Start the interrupt service routine (ISR) controllers
// This function initializes the different ISR controllers
// Like division by zero, debug, and non-maskable interrupts
void start_isr_controllers();
// Test interrupts function
// This function is used to test the interrupts
void test_interrupts();

#endif