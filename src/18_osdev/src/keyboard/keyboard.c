// adopted from http://www.osdever.net/bkerndev/Docs/keyboard.htm (brans kernel dev)
#include "keyboard.h"

#define INPUT_BUFFER_SIZE 128
static char input_buffer[INPUT_BUFFER_SIZE];
static int buffer_index = 0;
static bool line_ready = false;
static bool shift_pressed = false; // Track shift key state

extern bool piano_mode_enabled;

// standard keyboard layout for us keyboards (lowercase)
unsigned char kbdus[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift (42) */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift (54) */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

// Shifted keyboard layout (uppercase and special characters)
unsigned char kbdus_shift[128] =
{
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*',  /* 9 */
  '(', ')', '_', '+', '\b',  /* Backspace */
  '\t',      /* Tab */
  'Q', 'W', 'E', 'R',  /* 19 */
  'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',  /* Enter key */
    0,      /* 29   - Control */
  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',  /* 39 */
 '"', '~',   0,    /* Left shift */
 '|', 'Z', 'X', 'C', 'V', 'B', 'N',      /* 49 */
  'M', '<', '>', '?',   0,        /* Right shift */
  '*',
    0,  /* Alt */
  ' ',  /* Space bar */
    0,  /* Caps lock */
    0,  /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,  /* < ... F10 */
    0,  /* 69 - Num lock*/
    0,  /* Scroll Lock */
    0,  /* Home key */
    0,  /* Up Arrow */
    0,  /* Page Up */
  '-',
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
  '+',
    0,  /* 79 - End key*/
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0,   0,   0,
    0,  /* F11 Key */
    0,  /* F12 Key */
    0,  /* All other keys are undefined */
};

void read_line(char* buffer) {
  // Wait until a full line is entered (user pressed Enter)
  while (!line_ready);

  // Copy input to provided buffer
  int i;
  for (i = 0; i <= buffer_index; i++) {
      buffer[i] = input_buffer[i];
  }
  
  // Reset for next line read
  line_ready = false;
  buffer_index = 0; // Reset buffer index for next line
}

// initializing keyboard. keyboard is normally assigned to irq1. Here we bind keyboard_handler() to irq1 with register_interrupt_handler
void init_keyboard() {
    monitor_write("Initializing keyboard\n");
    register_interrupt_handler(IRQ1, &keyboard_handler);
}

// Handles the keyboard interrupt
void keyboard_handler(registers_t regs)
{
    // Read from the keyboard's data buffer
    unsigned char scancode = inb(0x60);

    // checks if we are in piano mode
    if (piano_mode_enabled) {
        if (scancode & 0x80) { // Ignore key releases
            stop_sound(); // stops the speaker
            return;
        }

        if (scancode == 0x01) { // ESC key to exit piano mode
            piano_mode_enabled = false;
            monitor_write("\nExited piano mode.\n> ");
            stop_sound(); // stops the speaker
            return;
        }

        handle_piano_key(scancode);
        return; // Skip normal keyboard input while in piano mode
    }

    /* Handle key release (top bit set) */
    if (scancode & 0x80)
    {
        // Check if released key is shift
        unsigned char released_key = scancode & 0x7F; // Remove the top bit to get the actual key
        
        if (released_key == 0x2A || released_key == 0x36) { // Left or right shift
            shift_pressed = false; // shift was released
        }
        return;
    }
    
    else
    {
        /* Handle key press */

        /* Check for shift keys first */
        if (scancode == 0x2A || scancode == 0x36) { // Left or right shift
            shift_pressed = true;
            return;
        }
        
        /* Check for ESC key (scancode 0x01) to stop music */
        if (scancode == 0x01) {
            stop_song_requested = true;  // Set the flag to stop any playing song
            return;
        }
        
        /* Check for backspace */
        if (scancode == 0x0E) {
            if (buffer_index > 0) {
                buffer_index--; // remove char from buffer index
                monitor_remove_char(); // remove char from display
            }
            return;
        }

        /* For other keys, get the character based on shift state */
        char c;
        if (shift_pressed) {
            c = kbdus_shift[scancode]; // use shift keyboard layout
        } else {
            c = kbdus[scancode]; // use normal keyboard
        }
        
        if (c) {
            // Echo character to screen
            monitor_put(c, color);
            
            // Handle special characters
            if (c == '\n') {
                // Enter key pressed - set line as ready
                input_buffer[buffer_index] = '\0'; // Null-terminate the string
                line_ready = true;
            } 
            else {
                // Regular character - add to buffer if there's space
                if (buffer_index < INPUT_BUFFER_SIZE - 1) {
                    input_buffer[buffer_index++] = c;
                }
            }
        }
    }
}