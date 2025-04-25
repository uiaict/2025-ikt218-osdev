#ifndef TEST_FUNCS_H
#define TEST_FUNCS_H

#include "libc/stdint.h"

// Global flag to track if the IDT test was invoked
extern volatile int idt_test_triggered;

// Test system initialization and setup
void test_system_initialization(void);

// Test VGA terminal output with different colors
void test_terminal_output(void);

// Test Global Descriptor Table functionality
void test_gdt(void);

// Test Interrupt Descriptor Table functionality
void test_idt(void);

// Test keyboard interrupt handling
void test_keyboard_interrupt(void);

// Interactive keyboard test - allows user to type and see characters on screen
void test_keyboard_interactive(void);

// Test software interrupts ved å manuelt utløse CPU exceptions
void test_software_interrupt(void);

// Test hardware interrupts (IRQs) - spesielt tastatur-input
void test_hardware_interrupts(void);

// Test interrupt status and functionality
void test_interrupt_status(void);

// Test memory management and allocation
void test_memory_management(void);

// Test Programmable Interval Timer functions
void test_programmable_interval_timer(void);

// Test PC speaker music playback
void test_music_player(void);

// Test hard drive functionality
void test_hard_drive(void);

// Run all tests
void run_all_tests(void);

#endif // TEST_FUNCS_H 