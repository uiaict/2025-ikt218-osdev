#include "kernel/keyboard.h"
#include "libc/stddef.h"

static keyboard_callback_t current_keyboard_subscriber = NULL;

void input_init() {
    current_keyboard_subscriber = NULL;
}

void input_set_keyboard_subscriber(keyboard_callback_t callback) {
    current_keyboard_subscriber = callback;
}

void input_route_keystroke(char ascii_char) {
    if (current_keyboard_subscriber != NULL) {
        current_keyboard_subscriber(ascii_char);
    }
}