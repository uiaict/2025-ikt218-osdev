#include "drivers/keyboard.h"
#include "interrupts/io.h"
#include "libc/stdio.h"
#include "libc/stdbool.h"
#include "libc/stddef.h"

char charBuffer[CHARACTER_BUFFER_SIZE];
int bufferIndex = 0;

// Initializes the keyboard
void initKeyboard() {
    printf("Initializing keyboard\n");
    registerInterruptHandler(IRQ1, &keyboardHandler);
}

// Handles the keyboard interrupt
void keyboardHandler(registers_t regs) {
    uint8_t scanCode = inb(KEYBOARD_DATA_PORT);

    if (scanCode & 0x80) {
        if ((scanCode & 0x7F) == LEFT_SHIFT || (scanCode & 0x7F) == RIGHT_SHIFT) {
            shiftPressed = false;
        }
    } else {
        if (scanCode == LEFT_SHIFT || scanCode == RIGHT_SHIFT) {
            shiftPressed = true;
        } else if (scanCode == CAPS_LOCK) {
            capsLockEnabled = !capsLockEnabled;
        } else {
            char ascii;

            if (capsLockEnabled && shiftPressed) {
                ascii = shiftCapsLockLookup[scanCode];
            } else if (shiftPressed) {
                ascii = shiftLookup[scanCode];
            } else if (capsLockEnabled) {
                ascii = capsLockLookup[scanCode];
            } else {
                ascii = defaultLookup[scanCode];
            }

            if (bufferIndex < CHARACTER_BUFFER_SIZE) {
                charBuffer[bufferIndex++] = ascii;
            }

            freeWrite(ascii);
        }
    }
}

void freeWrite(char ascii) {
    switch (ascii) {
        case '\b':
            putchar('\b');
            videoMemory[cursorPos] = ' ';
            break;
        case 0:
            break;
        default:
            putchar(ascii);
            break;
    }
}
