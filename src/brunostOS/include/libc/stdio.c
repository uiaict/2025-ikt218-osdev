#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"
#include "string.h"
#include "stdlib.h"
#include "speaker.h"
#include "io.h"
#include "libc/stdarg.h"
#include "memory/memutils.h"

extern uint8_t terminal_color;
extern int cursor_xpos;
extern int cursor_ypos;
extern char *video_memory;


void verify_cursor_pos(){
    // Fix cursor if OoB
    
    if (cursor_xpos >= VGA_WIDTH){
        cursor_xpos = 0;
        cursor_ypos++;
    }
    if (cursor_ypos >= VGA_HEIGHT){
        cursor_ypos = VGA_HEIGHT-1;        
    }
    if (cursor_xpos < 0 && cursor_ypos != 0){
        cursor_ypos--;
        cursor_xpos = VGA_WIDTH-1;
    }
    if (cursor_xpos < 0 && cursor_ypos == 0){
        cursor_ypos = 0;
        cursor_xpos = 0;
    }
    if (cursor_ypos < 0){
        cursor_ypos = 0;
        cursor_xpos = 0;
    }
}

void ctrlchar(int c){
    
    switch (c){
        case '\n':
            cursor_ypos++;
            break;
            
        case '\r':
            cursor_xpos = 0;
            break;
            
        case '\t':
            cursor_xpos = (cursor_xpos / 8 + 1) * 8;
            break;
            
        case '\b':
            cursor_xpos--;
            putchar(' ');
            cursor_xpos--;
            break; 
            
        case '\f':
            cursor_ypos = (cursor_ypos / VGA_HEIGHT + 1) * VGA_HEIGHT;
            cursor_xpos = 0;
            break;
            
        case '\a':
            // Speaker must be enabled
            beep();
            break;
        
        default:
            break;
    }
    verify_cursor_pos();
}


void print(const unsigned char *string){
    
    for (size_t i = 0; i < strlen(string); i++) {
        putchar((int)string[i]);        
    }
}

int putchar(const int c){
    
    if (c > 255 || c < 0){
        return false;
    }
    
    if (c < 32){
        ctrlchar(c);
        return true;
    }
    
    const size_t index = cursor_ypos * VGA_WIDTH + cursor_xpos;
    video_memory[index*2] = (unsigned char)c;
    video_memory[index*2 +1] = terminal_color;
    
    cursor_xpos++;
    verify_cursor_pos();
    
    return true;
}

int printf(const char* __restrict__ format, ...) {
	va_list parameters;
	va_start(parameters, format);
 
	int written = 0;
 
	while (*format != '\0') {

        if (*format != '%'){
            putchar(*format);
            format++;
            written++;
            continue;
        }

        //Char was '%', check what is next
        format++;

        // %% prints %
        if (*format == '%'){
            putchar(*format);
            format++;
            written++;
            continue;
        }

        // %i and %d for int
        if (*format == 'd' || *format == 'i'){
            int num = va_arg(parameters, int);
            unsigned char strnum[15] = {0};
            itoa(num, (char*)strnum);
            print(strnum);
            format++;
            written += strlen((char*)strnum);
            continue;
        }   

        // %s for string (char*)
        if(*format == 's'){
            const unsigned char *string = va_arg(parameters, const unsigned char*);
            print(string);
            format++;
            written += strlen((char*)string);
            continue;
        }

        // %c for char
        if (*format == 'c'){
            // const unsigned char is promoted to int, would abort if uchar
            const unsigned char c = va_arg(parameters, int); 
            putchar((int)c);
            format++;
            written++;
            continue;
        }

        // %u for unsigned int
        if (*format == 'u'){
            unsigned int num = va_arg(parameters, unsigned int);
            unsigned char strnum[15] = {0};
            utoa(num, (char*)strnum);
            print(strnum);
            format++;
            written += strlen((char*)strnum);
            continue;
        } 

        // %x for hex
        if (*format == 'x'){
            unsigned int num = va_arg(parameters, unsigned int);
            unsigned char strnum[15] = {0};
            xtoa(num, (char*)strnum);
            print(strnum);
            format++;
            written += strlen((char*)strnum);
            continue;
        } 

        // %f for double
        if (*format == 'f'){
            double num = va_arg(parameters, double);
            unsigned char strnum[15] = {0};
            ftoa(num, (char*)strnum, 6);
            print(strnum);
            format++;
            written += strlen((char*)strnum);
            continue;
        } 

        if (*format == '.'){
            int precision = 0;
            format++;
            // 32bit barely allows 10 digit precision, max would be 0.4294967296 (unsigned long int), 
            // printf("%.10f", 0.4294967295); maxe due to floating point (non)precision
            while (*format >= '0' && *format <= '9'){ // If charcode for format is within charcode for 0-9
                precision = precision * 10 + (*format - '0'); // For every loop, a leading 0 is added, then format is appended
                format++;
            }
            if (*format == 'f'){
                double num = va_arg(parameters, double);
                unsigned char strnum[15] = {0};
                ftoa(num, (char*)strnum, precision);
                print(strnum);
                format++;
                written += strlen((char*)strnum);
                continue;
            }
            
            
        }

        //Invalid flag, skip over
        format++;
        
	}
 
	va_end(parameters);
    update_cursor();
	return written;
}


int getchar(){
    int buffer_snapshot = buffer_index;

    while (buffer_index == buffer_snapshot){
    }

    unsigned char c = buffer[buffer_index];
    buffer_index--;

    return (int)c;
}

void scanf(unsigned char* __restrict__ format, ...){

    va_list parameters;
	va_start(parameters, format);
 
	while (*format != '\0') {

        if (*format != '%'){
            format++;
            putchar(*format);
            continue;
        }

        //Char was '%', check what is next
        format++;

        unsigned char scanned_str[80]; // Same as terminal width
        for (size_t i = 0; true; i++){
            int c = getchar();
            if (c == '\n'){
                scanned_str[i] = '\0';
                break;
            }
            scanned_str[i] = (unsigned char)c;
        }


        // %i and %d for int
        if (*format == 'd' || *format == 'i'){
            int *num = va_arg(parameters, int*);
            atoi(scanned_str, num);
            format++;
            continue;
        }   

        // %s for string (char*)
        if(*format == 's'){
            unsigned char *string = va_arg(parameters, unsigned char*);
            memcpy(string, scanned_str, strlen(scanned_str));
            format++;
            continue;
        }

        // %c for char
        if (*format == 'c'){
            unsigned char *string = va_arg(parameters, int*);
            *string = scanned_str[0];
            format++;
            continue;
        }

        // %u for unsigned int
        if (*format == 'u'){
            unsigned int *num = va_arg(parameters, unsigned int*);
            atou(scanned_str, num);
            format++;
            continue;
        } 

        // %f for double
        if (*format == 'f'){
            double *num = va_arg(parameters, double*);
            atof(scanned_str, num);
            format++;
            continue;
        } 
            
        //Invalid flag, skip over
        format++;  
    }

 
	va_end(parameters);
        
}