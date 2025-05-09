#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "libc/stdint.h"

/**
 * @brief Structure representing the state of registers saved during an
 * interrupt.
 */
typedef struct registers {
  uint32_t ds; // Data segment selector
  uint32_t edi, esi, ebp, useless_value, ebx, edx, ecx, eax; // Pushed by pusha.
  uint32_t int_no, err_code; // Interrupt number and error code (if applicable)
  uint32_t eip, cs, eflags, esp, ss; // Pushed by the processor automatically.
} registers_t;

/**
 * @brief Default handler for unhandled interrupts.
 */
void default_interrupt_handler();

/**
 * @brief Handler for spurious interrupts.
 *
 * Spurious interrupts can occur due to electrical noise or other hardware
 * issues. This handler acknowledges the interrupt and takes no further action.
 */
void spurious_interrupt_handler();

/**
 * @brief Handler for keyboard interrupts.
 */
void keyboard_handler();

/**
 * @brief Initialize the interrupt descriptor table (IDT) and related settings.
 */
void init_interrupts();

#endif