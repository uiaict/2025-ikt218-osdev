#include "irq.h"
#include "keyboard.h"
#include "terminal.h"
#include "common.h"
#include "system.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_BUFFER_SIZE 128

// Keycode to character map (for unshifted characters)
static const char keycode_to_char[128] = {
    0, 27, '1','2','3','4','5','6','7','8','9','0','-','=', 8,
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0,'a','s','d','f','g','h','j','k','l',';','\'','`', 0,'\\','z','x','c','v','b','n','m',',','.','/',
    0,'*',0, ' ',0, // Space
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

// Keycode to shifted character map (for special characters)
static const char keycode_to_shifted_char[128] = {
    0, 27, '!','@','#','$','%','^','&','*','(','1',')','_','+',
    '\t','Q','W','E','R','T','Y','U','I','O','P','{','}','\n',
    0,'A','S','D','F','G','H','J','K','L',':','"','~', 0,'|',
    'Z','X','C','V','B','N','M','<','>','?', 0, 0, 0, ' ',
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

static int shift_pressed = 0;

// Simple circular buffer for keyboard input
static char keyboard_buffer[KEYBOARD_BUFFER_SIZE] = {0};
static uint32_t buffer_head = 0;
static uint32_t buffer_tail = 0;

static void buffer_put(char c) {
    uint32_t next = (buffer_head + 1) % KEYBOARD_BUFFER_SIZE;
    if (next != buffer_tail) { // Avoid buffer overrun
        keyboard_buffer[buffer_head] = c;
        buffer_head = next;
    }
}

uint8_t keyboard_read_scancode() {
    uint8_t status;
    do {
        status = inb(0x64);
    } while ((status & 0x01) == 0); // wait for Output Buffer Full (bit 0)

    return inb(0x60);
}


char keyboard_getchar() {
    while (buffer_head == buffer_tail) {
        // Wait for input to be available
    }
    char c = keyboard_buffer[buffer_tail];
    buffer_tail = (buffer_tail + 1) % KEYBOARD_BUFFER_SIZE;
    return c;
}

char keyboard_getchar_nb() {
    if (buffer_head == buffer_tail) {
        return 0;
    }
    char c = keyboard_buffer[buffer_tail];
    buffer_tail = (buffer_tail + 1) % KEYBOARD_BUFFER_SIZE;
    return c;
}

void keyboard_callback(struct registers regs) {
    (void)regs;
    uint8_t scancode = inb(0x60); // Just read it directly!

    // Handle key release
    if (scancode & 0x80) {
        scancode &= 0x7F; // Remove high bit
        if (scancode == 42 || scancode == 54) { // Shift release
            shift_pressed = 0;
        }
        outb(0x20, 0x20);
        return;
    }

    // Handle key press
    if (scancode == 42 || scancode == 54) { // Shift press
        shift_pressed = 1;
        outb(0x20, 0x20);
        return;
    }

    // Handle regular key press
    if (scancode < sizeof(keycode_to_char)) {
        char c;
        // If the key is an alphabetic key (a-z), shift it if Shift is pressed
        if (scancode >= 30 && scancode <= 57) { // Check if it's a letter key (A-Z, a-z)
            if (shift_pressed) {
                c = keycode_to_shifted_char[scancode]; // Uppercase letter
            } else {
                c = keycode_to_char[scancode]; // Lowercase letter
            }
        } else {
            // For non-alphabetic keys, use shifted/unshifted characters
            c = shift_pressed ? keycode_to_shifted_char[scancode] : keycode_to_char[scancode];
        }

        if (c) {
            buffer_put(c);
        }
    }

    outb(0x20, 0x20); // End of Interrupt
}

void keyboard_install() {
    extern void set_idt_entry(int i, uint32_t base, uint16_t selector, uint8_t flags);
    extern void* irq_stub_table[];
    set_idt_entry(33, (uint32_t)irq_stub_table[1], 0x08, 0x8E);
    irq_install_handler(1, keyboard_callback);
}
