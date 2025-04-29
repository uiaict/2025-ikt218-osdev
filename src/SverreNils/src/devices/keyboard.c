#include "devices/keyboard.h"
#include "printf.h"
#include "libc/io.h"
#include "shell.h"
#include "arch/irq.h"

#define MAX_INPUT_LEN 128
static char input_buffer[MAX_INPUT_LEN];
static int input_pos = 0;

char scancode_ascii[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=','\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n', 0,
    'a','s','d','f','g','h','j','k','l',';','\'','`',  0,'\\',
    'z','x','c','v','b','n','m',',','.','/',  0, '*', 0,' ',
    // resten fylles implicit med 0
};

void keyboard_handler(uint8_t scancode) {
    if (scancode >= 128) return;

    char c = scancode_ascii[scancode];
    if (!c) return;

    if (c == '\n') {
        putc('\n');
        input_buffer[input_pos] = '\0';
        printf("Du skrev: %s\n", input_buffer);
        shell_handle_input(input_buffer);
        input_pos = 0;
        shell_prompt();
        return;
    }

    if (c == '\b') {
        if (input_pos > 0) {
            input_pos--;
            putc('\b');
            putc(' ');
            putc('\b');
        }
        return;
    }

    if (input_pos < MAX_INPUT_LEN - 1) {
        input_buffer[input_pos++] = c;
        putc(c);
    }
}

static void keyboard_wrapper() {
    __asm__ volatile("sti");  // ✅ aktiver interrupts eksplisitt
    uint8_t scancode = inb(0x60);
    keyboard_handler(scancode);
}


void init_keyboard() {
    irq_register_handler(1, keyboard_wrapper);
}
void restore_keyboard_handler() {
    irq_register_handler(1, keyboard_wrapper);
}
void reset_input_buffer() {
    input_pos = 0;
    input_buffer[0] = '\0';
    shell_prompt();  // ✅ viser "UiAOS> "
}
