#include "libc/stdint.h"
#include "libc/stdio.h"
#include "libc/stdbool.h"
#include "libc/terminal.h"
#include "idt.h"
#include "common.h"
#include "input.h"

#define BUFFER_SIZE 256
static char keyboard_buffer[BUFFER_SIZE];
static uint8_t buffer_index = 0;

static const char scancode_ascii[] = {
    '?', '?', '1', '2', '3', '4', '5', '6', '7', '8',            // 0-9
    '9', '0', '-', '=', '\b',                                    // Backspace
    '\t',                                                        // Tab
    'q', 'w', 'e', 'r',                                          // 16–19
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',                // 20–28 (Enter)
    0,                                                           // Ctrl
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', // 30–41
    0,                                                           // LShift
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',      // 43–53
    0,                                                           // RShift
    '*',
    0,   // Alt
    ' ', // Space
    // rest is usually not printable
};

static void keyboard_callback(registers_t *regs, void *ctx)
{
    unsigned char scan_code = inb(0x60);

    // Ignore break codes
    if (scan_code & 0x80)
        return;

    if (scan_code == 0x0E) // Backspace
    {
        if (buffer_index > 0)
        {
            buffer_index--;
            keyboard_buffer[buffer_index] = '\0';

            terminal_put('\b');
            terminal_put(' ');
            terminal_put('\b');
        }
        return;
    }

    char f = scancode_ascii[scan_code];
    if (f)
    {
        if (buffer_index < BUFFER_SIZE - 1)
        {
            keyboard_buffer[buffer_index++] = f;
            keyboard_buffer[buffer_index] = '\0';
            terminal_put(f);
        }
    }
}

void init_keyboard()
{
    register_irq_handler(IRQ1, keyboard_callback, NULL); // IRQ1 = 33
}
