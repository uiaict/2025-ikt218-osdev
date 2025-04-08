#include "keyboard.h"

void init_keyboard(){
    

    volatile uint8_t data = inb(KEYBOARD_DATA_PORT);
    volatile uint8_t command = inb(KEYBOARD_COMMAND_PORT);
    

    // while (command == 28)
    // {
    //     data = inb(KEYBOARD_DATA_PORT);
    //     command = inb(KEYBOARD_COMMAND_PORT);
    //     int a = 0;
    // }
    
    
    int a = 0;
    
    
    register_interrupt_handler(IRQ1, keyboard_handler);
}
void keyboard_handler(struct registers){
    uint8_t scanCode = inb(KEYBOARD_DATA_PORT);

    putchar_at(&defaultLookup[scanCode], 0, 0);
}