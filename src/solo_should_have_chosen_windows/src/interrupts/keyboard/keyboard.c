#include "interrupts/keyboard/keyboard.h"
#include "interrupts/keyboard/keyboard_map.h"

#include "libc/stdint.h"
#include "libc/stdbool.h"

#define KEYBOARD_BUFFER_SIZE 128

static char kb_buffer[KEYBOARD_BUFFER_SIZE];
static uint8_t kb_head = 0;
static uint8_t kb_tail = 0;

static bool shift_pressed = false;
static bool altgr_pressed = false;
static bool is_extended = false;

void keyboard_handle_scancode(uint8_t scancode) {
    if (scancode == 0xE0) {
        is_extended = true;
        return;
    }

    if (scancode & 0x80) {
        uint8_t released = scancode & 0x7F;
        if (released == KEYBOARD_LSHIFT || released == KEYBOARD_RSHIFT) {
            shift_pressed = false;
        } else if (released == KEYBOARD_ALT_GR) {
            altgr_pressed = false;
        }
    } else {
        if (is_extended) {
            is_extended = false;
            
            if (scancode == 0x4B) {  // Left arrow
                kb_buffer[kb_head] = '\x1B';  // ESC
                kb_head = (kb_head + 1) % KEYBOARD_BUFFER_SIZE;
                kb_buffer[kb_head] = 'D';     // 'D' means Left in escape codes
                kb_head = (kb_head + 1) % KEYBOARD_BUFFER_SIZE;
            } else if (scancode == 0x4D) { // Right arrow
                kb_buffer[kb_head] = '\x1B';
                kb_head = (kb_head + 1) % KEYBOARD_BUFFER_SIZE;
                kb_buffer[kb_head] = 'C';     // 'C' means Right in escape codes
                kb_head = (kb_head + 1) % KEYBOARD_BUFFER_SIZE;
            } else if (scancode == 0x48) { // Up arrow
                kb_buffer[kb_head] = '\x1B';
                kb_head = (kb_head + 1) % KEYBOARD_BUFFER_SIZE;
                kb_buffer[kb_head] = 'A';
                kb_head = (kb_head + 1) % KEYBOARD_BUFFER_SIZE;
            } else if (scancode == 0x50) { // Down arrow
                kb_buffer[kb_head] = '\x1B';
                kb_head = (kb_head + 1) % KEYBOARD_BUFFER_SIZE;
                kb_buffer[kb_head] = 'B';
                kb_head = (kb_head + 1) % KEYBOARD_BUFFER_SIZE;
            }
            return;
        }
        if (scancode == KEYBOARD_LSHIFT || scancode == KEYBOARD_RSHIFT) {
            shift_pressed = true;
        } else if (scancode == KEYBOARD_ALT_GR) {
            altgr_pressed = true;
        } else {
            char key = keyboard_normal[scancode];
            if (shift_pressed) {
                key = keyboard_shift[scancode];
            }
            if (altgr_pressed) {
                key = keyboard_altgr[scancode];
            }

            if (key != 0) {
                kb_buffer[kb_head] = key;
                kb_head = (kb_head + 1) % KEYBOARD_BUFFER_SIZE;
                if (kb_head == kb_tail) {
                    // Buffer overflow, discard the oldest character
                    kb_tail = (kb_tail + 1) % KEYBOARD_BUFFER_SIZE;
                }
            }
        }
    }
}

bool keyboard_has_char() {
    return kb_head != kb_tail;
}

char keyboard_get_char() {
    if (kb_head == kb_tail) {
        return 0; // No character available
    }
    char c = kb_buffer[kb_tail];
    kb_tail = (kb_tail + 1) % KEYBOARD_BUFFER_SIZE;
    return c;
}