#include "i386/keyboard.h"
#include "libc/system.h"
#include "common.h"
#include "i386/interruptRegister.h"
#include "screen.h"

bool shiftPressed = false;
bool capsEnabled = false;

char small_scancode_ascii[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=','\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n', 0,
    'a','s','d','f','g','h','j','k','l',';','\'','`', 0, '\\',
    'z','x','c','v','b','n','m',',','.','/', 0, '*', 0, ' ', 0,
};

char large_scancode_ascii[128] = {
    0,  27, '!','@','#','$','%','^','&','*','(',')','_','+','\b',
    '\t','Q','W','E','R','T','Y','U','I','O','P','{','}','\n', 0,
    'A','S','D','F','G','H','J','K','L',':','"','~', 0, '|',
    'Z','X','C','V','B','N','M','<','>','?', 0, '*', 0, ' ', 0,
};

char scanCodeToASCII(uint8_t* scancode) {
    if (*scancode >= 128) return 0;
    bool upper = (shiftPressed && !capsEnabled) || (!shiftPressed && capsEnabled);
    return upper ? large_scancode_ascii[*scancode] : small_scancode_ascii[*scancode];
}

void irq1_keyboard_handler(registers_t *regs, void *ctx)
{
    uint8_t scancode = inb(0x60);

    switch (scancode)
    {
        // Shift pressed
        case 0x2A: // Left Shift
        case 0x36: // Right Shift
            shiftPressed = true;
            break;

        // Shift released
        case 0xAA: // Left Shift
        case 0xB6: // Right Shift
            shiftPressed = false;
            break;

        // Caps Lock toggle
        case 0x3A:
            capsEnabled = !capsEnabled;
            break;

        // Backspace
        case 0x0E:
            printf("\b \b"); // visuell sletting
            break;

        // Enter
        case 0x1C:
            printf("\n");
            break;

        // Tab
        case 0x0F:
            printf("    "); // fire mellomrom
            break;

        // Escape
        case 0x01:
            printf("[ESC]");
            break;

        default:
            if (scancode < 128)
            {
                char ascii = scanCodeToASCII(&scancode);
                if (ascii != 0 && ascii != 2 && ascii != 3)
                {
                    char msg[2] = {ascii, '\0'};
                    printf(msg);
                }
            }
            break;
    }

    (void)regs;
    (void)ctx;
}
