#ifndef TEST_FUNCS_H
#define TEST_FUNCS_H

#include "libc/stdint.h"

// Global flag to track if the IDT test was invoked
extern volatile int idt_test_triggered;

// Test VGA terminal output with different colors
void test_terminal_output(void);

// Test Global Descriptor Table functionality
void test_gdt(void);

// Test Interrupt Descriptor Table functionality
void test_idt(void);

// Test keyboard interrupt handling
void test_keyboard_interrupt(void);

// Test software interrupts ved å manuelt utløse CPU exceptions
void test_software_interrupt(void);

// Test hardware interrupts (IRQs) - spesielt tastatur-input
void test_hardware_interrupts(void);

// Run all tests
void run_all_tests(void);

#endif // TEST_FUNCS_H 