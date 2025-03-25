#include "vga.h"
#include "../util/util.h"

int cursor_x = 0;
int cursor_y = 0;

volatile char * vga_buffer = (volatile char*)VGA_ADDRESS;

void printf(const char* str, ...) {
    int length = strlen(str);
    va_list args;
    va_start(args,str);

    for(int i = 0; i < length; i++) {
        if(str[i] == '%' && (str[i+1] == 'd' || str[i+1] == 'i')) {
            char buffer[32];
            int val = va_arg(args, int);
            itoa(val,buffer ,10);
            kernel_write(15,buffer);
            i++;
        } else if(str[i] == '%' && str[i+1] == 'c') {
            char val = va_arg(args, int);
            printChar(15,val); 
            i++;
        } else if(str[i] == '%' && str[i+1] == 's') {
            char* val = va_arg(args, char*);
            kernel_write(15,val); 
            i++;
        } else if(str[i] == '%' && str[i+1] == 'f') {
            char fstr[32];
            float val = va_arg(args, double);
            int prec = 6;
            ftoa(val, fstr, prec);
            kernel_write(15, fstr);
            i++;
        } else {
            printChar(15,str[i]);
        }   
    }
}

void printChar(int color, char c) {
    if (c == '\n' || cursor_x >= 80) {
        cursor_x = 0;
        cursor_y++;
    }

    if (c != '\n') {  // Ensure newlines don't write extra characters
        int index = (cursor_y * 80 + cursor_x) * 2;
        vga_buffer[index] = c; 
        vga_buffer[index + 1] = color; 
        cursor_x++;
    }
}

void kernel_write(int color, const char* str) {
    
    while(*str != '\0') {
        if(cursor_x >=80 || *str == '\n') {
            cursor_x = 0;
            cursor_y++;
        }
        if(*str != '\n') {
            int index = (cursor_y * 80 + cursor_x) * 2;
            vga_buffer[index] = *str; 
            vga_buffer[index+1] = color; 
            cursor_x++;
        }
        str++;
    }
}