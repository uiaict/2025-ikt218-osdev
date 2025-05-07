#include "input.h"
#include "libc/stdbool.h"

bool capsEnabled = false;

const char largeAscii[] = {'?', '?', '1', '2', '3', '4', '5', '6',
                         '7', '8', '9', '0', '-', '=', '\016', '?', 'Q', 'W', 'E', 'R', 'T', 'Y',
                         'U', 'I', 'O', 'P', '[', ']', '\034', '?', 'A', 'S', 'D', 'F', 'G',
                         'H', 'J', 'K', 'L', ';', '\'', '`', '?', '\\', 'Z', 'X', 'C', 'V',
                         'B', 'N', 'M', ',', '.', '/', '?', '?', '?', ' '};
const char smallAscii[] = {'?', '?', '1', '2', '3', '4', '5', '6',
                         '7', '8', '9', '0', '-', '=', '\016', '?', 'q', 'w', 'e', 'r', 't', 'y',
                         'u', 'i', 'o', 'p', '[', ']', '\034', '?', 'a', 's', 'd', 'f', 'g',
                         'h', 'j', 'k', 'l', ';', '\'', '`', '?', '\\', 'z', 'x', 'c', 'v',
                         'b', 'n', 'm', ',', '.', '/', '?', '?', '?', ' '};

char scancodeToAscii(unsigned char* scanCode){
    unsigned char a = *scanCode;
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
            return 32;
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
                    c = largeAscii[b];
                } else {
                    c = smallAscii[b];
                }
                return c;
            }else
            {
                return 0;
            }
    }
}