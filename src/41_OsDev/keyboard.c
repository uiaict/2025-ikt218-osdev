

#include "port_io.h"
#include "terminal.h"
#include <stdint.h>


static const char scancode_table[128] = {
    
    [0x01] = 27,           
    [0x02] = '1', [0x03] = '2', [0x04] = '3', [0x05] = '4',
    [0x06] = '5', [0x07] = '6', [0x08] = '7', [0x09] = '8',
    [0x0A] = '9', [0x0B] = '0', [0x0C] = '+', [0x0D] = '\'',
    [0x0E] = '\b',         
    [0x0F] = '\t',          
    [0x10] = 'q', [0x11] = 'w', [0x12] = 'e', [0x13] = 'r',
    [0x14] = 't', [0x15] = 'y', [0x16] = 'u', [0x17] = 'i',
    [0x18] = 'o', [0x19] = 'p', [0x1A] = 'a',    
    [0x1B] = '^',      
    [0x1C] = '\n',          
    [0x1E] = 'a', [0x1F] = 's', [0x20] = 'd', [0x21] = 'f',
    [0x22] = 'g', [0x23] = 'h', [0x24] = 'j', [0x25] = 'k',
    [0x26] = 'l', [0x27] = 'o',    
    [0x28] = 'z',     
    [0x29] = '`',    

    
    [0x2B] = '\\',           
    [0x2C] = 'z', [0x2D] = 'x', [0x2E] = 'c', [0x2F] = 'v',
    [0x30] = 'b', [0x31] = 'n', [0x32] = 'm',
    [0x33] = ',', [0x34] = '.', [0x35] = '-',      
    [0x37] = '*',             
    [0x39] = ' ',             
};


void keyboard_handler(void)
{
    static uint8_t extended = 0;
    uint8_t sc = inb(0x60);

    
    if (sc == 0xE0) { extended = 1; return; }

   
    if (sc & 0x80) { extended = 0; return; }

    
    if (extended) { extended = 0; return; }

   
    if (sc < sizeof scancode_table) {
        char ch = scancode_table[sc];
        if (ch) {
            char buf[2] = { ch, 0 };
            terminal_write(buf);
        }
    }
}
