#include "libc/scrn.h"
#include "libc/isr_handlers.h"

void handle_timer_interrupt() {
    //printf("Timer interrupt triggered!\n");
    send_eoi(0);
}

bool shift_pressed = false;

void handle_keyboard_interrupt() {
    uint8_t scancode = inb(0x60);

    // Shift-keys pressed
    if (scancode == 0x2A || scancode == 0x36) {
        shift_pressed = true;
        send_eoi(1);
        return;
    }

    // Shift-keys released (break codes)
    if (scancode == 0xAA || scancode == 0xB6) {
        shift_pressed = false;
        send_eoi(1);
        return;
    }

    // Ignorer break codes
    if (scancode & 0x80) {
        send_eoi(1);
        return;
    }

    if (scancode < 128) {
        char ascii = shift_pressed ? scancode_to_ascii_shift[scancode] : scancode_to_ascii[scancode];

        if (ascii != 0) {
            char str[2] = {ascii, '\0'}; // Lag en null-terminert streng
            terminal_write(str, VGA_COLOR(15, 0));
        }
    }

    send_eoi(1);
}


void handle_syscall() {
    printf("System call triggered!\n");
}

char scancode_to_ascii[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0,  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\','z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0,  ' ', 0,     '*', 0,  ' ', 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

char scancode_to_ascii_shift[128] = {
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0,  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0,
    '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,
    '*', 0,  ' ', 0, '*', 0,  ' ', 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
