#include "libc/stdint.h"
#include "libc/util.h"
#include "libc/stdio.h"
#include "libc/keyboard.h"
#include "libc/idt.h"
#include <libc/stdbool.h>  
#include "libc/common.h"
#include "libc/isr.h"
#include "libc/teminal.h"  // For terminal_putc

// Declare irq_install_handler at the top to avoid implicit declaration warning
//void irq_install_handler(int irq, void (*handler)(registers_t *regs));
//
//bool capsOn;
//bool capsLock;
//
//const uint32_t UNKNOWN = 0xFFFFFFFF;
//const uint32_t ESC = 0xFFFFFFFF - 1;
//const uint32_t CTRL = 0xFFFFFFFF - 2;
//const uint32_t LSHFT = 0xFFFFFFFF - 3;
//const uint32_t RSHFT = 0xFFFFFFFF - 4;
//const uint32_t ALT = 0xFFFFFFFF - 5;
//const uint32_t F1 = 0xFFFFFFFF - 6;
//const uint32_t F2 = 0xFFFFFFFF - 7;
//const uint32_t F3 = 0xFFFFFFFF - 8;
//const uint32_t F4 = 0xFFFFFFFF - 9;
//const uint32_t F5 = 0xFFFFFFFF - 10;
//const uint32_t F6 = 0xFFFFFFFF - 11;
//const uint32_t F7 = 0xFFFFFFFF - 12;
//const uint32_t F8 = 0xFFFFFFFF - 13;
//const uint32_t F9 = 0xFFFFFFFF - 14;
//const uint32_t F10 = 0xFFFFFFFF - 15;
//const uint32_t F11 = 0xFFFFFFFF - 16;
//const uint32_t F12 = 0xFFFFFFFF - 17;
//const uint32_t SCRLCK = 0xFFFFFFFF - 18;
//const uint32_t HOME = 0xFFFFFFFF - 19;
//const uint32_t UP = 0xFFFFFFFF - 20;
//const uint32_t LEFT = 0xFFFFFFFF - 21;
//const uint32_t RIGHT = 0xFFFFFFFF - 22;
//const uint32_t DOWN = 0xFFFFFFFF - 23;
//const uint32_t PGUP = 0xFFFFFFFF - 24;
//const uint32_t PGDOWN = 0xFFFFFFFF - 25;
//const uint32_t END = 0xFFFFFFFF - 26;
//const uint32_t INS = 0xFFFFFFFF - 27;
//const uint32_t DEL = 0xFFFFFFFF - 28;
//const uint32_t CAPS = 0xFFFFFFFF - 29;
//const uint32_t NONE = 0xFFFFFFFF - 30;
//const uint32_t ALTGR = 0xFFFFFFFF - 31;
//const uint32_t NUMLCK = 0xFFFFFFFF - 32;

// Ensure that the arrays are the correct size to match the scancodes
char lowercase[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ',
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', 0, 0, 0, '+', 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0
};
    
/*char uppercase[128] = {
    UNKNOWN,ESC,'!','@','#','$','%','^','&','*','(',')','_','+','\b','\t','Q','W','E','R',
'T','Y','U','I','O','P','{','}','\n',CTRL,'A','S','D','F','G','H','J','K','L',':','"','~',LSHFT,'|','Z','X','C',
'V','B','N','M','<','>','?',RSHFT,'*',ALT,' ',CAPS,F1,F2,F3,F4,F5,F6,F7,F8,F9,F10,NUMLCK,SCRLCK,HOME,UP,PGUP,'-',
LEFT,UNKNOWN,RIGHT,'+',END,DOWN,PGDOWN,INS,DEL,UNKNOWN,UNKNOWN,UNKNOWN,F11,F12,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,
UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,
UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,
UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN
};*/

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
    //switch(scanCode)
    //{
    //    case 1:
    //    case 29:
    //    case 56:
    //    case 59:
    //    case 60:
    //    case 61:
    //    case 62:
    //    case 63:
    //    case 64:
    //    case 65:
    //    case 66:
    //    case 67:
    //    case 68:
    //    case 87:
    //    case 88:
    //        break;
    //
    //    case 42:
    //        // shift key
    //        if (press == 0)
    //        {
    //            capsOn = true;
    //        } else {
    //            capsOn = false;
    //        }
    //        break;
    //
    //    case 58:
    //        if (!capsLock && press == 0)
    //        {
    //            capsLock = true;
    //        } else if (capsLock && press == 0)
    //        {
    //            capsLock = false;
    //        }
    //        break;
    //
    //    default:
    //        if (press == 0)
    //        {
    //            char c;
    //            if (capsOn || capsLock)
    //            {
    //                c = lowercase[scanCode];
    //                terminal_putc(c);   // Pass    a single char to kprint
    //            } else {
    //                 break;
    //            }
    //        
    //        }
    //}
}


void initKeyboard()
{
    //capsOn = false;
    //capsLock = false;
    register_interrupt_handler(IRQ1, keyboardHandler);  // Install the keyboard handler for IRQ1
}


