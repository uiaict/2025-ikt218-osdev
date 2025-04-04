#include "keyboard.h"
#include "isr.h"
#include <kprint.h>

// Scancode table for US QWERTY keyboard (set 1)
static const char scancode_to_ascii[128] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',  // 0-14
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',  // 15-28
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',          // 29-41
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,            // 42-54
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                      // 55-70
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                          // 71-86
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0                           // 87-102
};

// Shift key versions of the scancode table
static const char shift_scancode_to_ascii[128] = {
    0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',  // 0-14
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',  // 15-28
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',           // 29-41
    0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,             // 42-54
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                      // 55-70
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                          // 71-86
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0                           // 87-102
};

// Key state flags
static int shift_pressed = 0;
static int caps_lock = 0;
static int ctrl_pressed = 0;
static int alt_pressed = 0;

// Keyboard buffer
static char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
static int buffer_pos = 0;

// Check if a scancode represents a key release
static int is_key_release(uint8_t scancode) {
    return (scancode & 0x80);
}

// Get the base scancode (strip the release bit)
static uint8_t get_base_scancode(uint8_t scancode) {
    return (scancode & 0x7F);
}

// Convert a scancode to ASCII
static char scancode_to_char(uint8_t scancode) {
    if (scancode >= 128) {
        return 0; // Non-printable
    }
    
    char c;
    
    // Determine if we should use shift mapping
    if (shift_pressed) {
        c = shift_scancode_to_ascii[scancode];
    } else {
        c = scancode_to_ascii[scancode];
        
        // Apply caps lock for letters
        if (caps_lock && c >= 'a' && c <= 'z') {
            c = c - 'a' + 'A';  // Convert to uppercase
        }
    }
    
    return c;
}

// Keyboard interrupt handler
void keyboard_handler(uint8_t interrupt_num) {
    uint8_t scancode = inb(0x60);
    
    // Check if this is a key release
    if (is_key_release(scancode)) {
        uint8_t base_scancode = get_base_scancode(scancode);
        
        // Update modifier key states
        switch (base_scancode) {
            case SCANCODE_LSHIFT:
            case SCANCODE_RSHIFT:
                shift_pressed = 0;
                break;
                
            case SCANCODE_LCTRL:
                ctrl_pressed = 0;
                break;
                
            case SCANCODE_LALT:
                alt_pressed = 0;
                break;
        }
    } else {
        // Key press
        switch (scancode) {
            case SCANCODE_LSHIFT:
            case SCANCODE_RSHIFT:
                shift_pressed = 1;
                break;
                
            case SCANCODE_LCTRL:
                ctrl_pressed = 1;
                break;
                
            case SCANCODE_LALT:
                alt_pressed = 1;
                break;
                
            case SCANCODE_CAPSLOCK:
                caps_lock = !caps_lock;
                kprint("CapsLock: ");
                kprint(caps_lock ? "ON" : "OFF");
                kprint("\n");
                break;
                
            case SCANCODE_ENTER:
                // Add newline to buffer if there's space
                if (buffer_pos < KEYBOARD_BUFFER_SIZE - 1) {
                    keyboard_buffer[buffer_pos++] = '\n';
                    keyboard_buffer[buffer_pos] = '\0';
                }
                kprint("\n");
                break;
                
            case SCANCODE_BACKSPACE:
                if (buffer_pos > 0) {
                    buffer_pos--;
                    keyboard_buffer[buffer_pos] = '\0';
                    // Remove last character from screen
                    kprint("\b \b");
                }
                break;
                
            default:
                // Get the ASCII character
                char c = scancode_to_char(scancode);
                
                // Only handle printable characters
                if (c != 0) {
                    // Add to buffer if there's space
                    if (buffer_pos < KEYBOARD_BUFFER_SIZE - 1) {
                        keyboard_buffer[buffer_pos++] = c;
                        keyboard_buffer[buffer_pos] = '\0';
                    }
                    
                    // Print the character
                    char str[2] = {c, '\0'};
                    kprint(str);
                } else {
                    // For debugging: print scancode for unknown keys
                    kprint("Key pressed: 0x");
                    kprint_hex(scancode);
                    kprint("\n");
                }
                break;
        }
    }
}

// Initialize keyboard
void keyboard_init() {
    // Clear the keyboard buffer
    buffer_pos = 0;
    keyboard_buffer[0] = '\0';
    
    // Reset key states
    shift_pressed = 0;
    caps_lock = 0;
    ctrl_pressed = 0;
    alt_pressed = 0;
    
    // Register the keyboard handler
    register_interrupt_handler(IRQ1, keyboard_handler);
    
    kprint("Keyboard initialized\n");
}

// Get the current keyboard buffer
const char* get_keyboard_buffer() {
    return keyboard_buffer;
}

// Check if the keyboard buffer is empty
int keyboard_buffer_empty() {
    return (buffer_pos == 0);
}

// Process keyboard input (called from your main loop)
void process_keyboard_input() {
    if (buffer_pos > 0) {
        // Handle keyboard input here
        // For example, you could implement a simple command interpreter
        
        // Check if the buffer contains a newline (command submitted)
        if (keyboard_buffer[buffer_pos - 1] == '\n') {
            kprint("Command received: ");
            kprint(keyboard_buffer);
            
            // Process the command here
            // ...
            
            // Clear the buffer after processing
            buffer_pos = 0;
            keyboard_buffer[0] = '\0';
        }
    }
}