#include "i386/keyboard.h"
#include "i386/interruptRegister.h"
#include "i386/monitor.h"
#include "kernel/memory.h"
#include "libc/system.h"
#include "common.h"

int cannotType = 1;
int menu_buffer = 0;
int escpressed = 0;

static char key_buffer[BUFFER_SIZE];

void EnableBufferTyping()
{
    menu_buffer = 1;
}
void DisableBufferTyping()
{
    menu_buffer = 0;
}

// Enable free typing from user
void EnableTyping()
{
    cannotType = 0;
}
// Disable free typing from user
void DisableTyping()
{
    cannotType = 1;
}

int has_user_pressed_esc()
{
    if (!escpressed)
    {
        return 0;
    }
    escpressed = 0;
    return 1;
}

char get_first_buffer()
{
    return key_buffer[0];
}

void wait_for_keypress()
{
    reset_key_buffer();
    while (key_buffer[0] == '\0')
    {
        // venter på at bruker trykker på en knapp
    }
}

char get_key()
{
    reset_key_buffer();
    while (key_buffer[0] == '\0')
    {
    }
    char key = key_buffer[0];

    return key;
}

void write_to_buffer(char c)
{
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        if (key_buffer[i] == '\0')
        {
            key_buffer[i] = c;
            break;
        }
    }
}

void reset_key_buffer()
{
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        key_buffer[i] = '\0';
        if (key_buffer[i + 1] == '\0')
            break;
    }
}
void irq1_keyboard_handler(registers_t *regs, void *ctx)
{
    uint8_t scancode = inb(0x60);
    char ascii = scanCodeToASCII(&scancode);

    if (menu_buffer)
    {
        write_to_buffer(ascii);
    }
    else if (cannotType)
        return;
    else if (ascii == 1)
    {
        move_cursor_direction(arrowKeys2D.x, arrowKeys2D.y);
    }
    else if (ascii != 0)
    {
        putchar(ascii); // Eller bruk printf
    }

    (void)regs;
    (void)ctx;
}

bool shiftPressed = false;
bool capsEnabled = false;

// 1. Scancode-tabell
char small_scancode_ascii[128] =
    {
        '.',
        '.',
        '1',
        '2',
        '3',
        '4',
        '5',
        '6',
        '7',
        '8',
        '9',
        '0',
        '+',
        '\\',
        '.',
        '.',
        'q',
        'w',
        'e',
        'r',
        't',
        'y',
        'u',
        'i',
        'o',
        'p',
        '.',
        '.',
        '.',
        '.',
        'a',
        's',
        'd',
        'f',
        'g',
        'h',
        'j',
        'k',
        'l',
        '.',
        '.',
        '.',
        '.',
        '\'',
        'z',
        'x',
        'c',
        'v',
        'b',
        'n',
        'm',
        ',',
        '.',
        '-',

};

char large_scancode_ascii[128] =
    {
        '.',
        '.',
        '!',
        '\"',
        '#',
        '$',
        '%',
        '&',
        '/',
        '(',
        ')',
        '=',
        '\?',
        '`',
        '.',
        '.',
        'Q',
        'W',
        'E',
        'R',
        'T',
        'Y',
        'U',
        'I',
        'O',
        'P',
        '.',
        '.',
        '.',
        '.',
        'A',
        'S',
        'D',
        'F',
        'G',
        'H',
        'J',
        'K',
        'L',
        '.',
        '.',
        '.',
        '.',
        '*',
        'Z',
        'X',
        'C',
        'V',
        'B',
        'N',
        'M',
        ';',
        ':',
        '_',

};

// Handles scan codes and returns a char
char scanCodeToASCII(unsigned char *scanCode)
{
    unsigned char word = *scanCode;
    switch (word)
    {
    case 0x3A: // CapsLock pressed
        capsEnabled = !capsEnabled;
        return 0;

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

    case 0x01: // esc pressed
        escpressed = 1;
        return 0;

    case 0x48: // up arrow pressed
        if (arrowKeys2D.y == -1)
            return 1;
        arrowKeys2D.y -= 1;
        return 1;
    case 0xC8: // up arrow released
        arrowKeys2D.y += 1;
        return 0;

    case 0x50: // down arrow pressed
        if (arrowKeys2D.y == 1) return 1;
        arrowKeys2D.y += 1;
        return 1;
    case 0xD0: // down arrow released

        arrowKeys2D.y -= 1;
        return 0;

    case 0x4D: // right arrow pressed
        if (arrowKeys2D.x == 1) return 1;
        arrowKeys2D.x += 1;
        return 1;
    case 0xCD: // right arrow released
        arrowKeys2D.x -= 1;
        return 0;

    case 0x4B: // left arrow pressed
        if (arrowKeys2D.x == -1) return 1;
        arrowKeys2D.x -= 1;
        return 1;
    case 0xCB: // left arrow released
        arrowKeys2D.x += 1;
        return 0;

    case 0x0E: // backspce pressed
        return '\b';

    case 0x39: // space pressed
        return ' ';

    case 0x1C: // enter pressed
        return '\n';
        
    case 0xBA: // CapsLock released
    case 0x53: // delete pressed
    case 0xD3: // delete released
    case 0x9C: // enter released
    case 0x81: // esc released
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
