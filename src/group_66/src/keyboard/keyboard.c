#include "../idt/idt.h"
#include "../vga/vga.h"
#include "keyboard.h"
#ifndef SRC_UTIL_UTIL_H
#define SRC_UTIL_UTIL_H

bool capsOn;
bool capsLock;

const uint32_t UNKNOWN = 0xFFFFFFFF;
const uint32_t ESC = 0xFFFFFFFF - 1;
const uint32_t CTRL = 0xFFFFFFFF - 2;
const uint32_t LSHFT = 0xFFFFFFFF - 3;
const uint32_t RSHFT = 0xFFFFFFFF - 4;
const uint32_t ALT = 0xFFFFFFFF - 5;
const uint32_t F1 = 0xFFFFFFFF - 6;
const uint32_t F2 = 0xFFFFFFFF - 7;
const uint32_t F3 = 0xFFFFFFFF - 8;
const uint32_t F4 = 0xFFFFFFFF - 9;
const uint32_t F5 = 0xFFFFFFFF - 10;
const uint32_t F6 = 0xFFFFFFFF - 11;
const uint32_t F7 = 0xFFFFFFFF - 12;
const uint32_t F8 = 0xFFFFFFFF - 13;
const uint32_t F9 = 0xFFFFFFFF - 14;
const uint32_t F10 = 0xFFFFFFFF - 15;
const uint32_t F11 = 0xFFFFFFFF - 16;
const uint32_t F12 = 0xFFFFFFFF - 17;
const uint32_t SCRLCK = 0xFFFFFFFF - 18;
const uint32_t HOME = 0xFFFFFFFF - 19;
const uint32_t UP = 0xFFFFFFFF - 20;
const uint32_t LEFT = 0xFFFFFFFF - 21;
const uint32_t RIGHT = 0xFFFFFFFF - 22;
const uint32_t DOWN = 0xFFFFFFFF - 23;
const uint32_t PGUP = 0xFFFFFFFF - 24;
const uint32_t PGDOWN = 0xFFFFFFFF - 25;
const uint32_t END = 0xFFFFFFFF - 26;
const uint32_t INS = 0xFFFFFFFF - 27;
const uint32_t DEL = 0xFFFFFFFF - 28;
const uint32_t CAPS = 0xFFFFFFFF - 29;
const uint32_t NONE = 0xFFFFFFFF - 30;
const uint32_t ALTGR = 0xFFFFFFFF - 31;
const uint32_t NUMLCK = 0xFFFFFFFF - 32;


const uint32_t lowercase[128] = {
    UNKNOWN,ESC,'1','2','3','4','5','6','7','8',
    '9','0','-','=','\b','\t','q','w','e','r',
    't','y','u','i','o','p','[',']','\n',CTRL,
    'a','s','d','f','g','h','j','k','l',';',
    '\'','`',LSHFT,'\\','z','x','c','v','b','n','m',',',
    '.','/',RSHFT,'*',ALT,' ',CAPS,F1,F2,F3,F4,F5,F6,F7,F8,F9,F10,NUMLCK,SCRLCK,HOME,UP,PGUP,'-',LEFT,UNKNOWN,RIGHT,
    '+',END,DOWN,PGDOWN,INS,DEL,UNKNOWN,UNKNOWN,UNKNOWN,F11,F12,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,
    UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,
    UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,
    UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN
};
    
const uint32_t uppercase[128] = {
    UNKNOWN,ESC,'!','@','#','$','%','^','&','*','(',')','_','+','\b','\t','Q','W','E','R',
    'T','Y','U','I','O','P','{','}','\n',CTRL,'A','S','D','F','G','H','J','K','L',':','"','~',LSHFT,'|','Z','X','C',
    'V','B','N','M','<','>','?',RSHFT,'*',ALT,' ',CAPS,F1,F2,F3,F4,F5,F6,F7,F8,F9,F10,NUMLCK,SCRLCK,HOME,UP,PGUP,'-',
    LEFT,UNKNOWN,RIGHT,'+',END,DOWN,PGDOWN,INS,DEL,UNKNOWN,UNKNOWN,UNKNOWN,F11,F12,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,
    UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,
    UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,
    UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN
};


void initKeyboard() {
    capsOn = false;
    capsLock = false;
    irq_install_handler(1,&keyboardHandler);
}

void printRaw(char scancode){
    printf("Scancode: %d\n", scancode);
}

char getScanCode() {
    uint8_t scanCode = inPortB(0x60); // What key is pressed
    return scanCode;
}

void keyboardHandler(struct InterruptRegisters *regs) {
    uint8_t scanCode = inPortB(0x60) & 0x7F; // What key is pressed
    uint8_t press = inPortB(0x60) & 0x80; // Is the key pressed down or released

    switch(scanCode) {
        case 1:
        case 29:
        case 56:
        case 59:
        case 60: 
        case 61:
        case 62:
        case 63:
        case 64:
        case 65:
        case 66:
        case 67:
        case 68:
        case 87:
        case 88:
            break;
        case 42:
            if(press == 0) {
                capsOn = true;
            } else {
                capsOn = false;
            }
            break;
        case 58:
            if(press == 0) {
                capsLock = !capsLock;
            }
            break;
        default:
            if(press == 0) {
                if(capsOn || capsLock) {
                    printf("%c", uppercase[scanCode]);
                } else {
                    printf("%c", lowercase[scanCode]);
                }
            }
            break;   
    }
}

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{   
    
	outPortB(0x3D4, 0x0A); // 0x0A controlls the starting scanline for the cursor. So where in the current position it will start 
	outPortB(0x3D5, (inPortB(0x3D5) & 0xC0) | cursor_start);

	outPortB(0x3D4, 0x0B); // 0x0B controlls the ending scanline for the cursor. So where in the current position it will end 
	outPortB(0x3D5, (inPortB(0x3D5) & 0xE0) | cursor_end);
    
    // scanline starting from 0 and ending at 15 will give full white block of a cursor, while 14 to 15 will give a single line at the bottom. 
    // 0x3D4 and 0x3D5 are both I/O internal ports used to commmunicate betweenthe CPU and VGA hardware. 0x3D4 is for indexing which register we want to change,-
    // in this case 0x0A, while 0x3D5 is used to either read or wrtie from the indexed port at 0x3D4. 
}

void update_cursor(int x, int y)
{
	uint16_t pos = y * VGA_COLUMNS + x;

	outPortB(0x3D4, 0x0F);
	outPortB(0x3D5, (uint8_t) (pos & 0xFF));
	outPortB(0x3D4, 0x0E);
	outPortB(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

#endif