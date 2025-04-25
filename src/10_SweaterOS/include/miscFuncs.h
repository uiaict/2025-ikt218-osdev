#ifndef _MISCFUNCS_H
#define _MISCFUNCS_H

#include "libc/stdint.h" // Standard integer types for portability
#include "libc/stdbool.h"
#include "multiboot2.h"
#include "display.h"

// Utility functions
void hexToString(uint32_t num, char* str);
void delay(uint32_t ms);
void int_to_string(int num, char* str);

// Boot verification
bool verify_boot_magic(uint32_t magic);

// Memory layout handling
void print_multiboot_memory_layout(struct multiboot_tag* tag);

// System control
void halt(void);
void initialize_system(void);
void disable_interrupts(void);
void enable_interrupts(void);

#endif /* _MISCFUNCS_H */
