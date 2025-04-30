#include "input.h"
#include "libc/stdint.h"
#include "libc/stdbool.h"
#include "io.h"
#include "libc/print.h"

bool caps_enabled = false;
bool shift_pressed = false;

// Scancode to ASCII mapping tables
static const char lowercase_ascii[] = {
    '?', '?', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '?', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    '?', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    '?', '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', '?',
    '?', '?', ' '
};

static const char uppercase_ascii[] = {
    '?', '?', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '?', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    '?', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
    '?', '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', '?',
    '?', '?', ' '
};

char scancode_to_ascii(uint8_t scancode) {
    // Handle key releases
    if (scancode & 0x80) {
        uint8_t key = scancode & 0x7F;
        
        // Handle shift release
        if (key == 0x2A || key == 0x36) {
            shift_pressed = false;
        }
        return 0;
    }

    // Handle special keys
    switch (scancode) {
        case 0x1C:  // Enter
            return '\n';
        case 0x0E:  // Backspace
            return '\b';
        case 0x2A:  // Left shift
        case 0x36:  // Right shift
            shift_pressed = true;
            return 0;
        case 0x3A:  // Caps lock
            caps_enabled = !caps_enabled;
            return 0;
        case 0x39:  // Space
            return ' ';
        default:
            break;
    }

    // Handle regular keys
    if (scancode < sizeof(lowercase_ascii)) {
        bool uppercase = caps_enabled ^ shift_pressed;
        return uppercase ? uppercase_ascii[scancode] : lowercase_ascii[scancode];
    }

    return 0;
}

void init_keyboard() {
    // Enable keyboard interrupts (IRQ1)
    outb(0x21, inb(0x21) & 0xFD);  // Clear bit 1 of PIC1 data register
}