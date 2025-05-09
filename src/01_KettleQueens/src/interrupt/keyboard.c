#include "printing/terminal.h"
#include "interrupt/interrupts.h"
#include "libc/stdint.h"
#include "libc/stdbool.h"
#include "kernel/common.h"

static const char scancode_ascii[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=','\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0,'a','s','d','f','g','h','j','k','l',';','\'','`',
    0,'\\','z','x','c','v','b','n','m',',','.','/',
    0,'*',0,' ',0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

static const char scancode_ascii_shifted[128] = {
    0, 27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t','Q','W','E','R','T','Y','U','I','O','P','{','}','\n',
    0,'A','S','D','F','G','H','J','K','L',':','"','~',
    0,'|','Z','X','C','V','B','N','M','<','>','?',
    0,'*',0,' ',0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

static bool shift_pressed = false;

void keyboard_handler(struct InterruptRegisters* regs) {
    uint8_t scancode = inb(0x60);

    // Check for Shift press/release
    if (scancode == 0x2A || scancode == 0x36) {
        shift_pressed = true;
        return;
    } else if (scancode == 0xAA || scancode == 0xB6) {
        shift_pressed = false;
        return;
    }

    // Ignore key releases
    if (scancode & 0x80) return;

    char c = shift_pressed ? scancode_ascii_shifted[scancode] : scancode_ascii[scancode];
    if (c) {
        if (c == '\b'){
            // Handle backspace
            terminal_backspace();  // Handle backspace
        } else if (c == '\n') {
            // Handle newline
            terminal_write("\n");  // Move to the next line
        } else if (c == '\t') {
            // Handle tab
            terminal_write("    ");  // Insert 4 spaces for tab
        } else {
            char buf[2] = {c, '\0'};
            terminal_write(buf);
        }
    }
}
