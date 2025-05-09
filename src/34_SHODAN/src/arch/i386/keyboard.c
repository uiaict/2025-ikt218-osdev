#include "keyboard.h"
#include "terminal.h"
#include "io.h"
#include "irq.h"  // Needed for irq_register_handler

static const char scancode_table[128] = {
    0, 27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0, // Control
    'a','s','d','f','g','h','j','k','l',';','\'','`',
    0, // Left Shift
    '\\','z','x','c','v','b','n','m',',','.','/', 0,
    '*', 0, ' ', // Alt, space
    // rest 0
};

// New: Helper function to print numbers as text
void terminal_print_int(uint8_t num) {
    char buffer[4]; // max value 255 + null
    int i = 0;

    if (num == 0) {
        terminal_putchar('0');
        return;
    }

    while (num > 0) {
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    }

    // Print digits in reverse
    while (i--) {
        terminal_putchar(buffer[i]);
    }
}

void keyboard_handler() {
    uint8_t scancode = inb(0x60);

    if (scancode & 0x80) {  // If the high bit is set, it's a key release
        terminal_write("Key release detected: ");
        terminal_print_int(scancode & 0x7F); // Mask off the high bit to get the press scancode
        terminal_write("\n");
    } else {
        terminal_write("Scancode: ");
        terminal_print_int(scancode);  // Normal key press
        terminal_write("\n");

        if (scancode < 128) {
            char c = scancode_table[scancode];
            if (c >= 32 && c <= 126) {  // Printable ASCII only
                terminal_write("Character: ");
                terminal_putchar(c);
                terminal_write("\n");
            }
        }
    }
}

// Register the keyboard handler for IRQ1
void keyboard_install() {
    irq_register_handler(1, keyboard_handler);
}
