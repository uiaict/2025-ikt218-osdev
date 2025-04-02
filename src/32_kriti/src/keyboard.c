// keyboard.c
#include "keyboard.h"
#include "idt.h"
#include <libc/stdint.h>
#include "pic.h"
#include <libc/stdbool.h>
#include "kprint.h"

// Scancode to ASCII lookup table (Set 1 scancodes)
const key_mapping_t scancode_to_ascii[128] = {
    [0x00] = {0, 0},       // NULL
    [0x01] = {27, 27},     // ESC
    [0x02] = {'1', '!'},
    [0x03] = {'2', '@'},
    [0x04] = {'3', '#'},
    [0x05] = {'4', '$'},
    [0x06] = {'5', '%'},
    [0x07] = {'6', '^'},
    [0x08] = {'7', '&'},
    [0x09] = {'8', '*'},
    [0x0A] = {'9', '('},
    [0x0B] = {'0', ')'},
    [0x0C] = {'-', '_'},
    [0x0D] = {'=', '+'},
    [0x0E] = {'\b', '\b'}, // Backspace
    [0x0F] = {'\t', '\t'}, // Tab
    [0x10] = {'q', 'Q'},
    [0x11] = {'w', 'W'},
    [0x12] = {'e', 'E'},
    [0x13] = {'r', 'R'},
    [0x14] = {'t', 'T'},
    [0x15] = {'y', 'Y'},
    [0x16] = {'u', 'U'},
    [0x17] = {'i', 'I'},
    [0x18] = {'o', 'O'},
    [0x19] = {'p', 'P'},
    [0x1A] = {'[', '{'},
    [0x1B] = {']', '}'},
    [0x1C] = {'\n', '\n'}, // Enter
    [0x1D] = {0, 0},       // Left Control (no ASCII)
    [0x1E] = {'a', 'A'},
    [0x1F] = {'s', 'S'},
    [0x20] = {'d', 'D'},
    [0x21] = {'f', 'F'},
    [0x22] = {'g', 'G'},
    [0x23] = {'h', 'H'},
    [0x24] = {'j', 'J'},
    [0x25] = {'k', 'K'},
    [0x26] = {'l', 'L'},
    [0x27] = {';', ':'},
    [0x28] = {'\'', '"'},
    [0x29] = {'`', '~'},
    [0x2A] = {0, 0},       // Left Shift (no ASCII)
    [0x2B] = {'\\', '|'},
    [0x2C] = {'z', 'Z'},
    [0x2D] = {'x', 'X'},
    [0x2E] = {'c', 'C'},
    [0x2F] = {'v', 'V'},
    [0x30] = {'b', 'B'},
    [0x31] = {'n', 'N'},
    [0x32] = {'m', 'M'},
    [0x33] = {',', '<'},
    [0x34] = {'.', '>'},
    [0x35] = {'/', '?'},
    [0x36] = {0, 0},       // Right Shift (no ASCII)
    [0x37] = {'*', '*'},   // Numpad *
    [0x38] = {0, 0},       // Left Alt (no ASCII)
    [0x39] = {' ', ' '},   // Space
    [0x3A] = {0, 0},       // Caps Lock (no ASCII)
    [0x3B] = {0, 0},       // F1
    [0x3C] = {0, 0},       // F2
    [0x3D] = {0, 0},       // F3
    [0x3E] = {0, 0},       // F4
    [0x3F] = {0, 0},       // F5
    [0x40] = {0, 0},       // F6
    [0x41] = {0, 0},       // F7
    [0x42] = {0, 0},       // F8
    [0x43] = {0, 0},       // F9
    [0x44] = {0, 0},       // F10
    [0x45] = {0, 0},       // Num Lock
    [0x46] = {0, 0},       // Scroll Lock
    [0x47] = {'7', '7'},   // Numpad 7 / Home
    [0x48] = {'8', '8'},   // Numpad 8 / Up Arrow
    [0x49] = {'9', '9'},   // Numpad 9 / Page Up
    [0x4A] = {'-', '-'},   // Numpad -
    [0x4B] = {'4', '4'},   // Numpad 4 / Left Arrow
    [0x4C] = {'5', '5'},   // Numpad 5
    [0x4D] = {'6', '6'},   // Numpad 6 / Right Arrow
    [0x4E] = {'+', '+'},   // Numpad +
    [0x4F] = {'1', '1'},   // Numpad 1 / End
    [0x50] = {'2', '2'},   // Numpad 2 / Down Arrow
    [0x51] = {'3', '3'},   // Numpad 3 / Page Down
    [0x52] = {'0', '0'},   // Numpad 0 / Insert
    [0x53] = {'.', '.'},   // Numpad . / Delete
    // Additional keys not commonly used
};

