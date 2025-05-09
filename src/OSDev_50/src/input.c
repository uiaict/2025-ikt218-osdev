#include "common.h"
#include "input.h"
#include "libc/system.h"
#include "monitor.h"
#include "interrupts.h"

#define LOG_BUFFER_SIZE 256

static char key_log[LOG_BUFFER_SIZE];
static size_t log_index = 0;

bool capsEnabled = false;

const char large_ascii[] = {'?', '?', '1', '2', '3', '4', '5', '6',
                         '7', '8', '9', '0', '-', '=', '\016', '?', 'Q', 'W', 'E', 'R', 'T', 'Y',
                         'U', 'I', 'O', 'P', '[', ']', '\034', '?', 'A', 'S', 'D', 'F', 'G',
                         'H', 'J', 'K', 'L', ';', '\'', '`', '?', '\\', 'Z', 'X', 'C', 'V',
                         'B', 'N', 'M', ',', '.', '/', '?', '?', '?', ' '};
const char small_ascii[] = {'?', '?', '1', '2', '3', '4', '5', '6',
                         '7', '8', '9', '0', '-', '=', '\016', '?', 'q', 'w', 'e', 'r', 't', 'y',
                         'u', 'i', 'o', 'p', '[', ']', '\034', '?', 'a', 's', 'd', 'f', 'g',
                         'h', 'j', 'k', 'l', ';', '\'', '`', '?', '\\', 'z', 'x', 'c', 'v',
                         'b', 'n', 'm', ',', '.', '/', '?', '?', '?', ' '};

void test_outb();

char scancode_to_ascii(unsigned char* scan_code) {
    unsigned char a = *scan_code;
    switch (a){
        case 1:     //ESC
            return 0;    
        case 14:    // BACK
		    //backspace();
		    return 0; 
        case 15:
            return 0;    
        case 28:    // ENTER
		    return 2;
        case 29:    //CTRL
            return 0;    
        case 42:    // LSHIFT
            capsEnabled = !capsEnabled;
            return 0;
        case 54:   
            capsEnabled = !capsEnabled;
            return 0; 
        case 56:   
            return 0;
        case 57:       //SPACE
            return 3;
        case 58:   
            capsEnabled = !capsEnabled;
            return 0; 
        case 72:    //UP
            return 0;     
        case 75:   //LEFT
            return 0;    
        case 77:    //RIGHT
            return 0;   
        case 80:    //DOWN 
            return 0; 
        case 170:      
            capsEnabled = !capsEnabled;
            return 0;
        default:
            if (a < 57)
            {
                int b = a;
                char c;
                if (capsEnabled) {
                    c = large_ascii[b];
                } else {
                    c = small_ascii[b];
                }
                return c;
            }else
            {
                return 0;
            }
    }
}

void keyboard_handler(registers_t* regs, void* context) {
    (void)regs;    // Mark as unused
    (void)context; // Mark as unused

    uint8_t scancode = inb(0x60); // Read scancode from the keyboard controller
    char ascii = scancode_to_ascii(&scancode);

    if (ascii != 0) {
        monitor_put(ascii); // Display the character on the screen
    }

    outb(0x20, 0x20); // Send EOI to the PIC
}

void keyboard_logger(registers_t* regs, void* context) {
    (void)regs;    // Mark as unused
    (void)context; // Mark as unused

    uint8_t scancode = inb(0x60); // Read scancode from the keyboard controller
    char ascii = scancode_to_ascii(&scancode);

    if (ascii != 0) {
        // Add the key to the log buffer
        if (log_index < LOG_BUFFER_SIZE - 1) {
            key_log[log_index++] = ascii;
            key_log[log_index] = '\0'; // Null-terminate the string
        }

        // Display the key on the screen
        monitor_put(ascii);
    }

    // Send EOI to the PIC
    outb(0x20, 0x20);
}

void print_key_log() {
    monitor_writestring("Key Log: ");
    monitor_writestring(key_log);
    monitor_put('\n');
}

void test_outb() {
    outb(0x20, 0x20);
}
