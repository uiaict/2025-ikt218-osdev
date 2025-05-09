// libc/keyboard.c
#include "libc/keyboard.h"
#include "libc/io.h"
#include "libc/isr.h"
#include "libc/idt.h"

// simple scancode→ASCII for letters, digits, symbols
static const char lowercase[128] = {
    0, 27, '1','2','3','4','5','6','7','8','9','0','-','=','\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0,'a','s','d','f','g','h','j','k','l',';','\'','`',0,'\\',
    'z','x','c','v','b','n','m',',','.','/',0,'*',0,' ',
    // rest zeros...
};

// global storage of last key
volatile char last_key = 0;

// low-level poll for one scancode
static int get_scancode(void) {
    int sc;
    while (1) {
        if (inb(KEYBOARD_STATUS_PORT) & 1) {
            sc = inb(KEYBOARD_DATA_PORT);
            break;
        }
    }
    return sc;
}

// IRQ1 callback
static void keyboardHandler(registers_t regs) {
    int sc = get_scancode();
    // ignore key-release
    if (sc & 0x80) return;

    char key = 0;
    switch (sc) {
      case SCAN_CODE_KEY_UP:    key = 'w'; break;
      case SCAN_CODE_KEY_DOWN:  key = 's'; break;
      case SCAN_CODE_KEY_LEFT:  key = 'a'; break;
      case SCAN_CODE_KEY_RIGHT: key = 'd'; break;
      default:
        if (sc < 128) key = lowercase[sc];
    }
    if (key) last_key = key;
}

void initKeyboard(void) {
    register_interrupt_handler(IRQ1, keyboardHandler);
}

char get_last_key(void) {
    char k = last_key;
    last_key = 0;
    return k;
}
