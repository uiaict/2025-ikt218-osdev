#include "keyboard.h"
#include "io.h"
#include "libc/stdio.h"

// Simple scancode to ASCII lookup table (US QWERTY, non-shifted)
static const char scancode_to_ascii[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0, 0, // 0x00-0x0F
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,    // 0x10-0x1D
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',   // 0x1E-0x2C
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0     // 0x2D-0x39
    // Add more as needed
};

static char last_char = 0; // Store the last character pressed

static void delay(uint32_t count)
{
    for (volatile uint32_t i = 1; i < count; i++)
    {
        __asm__ volatile("nop");
    }
}

void init_keyboard_controller(void)
{
    printf("Initializing keyboard controller...\n");

    __asm__ volatile("cli");
    outb(0x64, 0xAD);
    printf("Disabled PS/2 port 1 (0x64=0xAD)\n");
    delay(10000);
    outb(0x64, 0xA7);
    printf("Disabled PS/2 port 2 (0x64=0xA7)\n");
    delay(10000);

    while (inb(0x64) & 0x01)
    {
        printf("Flushing output buffer, reading 0x60...\n");
        inb(0x60);
    }
    printf("Output buffer flushed\n");

    outb(0x64, 0x60);
    printf("Setting configuration byte (0x64=0x60)\n");
    delay(10000);
    outb(0x60, 0x41);
    printf("Enabled IRQ1, disabled translation (0x60=0x41)\n");
    delay(10000);

    outb(0x64, 0xAE);
    printf("Enabled PS/2 port 1 (0x64=0xAE)\n");
    delay(10000);
    outb(0x60, 0xFF);
    printf("Sent reset command (0x60=0xFF)\n");
    delay(10000);

    uint32_t timeout = 1000000;
    uint32_t count = 0;
    while (!(inb(0x64) & 0x01) && count < timeout)
    {
        printf("Waiting for output buffer... (0x64=0x%02x)\n", inb(0x64));
        delay(1000);
        count++;
    }

    if (count >= timeout)
    {
        printf("Timeout waiting for keyboard response\n");
    }
    else
    {
        uint8_t ack = inb(0x60);
        printf("Received ACK: 0x%02x\n", ack);
        if (ack != 0xFA)
        {
            printf("Keyboard reset failed: ACK=0x%02x\n", ack);
            for (int i = 0; i < 1000; i++)
            {
                if (inb(0x64) & 0x01)
                {
                    uint8_t scancode = inb(0x60);
                    printf("Polled scancode: 0x%02x\n", scancode);
                }
            }
        }
        else
        {
            printf("Keyboard reset successful\n");
        }
    }

    __asm__ volatile("sti");
}

void keyboard_handler(registers_t *r)
{
    (void)r;
    uint8_t scancode = inb(0x60);

    if (scancode & 0x80)
    {
        return;
    }

    if (scancode == 0x0E)
    {
        putchar('\b');
        last_char = '\b';
    }
    else
    {
        char c = (scancode < sizeof(scancode_to_ascii)) ? scancode_to_ascii[scancode] : 0;
        if (c)
        {
            putchar(c);
            last_char = c;
        }
    }
}

char keyboard_get_last_char(void)
{
    return last_char;
}

void keyboard_clear_last_char(void)
{
    last_char = 0;
}
