#include "keyboard.h"
#include "IDT.h"
#include "printf.h" // Include printf.h which has the outb/inb declarations

// Externing the keyboard buffer from IDT.c
extern char keyboard_buffer[64];
extern int buffer_position;
extern bool shift_pressed;
extern bool caps_lock_on;

// Helper function to read byte from port if needed in this file
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// Get the keyboard buffer
char* get_keyboard_buffer() {
    return keyboard_buffer;
}

// Clear the keyboard buffer
void clear_keyboard_buffer() {
    for (int i = 0; i < 64; i++) {
        keyboard_buffer[i] = '\0';
    }
    buffer_position = 0;
}

// Check if a specific key is pressed (e.g., shift, ctrl, alt)
bool is_key_pressed(uint8_t scancode) {
    switch (scancode) {
        case KEY_SHIFT_L:
        case KEY_SHIFT_R:
            return shift_pressed;
        case KEY_CAPS_LOCK:
            return caps_lock_on;
        default:
            return false;
    }
}
