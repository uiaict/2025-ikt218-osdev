#include "libc/stdint.h"
#include "isr.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_COMMAND_PORT 0x64

static const char defaultLookup[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t',       // 0x00 to 0x0F
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0, 'a', 's',      // 0x10 to 0x1F
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v',     // 0x20 to 0x2F
    'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0,                     // 0x30 to 0x3F
    0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-', '4', '5', '6', '+', '1',                   // 0x40 to 0x4F
    '2', '3', '0', '.'                                                                  // 0x50 to 0x53
};

void init_keyboard();
void keyboard_handler(struct registers);