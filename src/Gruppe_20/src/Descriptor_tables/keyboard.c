#include "libc/print.h"
#include "libc/stdint.h"
#include "libc/isr.h"
#include "io.h"

static char scancode_to_ascii[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=','\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n', 0,
    'a','s','d','f','g','h','j','k','l',';','\'','`',  0,'\\',
    'z','x','c','v','b','n','m',',','.','/',  0,'*',  0,' ', 0,
    // Remaining keys (function, control etc.) set to 0
};

void keyboard_callback(registers_t regs) {
    uint8_t scancode = inb(0x60);

    if (scancode < 128) {
        char c = scancode_to_ascii[scancode];
        if (c)
            print_char(c);
    }
}
