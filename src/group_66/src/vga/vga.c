#include "vga.h"
#include "../util/util.h"
#include "../keyboard/keyboard.h"

uint16_t column = 0;
uint16_t row = 0;
const uint16_t defaultColor = (COLOR8_LIGHT_GREY << 8) | (COLOR8_BLACK << 12); // ([Text_color] << 8) | ([Background_color] << 12)
uint16_t currentColor = defaultColor;

volatile uint16_t* vga_buffer = (volatile uint16_t*)VGA_ADDRESS;

int rowLastColum[25] = {0};

void reset() {
    row = 0;
    column = 0;

    for(uint16_t y = 0; y < VGA_ROWS; y++) {
        for(uint16_t x = 0; x < VGA_COLUMNS; x++) {
            vga_buffer[y*VGA_COLUMNS + x] = ' ' | currentColor;
        }
    }
}

void newLine() {
    if (row < VGA_ROWS-1) {
        rowLastColum[row] = column;
        row++;
        column = 0;
    } else {
        rowLastColum[row] = column;
        scrollUp();
        column = 0;
    }
}

void scrollUp() {
    for(uint16_t y = 1; y < VGA_ROWS; y++) {
        for(uint16_t x = 0; x < VGA_COLUMNS; x++) {
            vga_buffer[(y-1)*VGA_COLUMNS + x] = vga_buffer[(y)*VGA_COLUMNS + x];
        }
        rowLastColum[y-1] = rowLastColum[y];
    }
    //rowLastColum[24] = 0;

    for(uint16_t x = 0; x < VGA_COLUMNS; x++) {
        vga_buffer[(row) * VGA_COLUMNS + x] = ' ' | currentColor;
    }
}

void printf(const char* str, ...) {
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
            case '\b':
                if(column == 0 && row != 0) {
                    row--;
                    column = rowLastColum[row]; 
                } else if(column > 0) {
                    column--;
                    vga_buffer[row * VGA_COLUMNS + column] = ' ' | currentColor;
                }
                
                break;
            case '\t':
                if(column == VGA_COLUMNS) {
                    newLine();
                }
                uint16_t tabLen = 4-(column%4);
                while(tabLen != 0) {
                    vga_buffer[row*VGA_COLUMNS + (column++)] = ' ' | currentColor;
                    tabLen--;
                }
                break;
            case '%':
                if((*(str+1) == 'd' || *(str+1) == 'i')) {
                    char buffer[32] = {0};
                    int val = va_arg(args, int);
                    itoa(val,buffer,10);
                    char* buffer_ptr = buffer;
                    if(column == VGA_COLUMNS){
                        newLine();
                    }
                    while(*buffer_ptr != '\0') {
                        vga_buffer[row * VGA_COLUMNS + (column++)] = *buffer_ptr | currentColor;
                        buffer_ptr++;
                    }
                    str++;
                } else if(*(str+1) == 'c') {
                    char val = va_arg(args, int);
                    if(val == '\b') {
                        if(column == 0 && row != 0) {
                            row--;
                            column = rowLastColum[row]; 
                        } else if(column > 0) {
                            column--;
                        }
                        vga_buffer[row * VGA_COLUMNS + column] = ' ' | currentColor;
                    } else if(val == '\t') {
                        if(column == VGA_COLUMNS) {
                            newLine();
                        }
                        uint16_t tabLen = 4-(column%4);
                        while(tabLen != 0) {
                            vga_buffer[row*VGA_COLUMNS + (column++)] = ' ' | currentColor;
                            tabLen--;
                        }
                    } else if(val == '\n') {
                        newLine();
                    } else if(val == '\r') {
                        column = 0;
                    } else {
                        if(column == VGA_COLUMNS){
                            newLine();
                        }
                        vga_buffer[row * VGA_COLUMNS + (column++)] = val | currentColor; 
                    }   
                    str++;
                } else if(*(str+1) == 's') {
                    char* val = va_arg(args, char*);
                    if(column == VGA_COLUMNS){
                        newLine();
                    }
                    while(*val != '\0') {
                        vga_buffer[row * VGA_COLUMNS + (column++)] = *val | currentColor;
                        val++;
                    }
                    str++;
                } else if(*(str+1) == 'f') {
                    char fstr[32];
                    float val = va_arg(args, double);
                    int prec = 6;
                    ftoa(val, fstr, prec);
                    char* fstr_ptr = fstr;
                    if(column == VGA_COLUMNS){
                        newLine();
                    }
                    while(*fstr_ptr != '\0') {
                        vga_buffer[row * VGA_COLUMNS + (column++)] = *fstr_ptr | currentColor;
                        fstr_ptr++;
                    }
                    str++;
                } else {
                    if(column == VGA_COLUMNS){
                        newLine();
                    }
                    vga_buffer[row * VGA_COLUMNS + (column++)] = *str | currentColor;
                }
                break;
            default:
                if(column == VGA_COLUMNS){
                    newLine();
                }
                vga_buffer[row * VGA_COLUMNS + (column++)] = *str | currentColor;
                break;
        }
        str++;
        update_cursor(column,row);
    }
}
