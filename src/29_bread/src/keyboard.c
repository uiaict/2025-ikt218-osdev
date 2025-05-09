#include <keyboard.h>
#include <libc/common.h>
#include <print.h>
#include <libc/irq.h>
#include <libc/song.h>

// Keyboard buffer
static char keyboard_buffer[256];
static uint8_t buffer_start = 0;
static uint8_t buffer_end = 0;
static uint16_t buffer_count = 0;  // Changed from uint8_t to uint16_t to allow comparison with 256

// Shift key state
static uint8_t shift_pressed = 0;

// US keyboard layout scancode to ASCII translation tables
// Regular keymap (no shift pressed)
static const char scancode_to_ascii_table[128] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',  // 0-14
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',  // 15-28
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',          // 29-41
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,            // 42-54
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                   // 55-71
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0                        // 72-88
};

// Shifted keymap (shift pressed)
static const char scancode_to_ascii_shift_table[128] = {
    0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',  // 0-14
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',  // 15-28
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',           // 29-41
    0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,             // 42-54
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                   // 55-71
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0                        // 72-88
};

// Key release codes for shift keys
#define KEY_LEFT_SHIFT_RELEASE 0xAA
#define KEY_RIGHT_SHIFT_RELEASE 0xB6
#define KEY_LEFT_SHIFT 0x2A
#define KEY_RIGHT_SHIFT 0x36

// Keyboard ports
#define KEYBOARD_DATA_PORT 0x60

// Translate scancode to ASCII
char scancode_to_ascii(uint8_t scancode) {
    // Handle key release (high bit set)
    if (scancode & 0x80) {
        // Handle shift key release
        if (scancode == KEY_LEFT_SHIFT_RELEASE || scancode == KEY_RIGHT_SHIFT_RELEASE) {
            shift_pressed = 0;
        }
        return 0; // Don't process release scancodes further
    }
    
    // Handle special keys
    switch (scancode) {
        case KEY_LEFT_SHIFT:
        case KEY_RIGHT_SHIFT:
            shift_pressed = 1;
            return 0;
        default:
            // Regular key, translate scancode to ASCII
            if (scancode < 128) {
                if (shift_pressed) {
                    return scancode_to_ascii_shift_table[scancode];
                } else {
                    return scancode_to_ascii_table[scancode];
                }
            }
            return 0;
    }
}

#define NOTE_C4 262
#define NOTE_D4 294
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_G4 392
#define NOTE_A4 440
#define NOTE_B4 494
#define NOTE_C5 523

void play_key_note(char key) {
    uint32_t frequency = 0;
    uint32_t duration = 150; // Short duration for immediate feedback
    
    switch (key) {
        case '1': frequency = NOTE_C4; break;
        case '2': frequency = NOTE_D4; break;
        case '3': frequency = NOTE_E4; break;
        case '4': frequency = NOTE_F4; break;
        case '5': frequency = NOTE_G4; break;
        case '6': frequency = NOTE_A4; break;
        case '7': frequency = NOTE_B4; break;
        case '8': frequency = NOTE_C5; break;
        default: return; // Not a piano key
    }
    
    if (frequency > 0) {
        // Use play_sound instead of play_tone
        play_sound(frequency);
        // Sleep briefly to let the sound play
        sleep_interrupt(100); 
        // Then stop the sound
        stop_sound();
    }
}

// Keyboard interrupt handler
void keyboard_handler(registers_t regs) {
    uint8_t scancode = inb(0x60);
    
    // Handle key release (bit 7 set)
    bool is_pressed = !(scancode & 0x80);
    scancode = scancode & 0x7F; // Remove release bit
    
    // Call the piano visualization function
    on_key_press(scancode, is_pressed);
    
    // Continue with existing keyboard functionality
    // This should still handle the audio playing part
    // Translate the scancode to ASCII
    char ascii = scancode_to_ascii(scancode);
    
    // If a valid ASCII character was generated, add it to the buffer and print it
    if (ascii) {
        keyboard_buffer_add(ascii);
        putchar(ascii);

        if (ascii >= '1' && ascii <= '8') {
            play_key_note(ascii);
        }
    }
}

// Add a character to the keyboard buffer
void keyboard_buffer_add(char c) {
    if (buffer_count < 256) {
        keyboard_buffer[buffer_end] = c;
        buffer_end = (buffer_end + 1) % 256;
        buffer_count++;
    }
}

// Get a character from the keyboard buffer
char keyboard_buffer_get() {
    if (buffer_count > 0) {
        char c = keyboard_buffer[buffer_start];
        buffer_start = (buffer_start + 1) % 256;
        buffer_count--;
        return c;
    }
    return 0;
}

// Get the number of characters in the buffer
uint8_t keyboard_buffer_size() {
    return buffer_count;
}

// Initialize the keyboard
void init_keyboard() {
    // Register keyboard handler to IRQ1
    register_irq_handler(1, keyboard_handler);
    printf("Keyboard initialized\n");
}
