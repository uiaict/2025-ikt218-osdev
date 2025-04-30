#include "libc/scrn.h"
#include "libc/stdbool.h"
#include "pit/pit.h"

#define INPUT_BUFFER_SIZE 128

static char input_buffer[INPUT_BUFFER_SIZE];
static int input_head = 0;
static int input_tail = 0;
static bool shift_pressed = false;

void scrn_init_input_buffer() {
    input_head = 0;
    input_tail = 0;
    shift_pressed = false;
}

void scrn_set_shift_pressed(bool value) {
    shift_pressed = value;
}

bool scrn_get_shift_pressed() {
    return shift_pressed;
}

void scrn_store_keypress(char c) {
    int next_head = (input_head + 1) % INPUT_BUFFER_SIZE;
    if (next_head != input_tail) {
        input_buffer[input_head] = c;
        input_head = next_head;
    }
}

void terminal_write(const char* str, uint8_t color) {
    static size_t row = 0; 
    static size_t col = 0; 
    uint16_t* vga_buffer = VGA_MEMORY;

    for (size_t i = 0; str[i] != '\0'; i++) {
        char c = str[i];

        if (c == '\n') {
            // Go to new line
            row++;
            col = 0;
        } else if (c == '\b') {
            // Backspace: more one column back and delete the character
            if (col > 0) {
                col--;
                size_t index = row * VGA_WIDTH + col;
                vga_buffer[index] = VGA_ENTRY(' ', color); // Write empty space
            }
        } else {
            // Regular character – write it to VGA memory
            size_t index = row * VGA_WIDTH + col;
            vga_buffer[index] = VGA_ENTRY(c, color);
            col++;

            // If we go beyond the right edge, move to the next line
            if (col >= VGA_WIDTH) {
                col = 0;
                row++;
            }
        }

        // If we go beyond the bottom line, scroll up
        if (row >= VGA_HEIGHT) {
            for (size_t y = 1; y < VGA_HEIGHT; y++) {
                for (size_t x = 0; x < VGA_WIDTH; x++) {
                    vga_buffer[(y - 1) * VGA_WIDTH + x] = vga_buffer[y * VGA_WIDTH + x];
                }
            }

            // Empty the last line
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

    // Handle negative numbers for base 10
    if (num < 0 && base == 10) {
        is_negative = 1;
        num = -num;
    }

    // Convert number to string
    do {
        char digit = "0123456789ABCDEF"[num % base];
        str[i++] = digit;
        num /= base;
    } while (num > 0);

    // Addd negative sign if needed
    if (is_negative) {
        str[i++] = '-';
    }

    str[i] = '\0';

    // Reverse the string
    for (int j = 0, k = i - 1; j < k; j++, k--) {
        char temp = str[j];
        str[j] = str[k];
        str[k] = temp;
    }
}



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

void get_input(char* buffer, int max_len) {
    int index = 0;

    while (index < max_len - 1) {
        while (input_head == input_tail) {
            __asm__ volatile("hlt");
        }

        char c = input_buffer[input_tail];
        input_tail = (input_tail + 1) % INPUT_BUFFER_SIZE;

        if (c == '\n' || c == '\r') {
            break;
        } else if (c == '\b') {
            if (index > 0) {
                index--;
                printf("\b \b");
            }
        } else {
            buffer[index++] = c;
            char str[2] = {c, '\0'};
            printf(str);
        }
    }

    buffer[index] = '\0';
    printf("\n");
}


// Compares two strings character by character
int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++; s2++;
    }
    return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}

// Copies a string from src to dest, including '\0'
char *strcpy(char *dest, const char *src) {
    char *ret = dest;
    while ((*dest++ = *src++));
    return ret;
}
