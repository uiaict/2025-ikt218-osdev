#include "keyboard.h"
#include "io.h"
#include "libc/stdio.h"

void init_keyboard(){
    
    register_interrupt_handler(IRQ1, keyboard_handler);
    return;
}
void keyboard_handler(struct registers reg){
    uint8_t scan_code = inb(KEYBOARD_DATA_PORT);

    
    
    if (scan_code & 0x80){
        // If first bit of scancode is 1, a key was just released
        // 10000000 + 00101010 = 10101010
        // 0x80 + SHIFT_PRESS_CODE = SHIFT_RELEASE_CODE

        switch ((scan_code & 0x7F)){
        
            case LSHIFT_CODE:
                shift = false;
                return;        
            case RSHIFT_CODE:
                shift = false;
                return;
            case ALTGR_CODE:
                altgr = false;
                return;
            
            default:
                return; // We dont want to print on release
        }
        
        
    } else{
        
        switch (scan_code){
        
            case LSHIFT_CODE:
                shift = true;
                return;        
            case RSHIFT_CODE:
                shift = true;
                return;
            case ALTGR_CODE:
                altgr = true;
                return;
            case CAPSLOCK_CODE:
                capslock = !capslock;
                return;
                
            default:
                break;
        }
    }
        
    
    uint8_t c = 0;
    
    // If it works, it works
    if (shift && !capslock && !altgr){
        c = ASCII_shift[scan_code];
        
    } else if (!shift && capslock && !altgr){
        c = ASCII_caps[scan_code]; 
        
    } else if (!shift && !capslock && altgr){
        c = ASCII_altgr[scan_code]; 
        
    } else if (shift && capslock && !altgr){
        c = ASCII_caps_shift[scan_code]; 
        
    } else if (shift && !capslock && altgr){
        c = 0;
        
    } else if (!shift && capslock && altgr){
        c = ASCII_altgr[scan_code];
        
    } else if (shift && capslock && altgr){
        c = 0;
        
    } else {
        c = ASCII[scan_code];
    }
    
    
    if ((int)c <= 255 && (int)c != 0){
        putchar((int)c);
    }
    
    if (!shift && scan_code == 0x1C){
        // Generally, we want CRLF when we hit enter
        putchar('\r');
    }   
    
}