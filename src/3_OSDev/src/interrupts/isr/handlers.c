#include "interrupts.h"
#include "utils.h"
#include "descriptor_table.h"
#include "pit.h"
#include "menu.h"
#include "games/snakes/snakes.h"
#include <libc/stdint.h>
#include <libc/stddef.h>
#include <libc/stdbool.h>
#include <libc/stdio.h>

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
    char ascii_char = 0;
    if (scancode < 128) {
        ascii_char = shift_pressed 
            ? scancode_ascii_upper[scancode] 
            : scancode_ascii_lower[scancode];
    }
    if (menu_active) {    
        // If Up Arrow, down arrow, or enter is pressed
        if (ascii_char == 'w' || ascii_char == 's' || ascii_char == '\n' || ascii_char == 27) {
            handle_menu_input(ascii_char);
        }
    }
    else if (snakes_active) {
        handle_snake_input(ascii_char);
    } 
    else {
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
            }
    
            // Check bounds
            if (scancode < 128) {
                char ascii_char = shift_pressed 
                    ? scancode_ascii_upper[scancode] 
                    : scancode_ascii_lower[scancode];
                
                // If Backspace is pressed
                if (ascii_char == '\b') {
                    printf(0x0F, "\b \b");
                }
                // If Enter is pressed
                if (ascii_char == '\n') {
                    printf(0x0F, "\n");
                }
                // If Tab is pressed
                if (ascii_char == '\t') {
                    printf(0x0F, "\t");
                }
    
                // Finally print the ASCII character
                printf(0x0F, "%c", ascii_char);
            }
        }        
    }
    outb(0x20, 0x20); 
}

void mouse_handler(void) {
    // Print the mouse event
    printf(0x0F, "Mouse Interrupt Triggered\n");
}

void network_handler(void) {
    printf(0x0F, "Network Interrupt Triggered\n");
}

void init_irq_handlers(void) {
    register_irq_handlers(IRQ1, keyboard_handler);
    register_irq_handlers(IRQ12, mouse_handler);
    register_irq_handlers(IRQ10, network_handler);
}


void enable_interrupts()
{
    // Enable interrupts by setting the IF flag in the FLAGS register
    __asm__("sti");
}