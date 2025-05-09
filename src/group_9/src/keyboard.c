#include "keyboard.h"
#include "terminal.h"
#include "port_io.h"
#include "isr.h"
#include <stdint.h>

// US QWERTY keyboard layout
static const char keyboard_layout[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8',
    '9', '0', '-', '=', '\b',   // Backspace
    '\t',                      // Tab
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',  // Enter
    0,                         // Control
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0,                         // Left shift
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',
    0,                         // Right shift
    '*',
    0,                         // Alt
    ' ',                       // Spacebar
    0,                         // Caps lock
    // Remaining entries are zeros...
};

static char last_char = 0;  // Global last key pressed

// Translate a scancode to ASCII
static inline char scancode_to_ascii(uint8_t scancode) {
    if (scancode >= sizeof(keyboard_layout)) return 0;
    return keyboard_layout[scancode];
}

// IRQ1 handler
void keyboard_handler(struct regs* r) {
    (void)r;
    uint8_t scancode = inb(0x60);

    if (scancode & 0x80) {
        // Key release - ignore
        return;
    } else {
        char c = scancode_to_ascii(scancode);
        if (c) {
            last_char = c;            // Store for reading later
            terminal_putchar(c);      // Echo to screen
        }
    }

    outb(0x20, 0x20);  // Send EOI
}

// Reads last character and resets it
char keyboard_read_char() {
    char c = 0;
    while ((c = last_char) == 0) {
        __asm__ __volatile__("hlt");
    }
    last_char = 0;
    return c;
}

// Install keyboard IRQ
void keyboard_install() {
    irq_install_handler(1, keyboard_handler);
}
