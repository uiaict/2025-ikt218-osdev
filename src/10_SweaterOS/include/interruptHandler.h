#ifndef __INTERRUPT_HANDLER_H
#define __INTERRUPT_HANDLER_H

#include "libc/stdint.h"

// PIC (Programmable Interrupt Controller) ports
#define PIC1_COMMAND    0x20
#define PIC1_DATA       0x21
#define PIC2_COMMAND    0xA0
#define PIC2_DATA       0xA1

// Keyboard controller ports
#define KEYBOARD_DATA      0x60
#define KEYBOARD_STATUS    0x64

// For I/O operations
void outb(uint16_t port, uint8_t value);
void outw(uint16_t port, uint16_t value);
uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);
void io_wait(void);

// Interrupt handlers
void isr_handler(uint32_t esp);
void irq_handler(uint32_t esp);

// System initialization
void interrupt_initialize(void);
void pic_initialize(void);

// Keyboard functions
char keyboard_getchar(void);
int keyboard_data_available(void);
char scancode_to_ascii(uint8_t scancode);

// Utility functions
int interrupts_enabled(void);

#endif /* __INTERRUPT_HANDLER_H */ 