#include "keyboard.h"
#include "io.h"
#include "libc/stdio.h"

// Simple scancode to ASCII lookup table (US QWERTY, non-shifted)
static const char scancode_to_ascii[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0, 0, // 0x00-0x0F
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0, // 0x10-0x1D
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', // 0x1E-0x2C
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0 // 0x2D-0x39
    // Add more as needed
};

static char last_char = 0; // Store the last character pressed

void keyboard_handler(void) {
    // Read scancode from keyboard port (0x60)
    uint8_t scancode = inb(0x60);

    // Ignore key release (bit 7 set)
    if (scancode & 0x80) {
        outb(0x20, 0x20); // EOI to master PIC
        return;
    }

    // Convert scancode to ASCII
    char c = (scancode < sizeof(scancode_to_ascii)) ? scancode_to_ascii[scancode] : 0;
    if (c) {
        putchar(c); // Print character
        last_char = c; // Store the last character
    }

    // Send EOI
    outb(0x20, 0x20); // Master PIC
}

char keyboard_get_last_char(void) {
    return last_char;
}

void keyboard_clear_last_char(void) {
    last_char = 0;
}