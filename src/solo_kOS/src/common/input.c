#include "common/input.h"
#include "libc/system.h"

bool capsEnabled = false;

// These arrays map scancodes to characters.
// Depending on caps lock, we use either small or large letters.
const char large_ascii[] = {
    '?', '?', '1', '2', '3', '4', '5', '6',
    '7', '8', '9', '0', '-', '=', '\016', '?',
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I',
    'O', 'P', '[', ']', '\034', '?', 'A', 'S',
    'D', 'F', 'G', 'H', 'J', 'K', 'L', ';',
    '\'', '`', '?', '\\', 'Z', 'X', 'C', 'V',
    'B', 'N', 'M', ',', '.', '/', '?', '?',
    '?', ' '
};

const char small_ascii[] = {
    '?', '?', '1', '2', '3', '4', '5', '6',
    '7', '8', '9', '0', '-', '=', '\016', '?',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
    'o', 'p', '[', ']', '\034', '?', 'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
    '\'', '`', '?', '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/', '?', '?',
    '?', ' '
};

// Converts a keyboard scan code to an ASCII character.
// Some keys return special codes like 0 (ignore) or 2 (enter).
char scancode_to_ascii(unsigned char* scan_code) {
    unsigned char code = *scan_code;

    switch (code) {
        case 1:   // ESC
        case 15:  // TAB
        case 29:  // CTRL
        case 56:  // ALT
        case 72:  // UP
            return 1;
            
        case 75:  // LEFT
            return 3;

        case 77:  // RIGHT
            return 4;

        case 80:  // DOWN
            return 2;

        case 14:  // BACKSPACE
            return 5; // Not handled here

        case 28:  // ENTER
            return 6;

        case 42:  // LSHIFT
        case 54:  // RSHIFT
        case 58:  // CAPSLOCK
        case 170: // SHIFT RELEASE (depends on key repeat)
            capsEnabled = !capsEnabled;
            return 0;

        case 57:  // SPACE
            return 7;

        default:
            if (code < 57) {
                return capsEnabled ? large_ascii[code] : small_ascii[code];
            } else {
                return 0;
            }
    }
}
