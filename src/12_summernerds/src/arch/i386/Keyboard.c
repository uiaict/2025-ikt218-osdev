

// keyboard.c

#include "libc/stdint.h"
#include "libc/stddef.h"
#include "ISR.h"
#include "IRQ.h"
#include "keyboard.h"     
#include "libc/stdbool.h"
#include "io.h"
#include "../src/screen.h"
#include "print.h"




void print(const char* fmt, ...);
void write_line_to_terminal(const char* str, int line);


bool shiftPressed = false;
bool capsEnabled = false;

// 1. Scancode-tabell
char small_scancode_ascii[128] = 
{
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 
    '=', '-', '+', '*', '+', '?', '!', 'a', 'b', 'c',
    'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w',
    'x', 'z', 'y', '.', ',', ' '
    
};

char large_scancode_ascii[128] = 
{
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 
    '=', '-', '+', '*', '+', '?', '!', 'A', 'B', 'C',
    'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
    'X', 'Z', 'Y', '.', ',', ' '
    
};

// 2. Buffer for tastetrykk
char key_buffer[256];
int key_index = 0;


// switch Casene
char scanCodeToASCII(unsigned char* scanCode)
{
    unsigned char word = *scanCode;
    switch (word)                            ///////hvilke flere caser trenger jeg? tab pressed(0x0F)???? og released(0x8F)??????
    {
        case 0x3A:    //CapsLock pressed
        capsEnabled = !capsEnabled;
        return 0;

        case 0xBA:    // CapsLock released
        capsEnabled = !capsEnabled;
        return 0;

        case 0x53:    //delete pressed
        return 0;

        case 0xD3:  //delete released
        return 0;

        case 0x39:   //space pressed
        return 3;

        case 0x1C:  //enter pressed
        return 2;

        case 0x9C:   // enter released
        return 2;

        case 0x2A:   //Left shift pressed 
        shiftPressed = true;
        return 0;

        case 0xAA:   //left shift released
        shiftPressed = false;
        return 0;

        case 0x36:   ///right shift pressed
        shiftPressed = true;
        return 0;

        case 0xB6:   //right shift released
        shiftPressed = false;
        return 0;

        case 0x48:   //cursor up
        return 0;

        case 0x50:   //cursor down
        return 0;

        case 0x49:   // cursor høyre
        return 0;

        case 0x4B:  // cursor venstre
        return 0;

        case 0x01:   //esc pressed
        return 0;

        case 0x81:   //esc released
        return 0;

        case 0x0E:   //backspce pressed
        return 0;

        case 0x8E:   //backspce released
        return 0; 

        default:

        if (word < 128)
        {
            if (capsEnabled ^shiftPressed)
            {
                return large_scancode_ascii[word];
            }else{
                return small_scancode_ascii[word];
            }
        }
        return 0;
    }
}




// 3. Keyboard handler
void keyBoard_handler() {
    unsigned char ScanCode = inPortB(0x60) & 0x7F;     //leser scancoden
    char Press = inPortB(0x60) & 0x80;     //hvis koden kræsjer kan det vaære pga. press ///kommenter den vekk
    char ascii = scanCodeToASCII(&ScanCode);   //sender scancoden til funksjonen og få tilbake bokstav

    if (ascii == 0)
    return;

    key_buffer[key_index++] = ascii;
    key_buffer[key_index] = '\0';

    char str[2] = {ascii, '\0'};

    write_to_terminal(str, 15);
    write_to_terminal(key_buffer, 16);

   
    //printf("scan code: %d, press: %d\r\n", ScanCode, Press);   /////og dennne siden det står press her også
}

// 4. Init-funksjon
void init_keyboard() {


    register_irq_handler(1, keyBoard_handler);
}



