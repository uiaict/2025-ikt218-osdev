#include "input.h"
#include "interrupts.h"
#include "libc/system.h"
#include "libc/io.h"
#include "terminal.h"

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

static void keyboard_handler(int int_no, void* ctx);

void init_input(void) {
    uint8_t mask = inb(0x21);
    mask &= ~(1 << 1);
    outb(0x21, mask);

    // Register the C handler for IRQ1
    register_irq_handler(IRQ1, keyboard_handler, NULL);
}

// This function gets called on each keyboard interrupt
static void keyboard_handler(int int_no, void* ctx) {
    uint8_t scan = inb(0x60);
    char c = scancode_to_ascii(&scan);
    if (c) {
        terminal_put(c);
    }
}