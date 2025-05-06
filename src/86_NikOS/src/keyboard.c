#include "ports.h"
#include "terminal.h"
#include <stdint.h>

static const char scancode_to_ascii[128] = {
    0, 27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
   '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0,  'a','s','d','f','g','h','j','k','l',';','\'','`',
    0,  '\\','z','x','c','v','b','n','m',',','.','/',
    0,  '*', 0,  ' '
};

void keyboard_handler() {
    uint8_t scancode = inb(0x60);
    if (scancode & 0x80) return; // Ignore key releases
    char c = scancode_to_ascii[scancode];
    if (c) terminal_putchar(c);
}

void keyboard_install() {
    outb(0x21, inb(0x21) & ~0x02);
}