// Legacy keyboard map from your original code (keeping for compatibility)
static unsigned char keymap[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0,
    ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', 0, 0, 0,
    '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// Key state tracking
static bool shift_pressed = false;
static bool ctrl_pressed = false;
static bool alt_pressed = false;
static bool caps_lock_on = false;

// Buffer to store keystrokes
#define BUFFER_SIZE 256
static char keystroke_buffer[BUFFER_SIZE];
static int write_position = 0;
static int read_position = 0;
static int buffer_count = 0;

// Check if the keyboard buffer is empty
bool keyboard_buffer_empty(void) {
    return buffer_count == 0;
}

// Check if the keyboard buffer is full
bool keyboard_buffer_full(void) {
    return buffer_count == BUFFER_SIZE;
}

// Read a character from the keyboard buffer
int keyboard_read_char(void) {
    if (keyboard_buffer_empty()) {
        return -1;
    }
    
    char c = keystroke_buffer[read_position];
    read_position = (read_position + 1) % BUFFER_SIZE;
    buffer_count--;
    return c;
}

// Translate scancode to ASCII with shift and caps lock support
char translate_scancode(uint8_t scancode) {
    // Check if it's a break code (key release)
    bool is_break = scancode & 0x80;
    uint8_t code = scancode & 0x7F;
    
    // Handle special keys (modifiers)
    if (code == SC_LSHIFT || code == SC_RSHIFT) {  // Left or Right Shift
        shift_pressed = !is_break;
        return 0;  // No ASCII output
    } else if (code == SC_CTRL) {  // Control
        ctrl_pressed = !is_break;
        return 0;  // No ASCII output
    } else if (code == SC_ALT) {  // Alt
        alt_pressed = !is_break;
        return 0;  // No ASCII output
    } else if (code == SC_CAPS && !is_break) {  // Caps Lock (toggle on press)
        caps_lock_on = !caps_lock_on;
        return 0;  // No ASCII output
    }
    
    // If it's a break code, don't output any character
    if (is_break) {
        return 0;
    }
    
    // Check if scancode is in valid range
    if (code >= 128) {
        return 0;  // Out of range
    }
    
    // Determine if we should use the shifted value
    bool use_shifted = shift_pressed;
    
    // Handle caps lock for letters
    if (caps_lock_on && code >= 0x10 && code <= 0x32) {
        // Check if it's a letter key
        char normal_char = scancode_to_ascii[code].normal;
        if ((normal_char >= 'a' && normal_char <= 'z') ||
            (normal_char >= 'A' && normal_char <= 'Z')) {
            use_shifted = !use_shifted;  // Toggle shift for letters
        }
    }
    
    // Get the ASCII value
    char ascii = use_shifted ? scancode_to_ascii[code].shifted : scancode_to_ascii[code].normal;
    
    // Handle control characters
    if (ctrl_pressed && ascii >= 'a' && ascii <= 'z') {
        ascii = ascii - 'a' + 1;  // Convert to control code (Ctrl+A = 1, etc.)
    }
    
    return ascii;
}

// Keyboard interrupt handler
void keyboard_handler(void) {
    unsigned char scancode = inb(0x60);
    kprint("test keyboard input");
    
    // Use the enhanced translation function instead of the simple keymap
    char c = translate_scancode(scancode);
    
    if (c && !keyboard_buffer_full()) {
        // Store the keystroke in the buffer
        keystroke_buffer[write_position] = c;
        write_position = (write_position + 1) % BUFFER_SIZE;
        buffer_count++;
    }
    
    // Send EOI to PIC
    outb(0x20, 0x20);

    __asm__ volatile ("sti"); // denne måtte vi ha 
    
}

// Initialize the keyboard
void keyboard_init(void) {
    // Reset modifier states
    shift_pressed = false;
    ctrl_pressed = false;
    alt_pressed = false;
    caps_lock_on = false;
    
    // Reset buffer positions
    write_position = 0;
    read_position = 0;
    buffer_count = 0;
    
    // Register keyboard_handler in the IDT for IRQ1 (INT 0x21)
    //irq_install_handler(1, keyboard_handler);
    idt_set_interrupt_gate(0x21, (void*)keyboard_handler);
    
    // Enable keyboard IRQ in PIC
    // Clear bit 1 in PIC mask to enable IRQ1
    outb(0x21, inb(0x21) & ~0x02);

    
}

// Function to retrieve the keystroke buffer
char* get_keystroke_buffer(void) {
    return keystroke_buffer;
}

// Function to clear the keystroke buffer
void clear_keystroke_buffer(void) {
    for (int i = 0; i < BUFFER_SIZE; i++) {
        keystroke_buffer[i] = 0;
    }
    write_position = 0;
    read_position = 0;
    buffer_count = 0;
}



// External function declaration for your existing printing function
extern void kprint(const char* str); // Assuming your kprint takes a string argument

// Function to print a single character
void kprint_char(char c) {
    // Create a small 2-character buffer: the character + null terminator
    char str[2] = {c, '\0'};
    kprint(str);
}

// Update your print_keyboard_buffer function to use kprint
void print_keyboard_buffer(void) {
    while (!keyboard_buffer_empty()) {
        char c = keyboard_read_char();
        kprint_char(c);
    }
}

// Add a function to check and process keyboard input
// Call this from your main loop in kernel.c
void process_keyboard_input(void) {
    if (!keyboard_buffer_empty()) {
        print_keyboard_buffer();
    }
}