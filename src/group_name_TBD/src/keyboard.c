#include "keyboard.h"
#include "io.h"
#include "libc/stdio.h"

void init_keyboard(){
    
    register_interrupt_handler(IRQ1, keyboard_handler);
    return;
}
void keyboard_handler(struct registers){
    uint8_t scan_code = inb(KEYBOARD_DATA_PORT);

    switch (scan_code){
        
        case LSHIFT_PRESS_CODE:
            shift = true;
            break;        
        case RSHIFT_PRESS_CODE:
            shift = true;
            break;
        case LSHIFT_RELEASE_CODE:
            shift = false;
            break;        
        case RSHIFT_RELEASE_CODE:
            shift = false;
            break;
        case CAPSLOCK_PRESS_CODE:
            capslock = !capslock;
            break;
        case ALTGR_PRESS_CODE:
            altgr = true;
            break;
        case ALTGR_RELEASE_CODE:
            altgr = false;
            break;
        
        default:
            break;
    }
    
    if (scan_code > 0x56){
        // scancode for <
        // Prevents printing of charachters on key release
        // Will prevent some keypresses, but they arent implemented anyways
        return;
    }
    
    
    uint8_t c;
    
    if (shift){
        
        c = ASCII_shift_no[scan_code];
        
    } else if(altgr){
        
        c = ASCII_altgr_no[scan_code];
        
    } else if(capslock){
        
        c = ASCII_caps_no[scan_code];
        
    } else{
        
        c = ASCII_no[scan_code];
    }
    
    if ((int)c > 255 || (int)c < 0){
        c = 0;
    }
    
    printf(&c);
}