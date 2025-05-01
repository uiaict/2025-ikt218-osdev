#include "utils.h"
#include "libc/stdbool.h"
#include "../io/keyboard.h"
#include "../io/printf.h"


void memset(void *ptr, char value, uint32_t count){
    char *tmp = (char*)ptr;
    for (; count != 0; count--) {
        *tmp++ = value;
    }
}

//funksjon som skriver til en I/O-port
void outPortB(uint16_t port, uint8_t value)
{
    asm volatile ("outb %0, %1 " : : "a" (value), "dN" (port));
}
//funksjon som leser fra en I/O-port
uint8_t inPortB(uint16_t port){
    uint8_t value;
    asm volatile ("inb %1, %0" : "=a" (value) : "dN" (port));
    return value;
}


void get_input(char* input, int size) {
    int index = 0;
    bool buffer_empty_warning = true;

    while (index < size - 1) {
        // Vent på at et tastetrykk skal være tilgjengelig
        while (keyboard_buffer_empty()) {
            if (buffer_empty_warning) {
                mafiaPrint("Waiting for input...\n");  // Skriv ut at vi venter på input
                buffer_empty_warning = false;
            }
            // Påkall en interrupt istedenfor å bruke hlt direkte
            asm volatile("sti \n hlt");  // Skru på interrupts og vent på en interrupt (dvs. tastetrykk)
        }

        // Les tastetrykk fra tastaturbufferet
        input[index] = read_from_keyboard_buffer();

        // Hvis vi får et enter eller null-byte (slutt på input), bryt ut
        if ((input[index] == '\n' || input[index] == '\0') && index > 0) {
            break;
        }
        index++;
    }

    input[index] = '\0'; 
    
}

int stoi(const char *str){
    int numberFromString = 0;
    while (*str >= '0' && *str <= '9') {
        numberFromString = numberFromString * 10 + (*str - '0');
        str++;
    }
    return numberFromString;
}

