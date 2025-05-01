#ifndef KEYBOARD_H
#define KEYBOARD_H

// Define constants
#define KEYBOARD_ERROR -1
#define KEYBOARD_OK 0
#define KEY_PRESS 1
#define KEY_RELEASE 2

#include "../idt/idt.h"
#include "libc/stdbool.h"

// Declare functions
void toggle_caps_lock(void);
char scancode_to_ascii(unsigned char scancode);
unsigned char* read_keyboard_data_from_buffer(void);
int check_keyboard_errors(unsigned char* scancode);
int get_keyboard_event_type(unsigned char* scancode);
void log_key_press(char input);
void log_buffer(char terminalBuffer[], int* index);
void handle_key_press(unsigned char* scancode, 
char terminalBuffer[], int* index);
void handle_key_release(unsigned char* scancode);
void keyboard_isr(struct InterruptRegisters* regs);
void initKeyboard(void);



bool keyboard_buffer_empty(void);
char read_from_keyboard_buffer(void);
void clear_keyboard_buffer(void);

#endif // KEYBOARD_H