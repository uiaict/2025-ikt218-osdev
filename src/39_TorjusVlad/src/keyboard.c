#include "keyboard.h"

#include "libc/stdint.h"
#include "libc/stdbool.h"
#include "libc/portio.h"
#include "libc/stdio.h"

#include "arch/i386/idt.h"

#define KEYBOARD_BUFFER_SIZE 128
#define KEYBOARD_LSHIFT 0x2A
#define KEYBOARD_RSHIFT 0x36
#define KEYBOARD_ALT_GR 0x38
#define KEYBOARD_CTRL 0x1D
#define KEYBOARD_ALT 0x38
#define KEYBOARD_SIZE 128

static char kb_buffer[KEYBOARD_BUFFER_SIZE];
static uint8_t kb_head = 0;
static uint8_t kb_tail = 0;

static bool shift_pressed = false;
static bool altgr_pressed = false;
static bool ctrl_pressed = false;
static bool is_extended = false;

// Normal key mapping
const unsigned char keyboard_normal[KEYBOARD_SIZE] = {
    0,    '\x1B', '1',  '2',  '3',  '4',  '5',  '6',
    '7',  '8',    '9',  '0',  '+',  '\'', '\b', '\t',
    'q',  'w',    'e',  'r',  't',  'y',  'u',  'i',
    'o',  'p',    'a',  'u',  '\n', 0,    'a',  's',
    'd',  'f',    'g',  'h',  'j',  'k',  'l',  'o',
    'a',  '|',    0,    '<',  'z',  'x',  'c',  'v',
    'b',  'n',    'm',  ',',  '.',  '-',  0,    0,
    0,    ' ',    0,    0,    0,    0,    0,    0,
    0,    0,      0,    0,    0,    0,    0,    0,
    0,    0,      0,    0,    0,    0,    0,    0,
    0,    0,      0,    0,    0,    0,    0,    0,
    0,    0,      0,    0,    0,    0,    0,    0,
    0,    0,      0,    0,    0,    0,    0,    0,
    0,    0,      0,    0,    0,    0,    0,    0,
    0,    0,      0,    0,    0,    0,    0,    0,
    0,    0,      0,    0,    0,    0,    0,    0
};

// Shift key mapping
const unsigned char keyboard_shift[KEYBOARD_SIZE] = {
    0,    '\x1B', '!',  '"',  '#',  '$',  '%',  '&',
    '/',  '(',    ')',  '=',  '?',  '`',  '\b', '\t',
    'Q',  'W',    'E',  'R',  'T',  'Y',  'U',  'I',
    'O',  'P',    'A',  'U',  '\r', 0,    'A',  'S',
    'D',  'F',    'G',  'H',  'J',  'K',  'L',  'O',
    'A',  '*',    0,    '>',  'Z',  'X',  'C',  'V',
    'B',  'N',    'M',  ';',  ':',  '_',  0,    0,
    0,    ' ',    0,    0,    0,    0,    0,    0,
    0,    0,      0,    0,    0,    0,    0,    0,
    0,    0,      0,    0,    0,    0,    0,    0,
    0,    0,      0,    0,    0,    0,    0,    0,
    0,    0,      0,    0,    0,    0,    0,    0,
    0,    0,      0,    0,    0,    0,    0,    0,
    0,    0,      0,    0,    0,    0,    0,    0,
    0,    0,      0,    0,    0,    0,    0,    0,
    0,    0,      0,    0,    0,    0,    0,    0
};

// AltGr key mapping
const unsigned char keyboard_altgr[KEYBOARD_SIZE] = {
    0,    0,    0,    '@',  '#',  '$',  0,    0,
    '{',  '[',  ']',  '}',  '\\', 0,    0,    0,
    0,    0,    'E',  0,    0,    0,    0,    0,
    0,    0,    0,    '|',  0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,
    0,    '~',  0,    '>',  0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0
};

void keyboard_handler(struct interrupt_registers *regs) {
    //char scan_code = inb(0x60) & 0x7F; // what key is pressed 
    //char press = inb(0x60) & 0x80; // Pressed down or releaced 
    
    uint8_t raw = inb(0x60);

    if (raw == 0xE0) {
        is_extended = true;
        return;
    }

    bool released = raw & 0x80;
    uint8_t scan_code = raw & 0x7F;

    // Modifier handling
    if (released) {
        if (scan_code == KEYBOARD_LSHIFT || scan_code == KEYBOARD_RSHIFT) {
            shift_pressed = false;
        } else if (scan_code == KEYBOARD_CTRL) {
            ctrl_pressed = false;
        } else if (scan_code == KEYBOARD_ALT && is_extended) {
            altgr_pressed = false;
        }
    } else {
        if (scan_code == KEYBOARD_LSHIFT || scan_code == KEYBOARD_RSHIFT) {
            shift_pressed = true;
            return;
        } else if (scan_code == KEYBOARD_CTRL) {
            ctrl_pressed = true;
            return;
        } else if (scan_code == KEYBOARD_ALT && is_extended) {
            altgr_pressed = true;
            return;
        }

        char c = 0;
        if (altgr_pressed || (ctrl_pressed && is_extended)) {
            c = keyboard_altgr[scan_code];
        } else if (shift_pressed) {
            c = keyboard_shift[scan_code];
        } else {
            c = keyboard_normal[scan_code];
        }

        if (c) {
            printf("%c", c);  // You can buffer this instead
        }
    }

    is_extended = false;  // Reset flag after handling

}

void init_keyboard() {
    irq_install_handler(1, &keyboard_handler);
}


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