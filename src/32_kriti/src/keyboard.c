// keyboard.c
#include "keyboard.h"
#include "isr.h"
#include "kprint.h"


bool keyboard_debug_enabled = true;  // Standard: på


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

// Flag to track extended keys (0xE0 prefix)
static bool is_extended_key = false;

// Scancode buffer for menu navigation
#define SCANCODE_BUFFER_SIZE 32
static uint8_t scancode_buffer[SCANCODE_BUFFER_SIZE];
static int buffer_head = 0;
static int buffer_tail = 0;

// Keyboard buffer for text
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

// Add a scancode to the buffer
static void add_scancode_to_buffer(uint8_t scancode) {
    scancode_buffer[buffer_head] = scancode;
    buffer_head = (buffer_head + 1) % SCANCODE_BUFFER_SIZE;
    
    // If buffer is full, advance tail
    if (buffer_head == buffer_tail) {
        buffer_tail = (buffer_tail + 1) % SCANCODE_BUFFER_SIZE;
    }
    
    // Debug
    /*kprint("Added scancode to buffer: 0x");
    kprint_hex(scancode);
    kprint("\n");*/
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
    // Read the scancode from the keyboard controller
    uint8_t scancode = inb(0x60);
    
    // Debug raw scancode
   /* kprint("IRQ1: Raw scancode: 0x");
    kprint_hex(scancode);
    kprint("\n");*/
    
    // Check if this is the extended key prefix (0xE0)
    if (scancode == 0xE0) {
        //kprint("  Extended key prefix detected\n");
        is_extended_key = true;
        return;
    }
    
    // Check if this is a key release
    if (is_key_release(scancode)) {
        uint8_t base_scancode = get_base_scancode(scancode);
        
       /* kprint("  Key release: 0x");
        kprint_hex(base_scancode);
        kprint("\n");*/
        
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
        
     // Add release scancode to buffer with extended flag if needed
     if (is_extended_key) {
        add_scancode_to_buffer(scancode | 0x80);
        is_extended_key = false;
    } else {
        add_scancode_to_buffer(scancode);
    }
} else {
    // Key press - handle based on whether it's an extended key
    if (is_extended_key) {
       /* kprint("  Extended key press: 0x");
        kprint_hex(scancode);
        kprint("\n");*/
        
        // Map extended keys to our direct constants for arrow keys
        uint8_t mapped_scancode = scancode;
        switch (scancode) {
            case 0x48: mapped_scancode = SCANCODE_UP; break;
            case 0x50: mapped_scancode = SCANCODE_DOWN; break;
            case 0x4B: mapped_scancode = SCANCODE_LEFT; break;
            case 0x4D: mapped_scancode = SCANCODE_RIGHT; break;
            default: mapped_scancode = scancode | 0x80; break; // Use high bit for other extended keys
        }
        
        // Add to scancode buffer with the mapped code
        add_scancode_to_buffer(mapped_scancode);
        
        // Reset the extended key flag
        is_extended_key = false;
    } else {
        /*kprint("  Regular key press: 0x");
        kprint_hex(scancode);
        kprint("\n");*/
        
        // Add to scancode buffer
        add_scancode_to_buffer(scancode);
        
        // Handle regular key presses
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
                /*kprint("CapsLock: ");
                kprint(caps_lock ? "ON" : "OFF");
                kprint("\n");*/
                break;
                
            case SCANCODE_ENTER:
                // Add newline to buffer if there's space
                if (buffer_pos < KEYBOARD_BUFFER_SIZE - 1) {
                    keyboard_buffer[buffer_pos++] = '\n';
                    keyboard_buffer[buffer_pos] = '\0';
                }
                //kprint("\n");
                break;
                
            case SCANCODE_BACKSPACE:
                if (buffer_pos > 0) {
                    buffer_pos--;
                    keyboard_buffer[buffer_pos] = '\0';
                    // Remove last character from screen
                   // kprint("\b \b");
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
                }
                break;
        }
    }
}
}

// Initialize keyboard
void keyboard_init(void) {
    kprint("Initializing keyboard...\n");
    
    // Clear the keyboard buffer
    buffer_pos = 0;
    keyboard_buffer[0] = '\0';
    
    // Clear the scancode buffer
    buffer_head = 0;
    buffer_tail = 0;
    
    // Reset key states
    shift_pressed = 0;
    caps_lock = 0;
    ctrl_pressed = 0;
    alt_pressed = 0;
    is_extended_key = false;
    
    // Register the keyboard handler for IRQ1
    register_interrupt_handler(IRQ1, keyboard_handler);
    
    kprint("Keyboard initialized\n");
}

// Get the current keyboard buffer
const char* get_keyboard_buffer(void) {
    return keyboard_buffer;
}

// Check if the keyboard buffer is empty
int keyboard_buffer_empty(void) {
    return (buffer_pos == 0);
}

// Check if keyboard data is available in the scancode buffer
bool keyboard_data_available(void) {
    return buffer_head != buffer_tail;
}

// Get the next scancode from the buffer
uint8_t keyboard_get_scancode(void) {
    if (buffer_head == buffer_tail) {
        kprint("WARNING: Attempted to get scancode from empty buffer\n");
        return 0; // Buffer is empty
    }
    
    uint8_t scancode = scancode_buffer[buffer_tail];
    buffer_tail = (buffer_tail + 1) % SCANCODE_BUFFER_SIZE;
    
    /*kprint("Get scancode from buffer: 0x");
    kprint_hex(scancode);
    kprint("\n");*/
    
    return scancode;
}

// Wait for a key press and return the scancode
uint8_t keyboard_wait_for_key(void) {
    //kprint("keyboard_wait_for_key: Waiting for keypress...\n");
    
    // Wait until a key is available
    while (!keyboard_data_available()) {
        __asm__ volatile ("hlt");
    }
    
    // Get the scancode from the buffer
    uint8_t scancode = keyboard_get_scancode();
    
    /*kprint("keyboard_wait_for_key: Got scancode 0x");
    kprint_hex(scancode);
    kprint("\n");*/
    
    return scancode;
}

// Process keyboard input (called from your main loop)
void process_keyboard_input(void) {
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

// Check if a specific key is pressed (by scancode)
bool key_is_pressed(uint8_t scancode) {
    // For modifier keys like shift, ctrl, alt
    switch (scancode) {
        case SCANCODE_LSHIFT:
        case SCANCODE_RSHIFT:
            return shift_pressed;
            
        case SCANCODE_LCTRL:
            return ctrl_pressed;
            
        case SCANCODE_LALT:
            return alt_pressed;
            
        default:
            // For other keys, we'd need to maintain a key state array
            // This is a simplified version
            return false;
    }
}