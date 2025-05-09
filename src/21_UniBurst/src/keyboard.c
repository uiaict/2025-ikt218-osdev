#include "keyboard.h"
#include "io.h"
#include "libc/stdio.h"
#include "libc/stdbool.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"

uint8_t drawingColor = DEFAULT_BACKGROUND_COLOR;

char charBuffer[0];
int bufferIndex = 0;


static uint8_t lastScanCode = 0;


uint8_t checkKeyInput() {
    return lastScanCode;
}


// Initialize keyuboard
void initKeyboard() {
    printf("Initializing keyboard\n");
    registerInterruptHandler(IRQ1, &keyboardHandler);
}

// interrupts
void keyboardHandler(registers_t regs) {
    uint8_t scanCode = inb(KEYBOARD_DATA_PORT);

    //piano part
    if (!(scanCode & 0x80)) {
        lastScanCode = scanCode;
    } else {
        if ((scanCode & 0x7F) == lastScanCode) {
            lastScanCode = 0;
        }
    }
   
    if (scanCode & 0x80) {
        if ((scanCode & 0x7F) == LEFT_SHIFT || (scanCode & 0x7F) == RIGHT_SHIFT) {
            shiftPressed = false;
        }
    }
    else if (scanCode == ESCAPE) {
        drawingMode = !drawingMode;
        clearScreen();
        if (drawingMode) {
            printf("Color:\n");
            videoMemory[14 + 1] = (videoMemory[14 + 1] & 0x0F) | (drawingColor << 4);
        }
    }
   


    else if (drawingMode) {
        draw(scanCode);
        videoMemory[14 + 1] = (videoMemory[14 + 1] & 0x0F) | (drawingColor << 4);
        videoMemory[0] = 'C';
        videoMemory[2] = 'o';
        videoMemory[4] = 'l';
        videoMemory[6] = 'o';
        videoMemory[8] = 'r';
        videoMemory[10] = ':';
    }
    
    else {
        if (scanCode == LEFT_SHIFT || scanCode == RIGHT_SHIFT)
        {
            shiftPressed = true;
        } 
        else if (scanCode == CAPS_LOCK) 
        {
            capsLockEnabled = !capsLockEnabled;
        } 
        else 
        {
            char ascii;
            if (capsLockEnabled && shiftPressed) 
            {
                ascii = shiftCapsLockLookup[scanCode];
            } 
            else if (shiftPressed) 
            {
                ascii = shiftLookup[scanCode];
            } 
            else if (capsLockEnabled) 
            {
                ascii = capsLockLookup[scanCode];
            } 
            else 
            {
                ascii = defaultLookup[scanCode];
            }

            if (bufferIndex < CHARACTER_BUFFER_SIZE) {
                charBuffer[bufferIndex] = ascii;
                bufferIndex++;
            }

            freeWrite(ascii); 
        }
    }
}


void freeWrite(char ascii) {
    switch (ascii) {
    
        case '\b': // backspace
            putchar('\b');
            videoMemory[cursorPos] = ' '; 
            break;
        case 0: // dont do anything if unknown key
            break;
        default: // other character
            putchar(ascii);
            break;
    }
}





void draw(uint8_t scanCode) {
    
    char ascii = defaultLookup[scanCode];

    // Sets the color to the key typed in. 0-9 are normal and q-t are 11-15
    if (ascii >= '0' && ascii <= '9') {
        drawingColor = ascii - '0';
    } 

    // q-t are treated as 11-15
    switch (ascii) {
        case 'q':
            drawingColor = 11;
            break;
        case 'w':
            drawingColor = 12;
            break;
        case 'e':
            drawingColor = 13;
            break;
        case 'r':
            drawingColor = 14;
            break;
        case 't':
            drawingColor = 15;
            break;
        case '\n': // Newline character
            putchar('\n');
            break;    
        case '\t': // Tab character
            putchar('\t');
            break;
        case '\b': // Backspace character
            putchar('\b');
            break;
        case ' ': // Space character
            putchar(' ');
            break;
        case 'x': // Draw character
            videoMemory[cursorPos] = ' ';
            videoMemory[cursorPos + 1] = (videoMemory[cursorPos + 1] & 0x0F) | (drawingColor << 4);
            cursorPos += 2;
            break;
        default:
            break;
    }

    // Scroll the screen if the cursor is at the end of the screen
    if (cursorPos >= 160 * 25) { 
        scroll(); 
        cursorPos -= 160; 
    }

    setCursorPosition(cursorPos/2);
}