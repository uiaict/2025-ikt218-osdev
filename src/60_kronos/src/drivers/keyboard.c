#include "drivers/keyboard.h"
#include "drivers/terminal.h"
#include "libc/stdio.h"
#include "sys/io.h"

// Keyboard buffer to store characters
static char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
static uint32_t buffer_index = 0;
static uint32_t buffer_read_index = 0;

// Keep track of shift and caps lock state
static uint8_t shift_pressed = 0;
static uint8_t caps_lock_on = 0;

// US keyboard layout scancode to ASCII conversion table (unshifted keys)
static const char scancode_to_ascii_lowercase[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',  // 0-14
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',  // 15-28
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',          // 29-41
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,            // 42-54
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                  // 55-71
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0                       // 72-88
};

// US keyboard layout scancode to ASCII conversion table (shifted keys)
static const char scancode_to_ascii_uppercase[] = {
    0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',  // 0-14
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',  // 15-28
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',           // 29-41
    0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,             // 42-54
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                  // 55-71
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0                       // 72-88
};

void keyboard_init() {
    register_interrupt_handler(33, keyboard_handler); // IRQ1 = Interrupt 33
    printf("Keyboard driver initialized\n");
}

void keyboard_handler(registers_t regs) {
    uint8_t scancode = inb(KEYBOARD_DATA_PORT);
    
    // Check if this is a key release (bit 7 is set)
    if (scancode & 0x80) {
        scancode &= 0x7F;  // Clear the bit to get the actual scancode
        
        if (scancode == KEY_LEFT_SHIFT || scancode == KEY_RIGHT_SHIFT) {
            shift_pressed = 0;
        }
        return;
    }
    
    // Handle special keys
    switch (scancode) {
        case KEY_BACKSPACE:
            // Remove last character and move cursor back
            if (buffer_index > 0) {
                buffer_index--;
                printf("\b \b");
            }
            return;
            
        case KEY_ENTER:
            keyboard_buffer[buffer_index] = '\n';
            buffer_index = (buffer_index + 1) % KEYBOARD_BUFFER_SIZE;
            printf("\n");
            return;
            
        case KEY_LEFT_SHIFT:
        case KEY_RIGHT_SHIFT:
            shift_pressed = 1;
            return;
            
        case KEY_CAPS_LOCK:
            caps_lock_on = !caps_lock_on;
            return;
    }
    
    char ascii = 0;
    
    // Determine capitalization based on shift and caps lock state
    if ((shift_pressed && !caps_lock_on) || (!shift_pressed && caps_lock_on)) {
        if (scancode < sizeof(scancode_to_ascii_uppercase)) {
            ascii = scancode_to_ascii_uppercase[scancode];
        }
    } else {
        if (scancode < sizeof(scancode_to_ascii_lowercase)) {
            ascii = scancode_to_ascii_lowercase[scancode];
        }
    }
    
    if (ascii != 0) {
        keyboard_buffer[buffer_index] = ascii;
        buffer_index = (buffer_index + 1) % KEYBOARD_BUFFER_SIZE;
        
        char str[2] = {ascii, '\0'};
        printf("%s", str);
    }
}

char keyboard_get_last_char() {
    if (buffer_read_index == buffer_index) {
        return 0; // No new character
    }
    
    char c = keyboard_buffer[buffer_read_index];
    buffer_read_index = (buffer_read_index + 1) % KEYBOARD_BUFFER_SIZE;
    return c;
}

void keyboard_buffer_clear() {
    buffer_index = 0;
    buffer_read_index = 0;
}

int is_key_pressed(char key) {
    if (buffer_read_index == buffer_index) {
        return 0;
    }

    char last_key = keyboard_buffer[buffer_read_index];
    
    if (key >= 'A' && key <= 'Z') {
        key = key + ('a' - 'A');
    }

    if (last_key >= 'A' && last_key <= 'Z') {
        last_key = last_key + ('a' - 'A');
    }

    if (last_key == key) {
        buffer_read_index = (buffer_read_index + 1) % KEYBOARD_BUFFER_SIZE;
        return 1;
    }

    return 0;
}

char keyboard_peek_char() {
    if (buffer_read_index == buffer_index) {
        return 0;
    }
    
    return keyboard_buffer[buffer_read_index];
}