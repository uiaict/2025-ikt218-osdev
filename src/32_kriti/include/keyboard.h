// keyboard.h
#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <libc/stdint.h>
#include <libc/stdbool.h>

// Structure for scancode mapping
typedef struct {
    uint8_t normal;   // ASCII when key is pressed normally
    uint8_t shifted;  // ASCII when key is pressed with shift
} key_mapping_t;

// Function prototypes
void keyboard_init(void);
void keyboard_handler(void);  // Changed from uint8_t parameter to match your implementation
char translate_scancode(uint8_t scancode);
int keyboard_read_char(void);
bool keyboard_buffer_empty(void);
bool keyboard_buffer_full(void);
char* get_keystroke_buffer(void);
void clear_keystroke_buffer(void);
void process_keyboard_input(void);

// Define special scancodes
#define SC_ESC    0x01
#define SC_ENTER  0x1C
#define SC_CTRL   0x1D
#define SC_LSHIFT 0x2A
#define SC_RSHIFT 0x36
#define SC_ALT    0x38
#define SC_SPACE  0x39
#define SC_CAPS   0x3A
#define SC_F1     0x3B
#define SC_F12    0x58
#define SC_BKSP   0x0E
#define SC_TAB    0x0F

// Break code bit (for key release)
#define SC_BREAK_BIT 0x80

#endif /* KEYBOARD_H */