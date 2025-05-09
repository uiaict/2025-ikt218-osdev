#include "kernel/interrupt_functions.h"
#include "common/monitor.h"
#include "common/input.h"
#include "common/io.h"
#include "libc/stdbool.h"
#include "libc/stdint.h"
#include "libc/stddef.h"

extern volatile int last_key;

// Forward declarations
void irq_keyboard_menu_handler(registers_t* regs, void* context);
void irq_keyboard_text_handler(registers_t* regs, void* context);
void irq_keyboard_game_handler(registers_t* regs, void* context);

// Function pointer for current handler
static void (*active_keyboard_handler)(registers_t*, void*) = irq_keyboard_menu_handler;

// Default IRQ1 dispatcher
void irq_keyboard_handler(registers_t* regs, void* context) {
    if (active_keyboard_handler != NULL) {
        active_keyboard_handler(regs, context);
    }
}

// Switch between handlers
void set_keyboard_handler_mode(int mode) {
    switch (mode) {
        case 0:
            active_keyboard_handler = irq_keyboard_menu_handler;
            break;
        case 1:
            active_keyboard_handler = irq_keyboard_text_handler;
            break;
        case 2:
            active_keyboard_handler = irq_keyboard_game_handler;
            break;
    }
}

// IRQ1 keyboard handler for menu
void irq_keyboard_menu_handler(registers_t* regs, void* context) {
    uint8_t sc = inb(0x60);
    switch (sc) {
        case 0x48: last_key = 1; break; // UP
        case 0x50: last_key = 2; break; // DOWN
        case 0x1C: last_key = 6; break; // ENTER (new game or select)
        case 0x01: last_key = 9; break; // ESC (exit)
    }
}

// IRQ1 keyboard handler for text mode
void irq_keyboard_text_handler(registers_t* regs, void* context) {
    static unsigned char last_scancode = 0;
    static bool key_is_held = false;

    unsigned char scancode = inb(0x60);

    // Key release
    if (scancode & 0x80) {
        last_scancode = scancode & 0x7F;
        key_is_held = false;
        return;
    }

    if (scancode == last_scancode && key_is_held) {
        return; 
    }

    last_scancode = scancode;
    key_is_held = true;

    char c = scancode_to_ascii(&scancode);

    switch (c) {
        case 0:
            return; 
        case 1:
            monitor_scroll_up();
            return;
        case 2:
            monitor_scroll_down();
            return;
        case 3:
            printf("[RIGHT]\n");
            return;
        case 4:
            printf("[RIGHT]\n");
            return;
        case 5:
            monitor_backspace();
            return;
        case 6:
            monitor_enter();
            return;
        case 7:
            monitor_put_char(' ');
            return;
        default:
            monitor_put_char(c);
            return;
    }
}

void irq_keyboard_game_handler(registers_t* regs, void* context) {
    uint8_t sc = inb(0x60);
    switch (sc) {
        case 0x48: last_key = 1; break; // UP
        case 0x50: last_key = 2; break; // DOWN
        case 0x4B: last_key = 3; break; // LEFT
        case 0x4D: last_key = 4; break; // RIGHT
        case 0x19: last_key = 5; break; // P  (pause)
        case 0x1C: last_key = 6; break; // ENTER (new game or select)
        case 0x01: last_key = 9; break; // ESC (exit)
    }
}

// Registers your interrupt handlers
void init_interrupt_functions() {
    register_irq_handler(1, irq_keyboard_handler, NULL);       // IRQ 1 for keyboard
}
