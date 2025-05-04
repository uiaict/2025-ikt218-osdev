#include "keyboard.h"
#include "terminal.h"
#include "isr.h"
#include "libc/stdint.h"
#include "libc/stdbool.h"
//#include "interrupts.asm"


#define BUFFER_SIZE 256

char input_buffer[BUFFER_SIZE];
int buffer_index = 0;

bool shift_pressed = false;
extern uint16_t cursor_pos; 
extern uint16_t *vga_buffer;  


unsigned char scancode_to_ascii[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 
    0, '*', 0, ' ', 0
};


void keyboard_callback() 
{
    uint8_t scancode = inb(0x60);

    if (scancode < sizeof(scancode_to_ascii)) {
        char key = scancode_to_ascii[scancode];

        // Enter
        if (scancode == 0x1C) 
        {  
            input_buffer[buffer_index] = '\0';
            printf("\nDu skrev: %s\n", input_buffer);
            buffer_index = 0;
            move_cursor(cursor_pos);
        } 
        // Backspace
        else if (scancode == 0x0E && buffer_index > 0) 
        {  
            buffer_index--; 
            cursor_pos--;  
            vga_buffer[cursor_pos] = ' ' | 0x0700; 
            move_cursor(cursor_pos);  
        } 
        else if (key && buffer_index < BUFFER_SIZE - 1)
        {  
            input_buffer[buffer_index++] = key;
            vga_buffer[cursor_pos] = key | 0x0700;
            move_cursor(cursor_pos);
            print_char(key);
        }
    }
}





void init_keyboard() {
   // printf("Initializing keyboard...\n");  
    register_interrupt_handler(33, keyboard_callback);
}

