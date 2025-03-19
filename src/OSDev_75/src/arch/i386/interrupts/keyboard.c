#include "keyboard.h"
#include "util.h"     // for inPortB, outPortB
#include "idt.h"      // for irq_install_handler and struct InterruptRegisters
#include "stdio.h"    // now declares void print(const char* text)

const uint32_t CAPS = 0xFFFFFFFF - 29;
bool capsOn = false;
bool capsLock = false;

const uint32_t UNKNOWN = 0xFFFFFFFF;
const uint32_t ESC     = 0xFFFFFFFF - 1;
const uint32_t CTRL    = 0xFFFFFFFF - 2;
const uint32_t LSHFT   = 0xFFFFFFFF - 3;
const uint32_t RSHFT   = 0xFFFFFFFF - 4;
const uint32_t ALT     = 0xFFFFFFFF - 5;

const uint32_t lowercase[128] = {
    UNKNOWN, ESC, '1', '2', '3', '4', '5', '6', '7', '8',
    '9', '0', '-', '=', '\b', '\t', 'q', 'w', 'e', 'r',
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', CTRL,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
    '\'', '`', LSHFT, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',',
    '.', '/', RSHFT, '*', ALT, ' ', CAPS, 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
    'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 0, 0, 0, 0, 0, 0, 0,
};

const uint32_t uppercase[128] = {
    UNKNOWN, ESC, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', '\t', 'Q', 'W', 'E', 'R',
    'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', CTRL, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', LSHFT, '|', 'Z', 'X', 'C',
    'V', 'B', 'N', 'M', '<', '>', '?', RSHFT, '*', ALT, ' ', CAPS, 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
    'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 0, 0, 0, 0, 0, 0, 0,
};

void keyboardHandler(struct InterruptRegisters *regs) {
    // Read the scancode once from port 0x60
    uint8_t scancode = inPortB(0x60);
    
    // The high bit indicates key release; mask it off for the key code
    uint8_t keyCode = scancode & 0x7F;
    bool keyPressed = !(scancode & 0x80);
    
    switch (keyCode) {
        // Ignore keys that are not processed:
        case 1:   // ESC
        case 29:  // CTRL (left)
        case 56:  // ALT (left)
        case 59:  // F1 (example)
        case 60:  // F2
        case 61:  // F3
        case 62:  // F4
        case 63:  // F5
        case 64:  // F6
        case 65:  // F7
        case 66:  // F8
        case 67:  // F9
        case 68:  // F10
        case 87:  // F11
        case 88:  // F12
            break;
        case 42:  // Left Shift
            capsOn = keyPressed;
            break;
        case 58:  // Caps Lock
            if (keyPressed) {  // Toggle only on key press
                capsLock = !capsLock;
            }
            break;
        default:
            if (keyPressed) {  // Process key press events only
                uint32_t ch = (capsOn || capsLock) ? uppercase[keyCode] : lowercase[keyCode];
                if (ch != UNKNOWN) {
                    // Create a small null-terminated string and print it using the single-argument print function.
                    char s[2] = { (char)ch, '\0' };
                    print(s);
                }
            }
            break;
    }
}

void initKeyboard() {
    capsOn = false;
    capsLock = false;
    irq_install_handler(1, &keyboardHandler);
}
