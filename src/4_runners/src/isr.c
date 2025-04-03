#include "isr.h"
#include "libc/string.h"
#include "libc/stdio.h"
#include "string.h"

void terminal_write(const char* str); // Forward declaration

// Simple integer-to-string conversion
void int_to_string(int value, char* buffer) {
    char temp[16];
    int i = 0, j = 0;

    if (value == 0) {
        buffer[j++] = '0';
    } else {
        if (value < 0) {
            buffer[j++] = '-';
            value = -value;
        }
        while (value > 0) {
            temp[i++] = '0' + (value % 10);
            value /= 10;
        }
        while (i > 0) {
            buffer[j++] = temp[--i];
        }
    }
    buffer[j] = '\0';
}

void isr_handler(int interrupt_number) {
    char buf[32] = "Received interrupt: ";
    char num_buf[16];

    // Convert the interrupt number to a string
    int_to_string(interrupt_number, num_buf);

    // Concatenate the interrupt number to the message
    strcat(buf, num_buf);
    strcat(buf, "\n");

    // Write the message to the terminal
    terminal_write(buf);
}