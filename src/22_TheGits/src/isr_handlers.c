#include "libc/scrn.h"
#include "libc/isr_handlers.h"
#include "pit/pit.h"
#include "libc/io.h"
#include "libc/irq.h"
#include "libc/stdint.h"

static int timer_ticks = 0;


void handle_timer_interrupt() {
    pit_increment_tick();

    if (timer_ticks % 500 == 0) {
    }

    send_eoi(0);
}

bool shift_pressed = false;

void handle_keyboard_interrupt() {
    uint8_t scancode = inb(0x60);

    if (scancode == 0x2A || scancode == 0x36) {
        scrn_set_shift_pressed(true);
        send_eoi(1);
        return;
    }

    if (scancode == 0xAA || scancode == 0xB6) {
        scrn_set_shift_pressed(false);
        send_eoi(1);
        return;
    }

    if (scancode & 0x80) {
        send_eoi(1);
        return;
    }

    if (scancode < 128) {
        bool shift = scrn_get_shift_pressed();
        char c = shift ? scancode_to_ascii_shift[scancode] : scancode_to_ascii[scancode];
        if (c) {
            scrn_store_keypress(c);
        }
    }
    send_eoi(1);
}



void handle_div_zero() {
    printf("Divide by zero error triggered!\n");
}

void test_div_zero(){
    int a = 1;
    int b = 0;
    int c = a / b; // This will trigger a division by zero error
    printf("Result: %d\n", c); 
}

void handle_syscall() {
    printf("System call triggered!\n");
}

void default_int_handler() {
    printf("Unhandled interrupt triggered!\n");
    while (1) {
        __asm__ volatile ("hlt"); // Stop the system
    }
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

