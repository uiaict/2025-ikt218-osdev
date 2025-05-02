#include "i386/keyboard.h"
#include "libc/system.h"
#include "common.h"
#include "i386/interruptRegister.h"
#include "screen.h"

typedef struct{
    int x;
    int y;
}arrowKeys;

int cannotType = 1;

void EnableTyping(){
    cannotType = 0;
}
void DisableTyping(){
    cannotType = 1;
}

static arrowKeys move2D;

void irq1_keyboard_handler(registers_t *regs, void *ctx)
{
    

    uint8_t scancode = inb(0x60);
    char ascii = scanCodeToASCII(&scancode);

    if (cannotType) return;

    if (ascii != 0 && ascii != 2 && ascii != 3)
    {
        char msg[2] = {ascii, '\0'};
        printf(msg); // Eller bruk printf
    }

    (void)regs;
    (void)ctx;
}

bool shiftPressed = false;
bool capsEnabled = false;

// 1. Scancode-tabell
char small_scancode_ascii[128] =
    {
        '.', '.', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0','+', '\\', '.', '.', 
        'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '.', '.', '.', '.', 
        'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', '.', '.', '.', '.', '.', 
        'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '-',

    };

char large_scancode_ascii[128] =
    {
        '.', '.', '!', '\"', '#', '$', '%', '&', '/', '(', ')', '=', '\?', '`', '.', '.',
        'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '.', '.', '.', '.',
        'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', '.', '.', '.', '.', '.',
        'Z', 'X', 'C', 'V', 'B', 'N', 'M', ';', ':', '_', 

};

// 2. Buffer for tastetrykk
// char key_buffer[256];
// int key_index = 0;

// switch Casene
char scanCodeToASCII(unsigned char *scanCode)
{
    unsigned char word = *scanCode;
    switch (word) ///////hvilke flere caser trenger jeg? tab pressed(0x0F)???? og released(0x8F)??????
    {
    case 0x3A: // CapsLock pressed
        capsEnabled = !capsEnabled;
        return 0;

    case 0xBA: // CapsLock released
        // capsEnabled = !capsEnabled;
        return 0;

    case 0x53: // delete pressed
        return 0;

    case 0xD3: // delete released
        return 0;

    case 0x39: // space pressed
        return ' ';

    case 0x1C: // enter pressed
        return '\n';

    case 0x9C: // enter released
        return 2;

    case 0x2A: // Left shift pressed
        shiftPressed = true;
        return 0;

    case 0xAA: // left shift released
        shiftPressed = false;
        return 0;

    case 0x36: /// right shift pressed
        shiftPressed = true;
        return 0;

    case 0xB6: // right shift released
        shiftPressed = false;
        return 0;

    case 0x48: // cursor up
        return 0;

    case 0x50: // cursor down
        return 0;

    case 0x49: // cursor høyre
        return 0;

    case 0x4B: // cursor venstre
        return 0;

    case 0x01: // esc pressed
        return 0;

    case 0x81: // esc released
        return 0;

    case 0x0E: // backspce pressed
        return '\b';

    case 0x8E: // backspce released
        return 0;

    default:

        if (word < 128)
        {
            if (capsEnabled ^ shiftPressed)
            {
                return large_scancode_ascii[word];
            }
            else
            {
                return small_scancode_ascii[word];
            }
        }
        return 0;
    }
}
