#include "libc/string.h"
#include "libc/stdarg.h"

// Funksjonsdeklarasjoner
void reverse(char str[], int length);
char* itoa(int num, char* str, int base);

void putchar(char c, char* video_memory, size_t index) {
    video_memory[index * 2] = c;      // Tegn
    video_memory[index * 2 + 1] = 0x07; // Attributtbyte (lys grå på svart)
}

void print_string(const char* str, char* video_memory) {
    size_t len = strlen(str);
    for (size_t i = 0; i < len; i++) {
        putchar(str[i], video_memory, i);
    }
}

void print_integer(int num, char* video_memory) {
    char buffer[12]; // Nok til å holde INT_MIN
    itoa(num, buffer, 10); // Konverter heltall til streng
    print_string(buffer, video_memory);
}

void printf(const char* format, ...) {
    char* video_memory = (char*)0xb8000; // Videominneadresse
    va_list args;
    va_start(args, format);
    
    size_t index = 0; // Indeks for videominne
    char buffer[256] = {0}; // Buffer for midlertidig lagring

    for (const char* p = format; *p != '\0'; p++) {
        if (*p == '%' && *(p + 1) == 'd') {
            int i = va_arg(args, int);
            itoa(i, buffer, 10); // Konverter heltall til streng
            print_string(buffer, video_memory + index * 2);
            index += strlen(buffer); // Flytt indeksen fremover
            p++; // Hopp over 'd'
        } else if (*p == '%' && *(p + 1) == 's') {
            char* s = va_arg(args, char*);
            print_string(s, video_memory + index * 2);
            index += strlen(s); // Flytt indeksen fremover
            p++; // Hopp over 's'
        } else {
            putchar(*p, video_memory, index++);
        }
    }

    va_end(args);
}
