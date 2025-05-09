#include "utils.h"

void memset(void *ptr, char value, uint32_t count) {
    char *tmp = (char *)ptr;
    for (; count != 0; count--) {
        *tmp++ = value;
    }
}

// funksjon som skriver til en I/O-port
void outPortB(uint16_t port, uint8_t value) {
    asm volatile("outb %0, %1 " : : "a"(value), "dN"(port));
}

// funksjon som leser fra en I/O-port
uint8_t inPortB(uint16_t port) {
    uint8_t value;
    asm volatile("inb %1, %0" : "=a"(value) : "dN"(port));
    return value;
}

void get_input(char *input, int size) {
    int index = 0;
    bool buffer_empty_warning = true;

    while (index < size - 1) {
        // Vent på tastetrykk
        while (keyboard_buffer_empty()) {
            if (buffer_empty_warning) {
                Print("Waiting for input...\n");
                buffer_empty_warning = false;
            }
            asm volatile("hlt");
        }
        // Les ett tegn
        char c = read_from_keyboard_buffer();

        // Echo til skjerm
        if (c != '\n' && c != '\0')
            putc(c);

        // Legg i input-bufferet
        input[index] = c;

        // Hvis newline og minst ett tegn, avslutt
        if (c == '\n' && index > 0) {
            putc('\n');
            break;
        }
        index++;
    }

    input[index] = '\0';
}


int stoi(const char *str) {
    int numberFromString = 0;
    while (*str >= '0' && *str <= '9') {
        numberFromString = numberFromString * 10 + (*str - '0');
        str++;
    }
    return numberFromString;
}

// Ny funksjon: les ett tegn og returner det
char get_char(void) {
    // Sørg for at interrupts er på globalt (sti kjøres i kernel_main)
    while (keyboard_buffer_empty()) {
        asm volatile("hlt");
    }
    return read_from_keyboard_buffer();
}