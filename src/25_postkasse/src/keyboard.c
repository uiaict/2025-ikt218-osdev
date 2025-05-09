#include "libc/io.h"
#include "arch/i386/idt/isr.h"
#include "libc/monitor.h"

//Clipboard to store what you type
#define KEYBOARD_BUFFER_SIZE 256
static char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
static uint8_t buffer_index = 0;
volatile char last_key = 0;

//Table converts key presses (scancodes) into letters
static const char scancode_to_ascii[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' ', 0,
};

//When pressing a key function is called
void keyboard_callback(registers_t* regs) {
    uint8_t scancode = inb(0x60); // Read the key press

    if (scancode > 57)            // Ignore invalid keys
        return; 

    char c = scancode_to_ascii[scancode];
    if (c){
        last_key = c;
        if (c == '\b') { // Handle backspace
            if (buffer_index > 0) {
                buffer_index--;
                keyboard_buffer[buffer_index] = 0;
                monitor_backspace();
            }
        }
        else if (c == '\n') { // Handle enter
            keyboard_buffer[buffer_index++] = '\n';
            monitor_newline();
        }
        else { // Normal character
            monitor_put(c); // Print the character
            keyboard_buffer[buffer_index++] = c;
        }
    }
}

//Sets ip the keyboaard interrupt handler
//Ensurtes whenever keys are pressed the OS will handle it byu calling the keyboard_callback function
void init_keyboard() {
    // Register the keyboard interrupt handler
    // IRQ1 is the keyboard interrupt
    // The function keyboard_callback will be called when a key is pressed
    register_interrupt_handler(IRQ1, &keyboard_callback);
}

char keyboard_get_key() {
    while (last_key == 0) {
        __asm__ volatile ("hlt"); // Sleep until next interrupt (very efficient)
    }
    char key = last_key;
    last_key = 0; // Reset after reading
    return key;
}
