#include "keyboard.h"
#include "terminal.h"
#include "port_io.h"
#include "isr.h"
#include <stdint.h>

// Basic US QWERTY keyboard layout
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

// Translate a scancode to an ASCII character
static inline char scancode_to_ascii(uint8_t scancode) {
    if (scancode >= sizeof(keyboard_layout)) {
        return 0;
    }
    return keyboard_layout[scancode];
}

// Keyboard interrupt handler
void keyboard_handler(struct regs* r) {
    (void)r; // Unused parameter (we don't need register state here)

    uint8_t scancode = inb(0x60);

    // If the highest bit is set, it means the key was released
    if (scancode & 0x80) {
        // Key release - ignore
        return;
    } else {
        char c = scancode_to_ascii(scancode);
        if (c) {
            terminal_putchar(c);
        }
    }

    // Send End of Interrupt (EOI) to the PIC
    outb(0x20, 0x20);
}

void keyboard_install() {
    irq_install_handler(1, keyboard_handler);
}

