#include <interrupts.h>
#include <descriptor_table.h>
#include <pit.h>
#include <libc/stdint.h>
#include <libc/stddef.h>
#include <libc/stdbool.h>

#define MOUSE_MOVE 0
#define MOUSE_LEFT_CLICK 1
#define MOUSE_RIGHT_CLICK 2
#define MOUSE_SCROLL_UP 3
#define MOUSE_SCROLL_DOWN 4

char scancode_ascii_lower[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z',
    'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0, // etc
};
char scancode_ascii_upper[128] = {
    0, 27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0,
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0, '|',
    'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?'
};

bool shift_pressed = false;
void keyboard_handler(void) {

    uint8_t scancode = inb(0x60);

    if (scancode & 0x80) {
        // This is a key release
        scancode &= 0x7F; // Strip the top bit
        if (scancode == 0x2A || scancode == 0x36) {
            shift_pressed = false;
        }
    } else {
        // This is a key press
        if (scancode == 0x2A || scancode == 0x36) {
            shift_pressed = true;
            return;
        }

        // Check bounds
        if (scancode < 128) {
            char ascii_char = shift_pressed 
                ? scancode_ascii_upper[scancode] 
                : scancode_ascii_lower[scancode];
            
            // If Backspace is pressed
            if (ascii_char == '\b') {
                print(0x0F, "\b \b");
                return;
            }
            // If Enter is pressed
            if (ascii_char == '\n') {
                print(0x0F, "\n");
                return;
            }
            // If Tab is pressed
            if (ascii_char == '\t') {
                print(0x0F, "\t");
                return;
            }

            // Finally print the ASCII character
            print(0x0F, "%c", ascii_char);
        }
    }
}

void mouse_handler(void) {
    // Read the data from the mouse buffer
    uint8_t mouse_data = inb(0x60);

    // Check for any errors in the mouse data
    
    // Determine the type of mouse event
    int event_type = 0;
    if (mouse_data & 0x01) {
        event_type = MOUSE_MOVE;
    } else if (mouse_data & 0x02) {
        event_type = MOUSE_LEFT_CLICK;
    } else if (mouse_data & 0x04) {
        event_type = MOUSE_RIGHT_CLICK;
    } else if (mouse_data & 0x08) {
        event_type = MOUSE_SCROLL_UP;
    } else if (mouse_data & 0x10) {
        event_type = MOUSE_SCROLL_DOWN;
    }

    // Update the cursur position 
    if (event_type == MOUSE_MOVE) {
        
    }
    else if (event_type == MOUSE_LEFT_CLICK) {

    }
    else if (event_type == MOUSE_RIGHT_CLICK) {

    }
    else if (event_type == MOUSE_SCROLL_UP) {

    }
    else if (event_type == MOUSE_SCROLL_DOWN) {

    }
    // Print the mouse event
    print(0x0F, "Mouse Event: %d\n", event_type);
}

void network_handler(void) {
    print(0x0F, "Network Interrupt Triggered\n");
}

void init_irq_handlers(void) {
    register_irq_handlers(IRQ1, keyboard_handler);
    register_irq_handlers(IRQ12, mouse_handler);
}


void enable_interrupts()
{
    // Enable interrupts by setting the IF flag in the FLAGS register
    __asm__("sti");
}