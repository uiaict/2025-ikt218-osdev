#ifndef TEST_FUNCS_H
#define TEST_FUNCS_H

#include "libc/stdint.h"

// Global flagg for å spore om IDT testen ble utløst
extern volatile int idt_test_triggered;

/**
 * System tester
 * 
 * Funksjoner for å teste ulike systemkomponenter
 */
void test_terminal_output(void);          // Tester terminal utskrift med farger
void test_gdt(void);                      // Tester Global Descriptor Table
void test_idt(void);                      // Tester Interrupt Descriptor Table
void test_keyboard_interactive(void);     // Tester keyboard via menyen i stedet
void test_software_interrupt(void);       // Tester software interrupts
void test_memory_management(void);        // Tester minnehåndtering
void test_programmable_interval_timer(void); // Tester PIT funksjoner
void test_music_player(void);             // Tester lydavspilling
void test_hard_drive(void);               // Tester harddisk funksjonalitet

// Hovedfunksjon for å kjøre alle definerte tester
void run_all_tests(void);

#endif // TEST_FUNCS_H 