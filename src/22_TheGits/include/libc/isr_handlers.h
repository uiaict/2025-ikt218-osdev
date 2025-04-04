#ifndef ISR_HANDLERS_H
#define ISR_HANDLERS_H

#include "libc/idt.h"
#include "libc/stdbool.h"
void handle_timer_interrupt();
void handle_keyboard_interrupt();
void handle_syscall();
extern char scancode_to_ascii[128];
extern char scancode_to_ascii_shift[128];

#endif // ISR_HANDLERS_H