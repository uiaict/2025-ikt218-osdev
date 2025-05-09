typedef unsigned char bool;
#define true 1
#define false 0

#include <libc/keyboard.h>
#include <libc/terminal.h>
#include <libc/io.h>
#include <libc/isr.h>

#define KEYBOARD_DATA_PORT 0x60
#define INPUT_BUFFER_SIZE 256

char input_buffer[INPUT_BUFFER_SIZE];
int input_length = 0;
int input_cursor = 0;

bool caps_enabled = false;

const char large_ascii[] = {
    '?', '?', '1', '2', '3', '4', '5', '6',
    '7', '8', '9', '0', '-', '=', '\b', '\t',
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I',
    'O', 'P', '[', ']', '\n', 0, 'A', 'S',
    'D', 'F', 'G', 'H', 'J', 'K', 'L', ';',
    '\'', '`', 0, '\\', 'Z', 'X', 'C', 'V',
    'B', 'N', 'M', ',', '.', '/', 0, '*',
    0, ' ', 0};

const char small_ascii[] = {
    '?', '?', '1', '2', '3', '4', '5', '6',
    '7', '8', '9', '0', '-', '=', '\b', '\t',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
    'o', 'p', '[', ']', '\n', 0, 'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
    '\'', '`', 0, '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/', 0, '*',
    0, ' ', 0};



// Faktisk interrupt handler for tastaturet
void keyboard_handler(registers_t regs)
{

    terminal_write("IRQ1! "); // <- LEGG TIL DENNE
    uint8_t scancode = inb(KEYBOARD_DATA_PORT);

    terminal_putc('h');

    // Ignore key releases (scancode > 128 = key up)
    if (scancode & 0x80)
    {
        return;
    }
    else
    {
        // Venstre pil
        if (scancode == 0x4B)
        {
            if (input_cursor > 0)
            {
                input_cursor--;
                if (cursor_x > 0)
                    cursor_x--;
                move_cursor();
            }
            return;
        }
        // Høyre pil
        else if (scancode == 0x4D)
        {
            if (input_cursor < input_length)
            {
                input_cursor++;
                if (cursor_x < 80)
                    cursor_x++;
                move_cursor();
            }
            return;
        }
        // Backspace
        else if (scancode == 0x0E)
        {
            if (input_cursor > 0)
            {
                input_cursor--;
                input_length--;
                cursor_x--;
                move_cursor();
                terminal_putc(' '); // Vis sletting
                cursor_x--;
                move_cursor();
            }
            return;
        }
        // Enter
        else if (scancode == 0x1C)
        {
            terminal_putc('\n');
            input_length = 0;
            input_cursor = 0;
            return;
        }

        // Vanlig tastetrykk
        char c;
        if (caps_enabled)
            c = large_ascii[scancode];
        else
            c = small_ascii[scancode];

        if (c)
        {
            if (input_length < INPUT_BUFFER_SIZE - 1)
            {
                terminal_putc(c); // <-- Viser bokstaven på skjermen!
                input_buffer[input_cursor++] = c;
                input_length++;
            }
        }
    }

    // Send EOI
    outb(0x20, 0x20);
}

// Funksjon for å registrere tastatur handler
void keyboard_init()
{
    terminal_write("Initializing keyboard...\n");
    register_interrupt_handler(33, keyboard_handler); // IRQ1 = 33
}
