// http://www.osdever.nsavet/bkerndev/Docs/keyboard.htm (brans kernel dev)
#include "keyboard.h"
#include "libc/stdbool.h"
#include "libc/common.h"
#include "../piano/piano.h"
#include "../song/SongPlayer.h"
#include "../ui/shell.h"

#define INPUT_BUFFER_SIZE 128
static char input_buffer[INPUT_BUFFER_SIZE];
static int buffer_index = 0;
static bool line_ready = false;

extern bool piano_mode_enabled;



unsigned char kbdus[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
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


void read_line(char* buffer) {
  // Wait until a full line is entered (user pressed Enter)
  while (!line_ready) {
      // You can add a CPU halt or yield here if needed
  }

  // Copy input to provided buffer
  int i;
  for (i = 0; i <= buffer_index; i++) {
      buffer[i] = input_buffer[i];
  }
  
  // Reset for next line read
  line_ready = false;
  buffer_index = 0; // Reset buffer index for next line
}


// funksjon som initialiserer keyboard. keyboard er assigna til "irq1" så vi binder keyboard_handler funksjonen til den med register_interrupt_handler
void init_keyboard() {
    monitor_write("Initializing keyboard\n");
    register_interrupt_handler(IRQ1, &keyboard_handler);
}

/* Handles the keyboard interrupt - kan legge til handling for andre ting som f.eks backspace ++ */
void keyboard_handler(registers_t regs)
{
    /* Read from the keyboard's data buffer*/
    unsigned char scancode = inb(0x60);

    // checks if were in piano mode
    if (piano_mode_enabled) {
        if (scancode & 0x80) {
            // Ignore key releases
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
    /* If the top bit of the byte we read from the keyboard is
    *  set, that means that a key has just been released */
    if (scancode & 0x80)
    {
        /* We don't handle key releases right now */
        return;
    }
    else
    {

        /* Check for ESC key (scancode 0x01) to stop music */
        if (scancode == 0x01) {
            stop_song_requested = true;  // Set the flag to stop any playing song
            return;
        }
        /* Check for backspace first - special handling */
        if (scancode == 0x0E) {
            if (buffer_index > 0) {
                buffer_index--;
                monitor_remove_char();
            }
            return;
        }
        

        /* For other keys, get the character */
        char c = kbdus[scancode];
        
        if (c) {
            // Echo character to screen
            monitor_put(c,15);
            
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