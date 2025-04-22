#include "libc/stdint.h"
#include "libc/util.h"
#include "libc/stdio.h"
#include "libc/keyboard.h"
#include "libc/idt.h"
#include <libc/stdbool.h>  
#include "libc/common.h"
#include "libc/isr.h"
#include "libc/teminal.h"  // For terminal_putc


char lowercase[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ',
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', 0, 0, 0, '+', 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0
};
    


static int get_scancode()
{
    //Initializes the variable scancode to 0.
    int scanCode = 0;

    //Enters an infinite loop.
    while(1)
    {
        //Keeps checking if the keyboard has sent a scancode by checking the first bit of the keyboard status port.
        if((inb(KEYBOARD_STATUS_PORT) & 1) != 0)
        {
            //When a scancode has been recieved it stores it in the scancode variable and exits the function.
            scanCode = inb(KEYBOARD_DATA_PORT);
            break;
        }
        
    }

    //Returns the scancode.
    return scanCode;
}
    

void keyboardHandler(registers_t regs)
{
    int scanCode;  // What key is pressed
    char press = 0;    

    scanCode = get_scancode();  // Get the scancode from the keyboard
    if (scanCode & 0x80)  // If the high bit is set, it's a key release
    {
       return;
    }
    else
    {
        press = lowercase[scanCode]; 
    }

    terminal_putc(press);  
    
}


void initKeyboard()
{
    //capsOn = false;
    //capsLock = false;
    register_interrupt_handler(IRQ1, keyboardHandler);  // Install the keyboard handler for IRQ1
}


