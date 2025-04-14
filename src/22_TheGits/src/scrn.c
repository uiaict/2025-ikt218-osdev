#include "libc/scrn.h"
#include "libc/stdarg.h"

void terminal_write(const char* str, uint8_t color) {
    static size_t row = 0; 
    static size_t col = 0; 
    uint16_t* vga_buffer = VGA_MEMORY;

    for (size_t i = 0; str[i] != '\0'; i++) {
        char c = str[i];

        if (c == '\n') {
            // Gå til ny linje
            row++;
            col = 0;
        } else if (c == '\b') {
            // Backspace: flytt én kolonne tilbake og slett tegnet
            if (col > 0) {
                col--;
                size_t index = row * VGA_WIDTH + col;
                vga_buffer[index] = VGA_ENTRY(' ', color); // Skriv blankt tegn
            }
        } else {
            // Vanlig tegn – skriv det til VGA-minnet
            size_t index = row * VGA_WIDTH + col;
            vga_buffer[index] = VGA_ENTRY(c, color);
            col++;

            // Hvis vi går utenfor høyre kant, hopp til ny linje
            if (col >= VGA_WIDTH) {
                col = 0;
                row++;
            }
        }

        // Hvis vi går utenfor nederste linje, scroll opp
        if (row >= VGA_HEIGHT) {
            for (size_t y = 1; y < VGA_HEIGHT; y++) {
                for (size_t x = 0; x < VGA_WIDTH; x++) {
                    vga_buffer[(y - 1) * VGA_WIDTH + x] = vga_buffer[y * VGA_WIDTH + x];
                }
            }

            // Tøm siste linje
            for (size_t x = 0; x < VGA_WIDTH; x++) {
                vga_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = VGA_ENTRY(' ', color);
            }

            row = VGA_HEIGHT - 1;
        }
    }
}

void itoa(int num, char* str, int base) {
    int i = 0;
    int is_negative = 0;

    // Håndter negativt tall for base 10
    if (num < 0 && base == 10) {
        is_negative = 1;
        num = -num;
    }

    // Konverter tall til streng
    do {
        char digit = "0123456789ABCDEF"[num % base];
        str[i++] = digit;
        num /= base;
    } while (num > 0);

    // Legg til negativt tegn hvis nødvendig
    if (is_negative) {
        str[i++] = '-';
    }

    str[i] = '\0';

    // Reverser strengen
    for (int j = 0, k = i - 1; j < k; j++, k--) {
        char temp = str[j];
        str[j] = str[k];
        str[k] = temp;
    }
}


/*void printf(const char* format, ...) {
    va_list args;
    va_start(args, format);

    for (const char* ptr = format; *ptr != '\0'; ptr++) {
        if (*ptr == '%' && *(ptr + 1) == 'c') {
            char c = (char)va_arg(args, int);
            terminal_write(&c, VGA_COLOR(15, 0));
            ptr++; // Hopp over 'c'
        } else {
            // Skriv kun én karakter – unngå å skrive hele strengen igjen!
            char ch = *ptr;
            terminal_write(&ch, VGA_COLOR(15, 0));
        }
    }

    va_end(args);
}*/

void printf(const char* format, ...) {
    va_list args;
    va_start(args, format);

    for (const char* ptr = format; *ptr != '\0'; ptr++) {
        if (*ptr == '%' && *(ptr + 1) != '\0') {
            ptr++;
            switch (*ptr) {
                case 'c': {
                    char c = (char)va_arg(args, int);
                    terminal_write(&c, VGA_COLOR(15, 0));
                    break;
                }
                case 'd': {
                    int val = va_arg(args, int);
                    char buf[32];
                    itoa(val, buf, 10);
                    terminal_write(buf, VGA_COLOR(15, 0));
                    break;
                }
                case 'x': {
                    int val = va_arg(args, int);
                    char buf[32];
                    itoa(val, buf, 16);
                    terminal_write(buf, VGA_COLOR(15, 0));
                    break;
                }
                case 's': {
                    char* str = va_arg(args, char*);
                    terminal_write(str, VGA_COLOR(15, 0));
                    break;
                }
                default: {
                    terminal_write("%", VGA_COLOR(15, 0));
                    terminal_write(ptr, VGA_COLOR(15, 0));
                    break;
                }
            }
        } else {
            char ch = *ptr;
            terminal_write(&ch, VGA_COLOR(15, 0));
        }
    }

    va_end(args);
}


void panic(const char* message) {
    printf("KERNEL PANIC: %s\n", message);
    while (1) {
        __asm__ volatile ("cli; hlt");
    }
}
