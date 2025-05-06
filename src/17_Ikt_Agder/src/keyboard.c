#include "libc/keyboard.h"
#include "libc/stdio.h"

char scancode_to_ascii[128] = {
    [0x02] = '1', [0x03] = '2', [0x04] = '3', [0x05] = '4',
    [0x06] = '5', [0x07] = '6', [0x08] = '7', [0x09] = '8',
    [0x0A] = '9', [0x0B] = '0', [0x1E] = 'a', [0x30] = 'b',
    [0x2E] = 'c', [0x20] = 'd', [0x12] = 'e', [0x21] = 'f',
    // (add more mappings as needed)
};

char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
int buffer_index = 0;

// IRQ1 handler for keyboard
void irq1_handler() {
    // Read scancode from the keyboard data port (0x60)
    uint8_t scancode = inb(0x60);  // inb() is an I/O port read function

    if (scancode < 128) {
        // Translate scancode to ASCII and store it in the buffer
        keyboard_buffer[buffer_index++] = scancode_to_ascii[scancode];
    }

    // Echo the ASCII character to the screen
    printf("%c", scancode_to_ascii[scancode]);
}

// Keyboard initialization
void keyboard_init() {
    // Register the IRQ1 handler
    set_idt_entry(33, (uint32_t)irq1_handler, 0x08, 0x8E);
}
