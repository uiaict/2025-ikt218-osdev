#include "vga.h"
#include "../util/util.h"

uint16_t column = 0;
uint16_t row = 0;
const uint16_t defaultColor = (COLOR8_LIGHT_GREY << 8) | (COLOR8_BLACK << 12); // ([Text_color] << 8) | ([Background_color] << 12)
uint16_t currentColor = defaultColor;

volatile uint16_t* vga_buffer = (volatile uint16_t*)VGA_ADDRESS;

void reset() {
    row = 0;
    column = 0;

    for(uint16_t y = 0; y < VGA_ROWS; y++) {
        for(uint16_t x = 0; x < VGA_COLUMS; x++) {
            vga_buffer[y*VGA_COLUMS + x] = ' ' | currentColor;
        }
    }
}

void newLine() {
    if (row < VGA_ROWS-1) {
        row++;
        column = 0;
    } else {
        scrollUp();
        column = 0;
    }
}

void scrollUp() {
    for(uint16_t y = 0; y < VGA_ROWS; y++) {
        for(uint16_t x = 0; x < VGA_COLUMS; x++) {
            vga_buffer[(y-1)*VGA_COLUMS + x] = vga_buffer[y*VGA_COLUMS + x];
        }
    }

    for(uint16_t x = 0; x < VGA_COLUMS; x++) {
        vga_buffer[(row-1) * VGA_COLUMS + x] = ' ' | currentColor;
    }
}

void print(const char* str, ...) {
    va_list args;
    va_start(args,str);
    while(*str) {
        switch(*str) {
            case '\n':
                newLine();
                break;
            case '\r':
                column = 0;
                break;
            case '\t':
                if(column == VGA_COLUMS) {
                    newLine();
                }
                uint16_t tabLen = 4-(column%4);
                while(tabLen != 0) {
                    vga_buffer[row*VGA_COLUMS + (column++)] = ' ' | currentColor;
                    tabLen--;
                }
                break;
            case '%':
                if((*(str+1) == 'd' || *(str+1) == 'i')) {
                    char buffer[32] = {0};
                    int val = va_arg(args, int);
                    itoa(val,buffer,10);
                    char* buffer_ptr = buffer;
                    if(column == VGA_COLUMS){
                        newLine();
                    }
                    while(*buffer_ptr != '\0') {
                        vga_buffer[row * VGA_COLUMS + (column++)] = *buffer_ptr | currentColor;
                        buffer_ptr++;
                    }
                    str++;
                } else if(*(str+1) == 'c') {
                    char val = va_arg(args, int);
                    if(column == VGA_COLUMS){
                        newLine();
                    }
                    vga_buffer[row * VGA_COLUMS + (column++)] = val | currentColor; 
                    str++;
                } else if(*(str+1) == 's') {
                    char* val = va_arg(args, char*);
                    if(column == VGA_COLUMS){
                        newLine();
                    }
                    while(*val != '\0') {
                        vga_buffer[row * VGA_COLUMS + (column++)] = *val | currentColor;
                        val++;
                    }
                    str++;
                } else if(*(str+1) == 'f') {
                    char fstr[32];
                    float val = va_arg(args, double);
                    int prec = 6;
                    ftoa(val, fstr, prec);
                    char* fstr_ptr = fstr;
                    if(column == VGA_COLUMS){
                        newLine();
                    }
                    while(*fstr_ptr != '\0') {
                        vga_buffer[row * VGA_COLUMS + (column++)] = *fstr_ptr | currentColor;
                        fstr_ptr++;
                    }
                    str++;
                } else {
                    if(column == VGA_COLUMS){
                        newLine();
                    }
                    vga_buffer[row * VGA_COLUMS + (column++)] = *str | currentColor;
                }
                break;
            default:
                if(column == VGA_COLUMS){
                    newLine();
                }
                vga_buffer[row * VGA_COLUMS + (column++)] = *str | currentColor;
                break;
        }
        str++;
    }
}