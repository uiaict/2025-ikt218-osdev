#include "interrupts.h"
#include "libc/stdint.h"
#include "common.h"
#include "terminal.h"
#include "keyboard.h"

// Keyboard buffer
#define KB_BUFFER_SIZE 256
static char kb_buffer[KB_BUFFER_SIZE];
static uint32_t kb_buffer_head = 0;
static uint32_t kb_buffer_tail = 0;

// Scancode buffer (for raw scancodes)
#define SC_BUFFER_SIZE 32
static uint8_t sc_buffer[SC_BUFFER_SIZE];
static uint32_t sc_buffer_head = 0;
static uint32_t sc_buffer_tail = 0;

// US QWERTY keyboard layout scancode to ASCII mapping
static char keyboard_map[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    '-', 0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// Flag for shift key
static int shift_pressed = 0;

// Keyboard handler for IRQ1
// Keyboard handler with expanded functionality
void keyboard_handler(registers_t* regs, void* data) {
    // Read the scancode
    uint8_t scancode = inb(0x60);
    
    // Store the raw scancode in the scancode buffer
    // Only store press events (not releases) to simplify menu navigation
    if (!(scancode & 0x80)) {
        sc_buffer[sc_buffer_head] = scancode;
        sc_buffer_head = (sc_buffer_head + 1) % SC_BUFFER_SIZE;
    }
    
    // Handle shift key press/release
    if (scancode == 0x2A || scancode == 0x36) {         // Shift key pressed
        shift_pressed = 1;
        return;
    } else if (scancode == 0xAA || scancode == 0xB6) {  // Shift key released
        shift_pressed = 0;
        return;
    }
    
    // Skip key release events
    if (scancode & 0x80) {
        return;
    }
    
    // Map the scancode to ASCII
    char c = 0;
    
    switch (scancode) {
        // Letters - same as US layout
        case 0x1E: c = shift_pressed ? 'A' : 'a'; break;
        case 0x30: c = shift_pressed ? 'B' : 'b'; break;
        case 0x2E: c = shift_pressed ? 'C' : 'c'; break;
        case 0x20: c = shift_pressed ? 'D' : 'd'; break;
        case 0x12: c = shift_pressed ? 'E' : 'e'; break;
        case 0x21: c = shift_pressed ? 'F' : 'f'; break;
        case 0x22: c = shift_pressed ? 'G' : 'g'; break;
        case 0x23: c = shift_pressed ? 'H' : 'h'; break;
        case 0x17: c = shift_pressed ? 'I' : 'i'; break;
        case 0x24: c = shift_pressed ? 'J' : 'j'; break;
        case 0x25: c = shift_pressed ? 'K' : 'k'; break;
        case 0x26: c = shift_pressed ? 'L' : 'l'; break;
        case 0x32: c = shift_pressed ? 'M' : 'm'; break;
        case 0x31: c = shift_pressed ? 'N' : 'n'; break;
        case 0x18: c = shift_pressed ? 'O' : 'o'; break;
        case 0x19: c = shift_pressed ? 'P' : 'p'; break;
        case 0x10: c = shift_pressed ? 'Q' : 'q'; break;
        case 0x13: c = shift_pressed ? 'R' : 'r'; break;
        case 0x1F: c = shift_pressed ? 'S' : 's'; break;
        case 0x14: c = shift_pressed ? 'T' : 't'; break;
        case 0x16: c = shift_pressed ? 'U' : 'u'; break;
        case 0x2F: c = shift_pressed ? 'V' : 'v'; break;
        case 0x11: c = shift_pressed ? 'W' : 'w'; break;
        case 0x2D: c = shift_pressed ? 'X' : 'x'; break;
        case 0x15: c = shift_pressed ? 'Y' : 'y'; break;
        case 0x2C: c = shift_pressed ? 'Z' : 'z'; break;
        
        // Numbers row - Norwegian layout
        case 0x02: c = shift_pressed ? '!' : '1'; break;
        case 0x03: c = shift_pressed ? '"' : '2'; break; // " instead of @
        case 0x04: c = shift_pressed ? '#' : '3'; break;
        case 0x05: c = shift_pressed ? '$' : '4'; break; // ¤ is not in ASCII
        case 0x06: c = shift_pressed ? '%' : '5'; break;
        case 0x07: c = shift_pressed ? '&' : '6'; break;
        case 0x08: c = shift_pressed ? '/' : '7'; break; // / instead of &
        case 0x09: c = shift_pressed ? '(' : '8'; break;
        case 0x0A: c = shift_pressed ? ')' : '9'; break;
        case 0x0B: c = shift_pressed ? '=' : '0'; break; // = instead of )
        
        // Special characters - Norwegian layout
        case 0x0C: c = shift_pressed ? '?' : '+'; break; // + instead of -
        case 0x0D: c = shift_pressed ? '`' : '\''; break; // Different from US
        
        // Norwegian-specific keys
        case 0x1A: c = shift_pressed ? 'Å' : 'å'; break; // å/Å
        case 0x1B: c = shift_pressed ? '^' : '¨'; break; // ¨/^ (dead keys)
        case 0x27: c = shift_pressed ? 'Ø' : 'ø'; break; // ø/Ø
        case 0x28: c = shift_pressed ? 'Æ' : 'æ'; break; // æ/Æ
        case 0x29: c = shift_pressed ? '>' : '<'; break; // Keys left of Z
        case 0x2B: c = shift_pressed ? '*' : '\''; break; // Single quote/asterisk
        
        // Common keys - similar to US
        case 0x39: c = ' '; break;                       // Space
        case 0x1C: c = '\n'; break;                      // Enter
        case 0x0E: c = '\b'; break;                      // Backspace
        case 0x0F: c = '\t'; break;                      // Tab
        
        // Norwegian layout for comma, period, minus
        case 0x33: c = shift_pressed ? ';' : ','; break; // Comma
        case 0x34: c = shift_pressed ? ':' : '.'; break; // Period
        case 0x35: c = shift_pressed ? '_' : '-'; break; // Minus
        
        default: c = 0; break;  // Unknown key
    }
    
    // If we mapped to a valid character, output it
    if (c) {
        // Add the character to the buffer
        kb_buffer[kb_buffer_head] = c;
        kb_buffer_head = (kb_buffer_head + 1) % KB_BUFFER_SIZE;
        
        // Display the character
        terminal_putchar(c);
    }
}

// Initialize the keyboard
void keyboard_init() {
    // Write directly to video memory to confirm this function runs
    uint16_t* video_memory = (uint16_t*)0xB8000;
    video_memory[160] = (0x0F << 8) | 'I'; // Row 2, column 0
    video_memory[161] = (0x0F << 8) | 'N'; // Row 2, column 1
    video_memory[162] = (0x0F << 8) | 'I'; // Row 2, column 2
    video_memory[163] = (0x0F << 8) | 'T'; // Row 2, column 3
    
    // Wait for keyboard controller
    while (inb(0x64) & 0x2);
    
    // Enable keyboard
    outb(0x64, 0xAE);
    
    // Clear pending data
    while (inb(0x64) & 0x1) {
        inb(0x60);
    }
    
    // Try both ways of registering
    register_interrupt_handler(IRQ1, keyboard_handler, NULL);
    register_irq_handler(1, keyboard_handler, NULL);  // If this function exists
    
    // Mark registration complete
    video_memory[165] = (0x0F << 8) | 'D';
    video_memory[166] = (0x0F << 8) | 'O';
    video_memory[167] = (0x0F << 8) | 'N';
    video_memory[168] = (0x0F << 8) | 'E';
}

// Get a character from the keyboard buffer
char keyboard_getchar() {
    if (kb_buffer_head == kb_buffer_tail) {
        return 0;  // Buffer is empty
    }
    
    char c = kb_buffer[kb_buffer_tail];
    kb_buffer_tail = (kb_buffer_tail + 1) % KB_BUFFER_SIZE;
    return c;
}

// Get a scancode from the keyboard (non-blocking)
// Returns 0 if no scancode is available
uint8_t keyboard_get_scancode() {
    if (sc_buffer_head == sc_buffer_tail) {
        return 0;  // Buffer is empty
    }
    
    uint8_t scancode = sc_buffer[sc_buffer_tail];
    sc_buffer_tail = (sc_buffer_tail + 1) % SC_BUFFER_SIZE;
    return scancode;
}

// Convert a scancode to an ASCII character
char keyboard_scancode_to_ascii(uint8_t scancode) {
    if (scancode < 128) {
        return keyboard_map[scancode];
    }
    return 0;
}