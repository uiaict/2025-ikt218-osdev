#include "libc/keyboard.h"
#include "libc/stdint.h"

// Simplified US QWERTY scancode to ASCII table
static const char scancode_table[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n', 0,
    'a','s','d','f','g','h','j','k','l',';','\'','`', 0, '\\',
    'z','x','c','v','b','n','m',',','.','/', 0, '*', 0, ' ',
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static volatile char last_key = 0;

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void keyboard_handler(void) {
    uint8_t scancode = inb(0x60);
    if (scancode & 0x80) return;

    char key = scancode_table[scancode];

    if (key == '\b') {
        terminal_backspace();
    } 

    else {
        last_key = key;
    }
}

char get_last_key(void) {
    char key = last_key;
    last_key = 0;
    return key;
}
