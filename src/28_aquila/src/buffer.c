#include "buffer.h"

// declare variables for input buffer
char buffer[INPUT_BUFFER_MAX] = { 0};
int input_len = 0;
int input_cursor = 0;

void buffer_handler(int action, char ascii) {
    switch (action) {
        case 0: // add character to buffer
            if (input_len < INPUT_BUFFER_MAX - 1) { // leave space for null terminator
                buffer[input_len] = ascii; // add character to buffer
                buffer[input_cursor] = ascii; // insert character
                input_len++;
                input_cursor++;
            }
            break;
        case 1: // remove character from cursor position
            if (input_cursor <= input_len) {
                for (int i = input_cursor; i <= input_len; i++) {
                    buffer[i-1] = buffer[i]; // shift characters left
                }
                input_len--;
                input_cursor--;
            }
            break;
        case 2: // cursor left
            if (input_cursor >= 0) {
                input_cursor--;
            }
            break;
        case 3: // cursor right
            if (input_cursor < input_len) {
                input_cursor++;
            }
            break;
        case 4: // enter key pressed
            buffer[input_len+1] = '\0'; // null-terminate the string
            if (strcmp(buffer, "") == 0) {
            } else if (strcmp(buffer, "exit") == 0) {
                cmd_exit();
            } else if (strcmp(buffer, "help") == 0) {
                cmd_help();
            } else if (strcmp(buffer, "test") == 0) {
                cmd_test();
            } else if (strcmp(buffer, "clear") == 0) {
                cmd_clear_screen();
            }
            else {
                printf("Command: %s is not recognized\n", buffer); // print the command
            }
            printf("\naquila: ");
            input_len = 0; // reset buffer length after processing
            input_cursor = 0; // reset cursor position
            for (int i = 0; i < INPUT_BUFFER_MAX; i++) {
                buffer[i] = 0; // clear the buffer
            }
            break;

    }
}

void cmd_clear_screen() {
    printf("Clearing screen...\n");
    sleep_interrupt(1000);
    clear_screen(); // clear the screen
}

void cmd_exit() {
    printf("Exiting...\n");
    sleep_interrupt(1000);
    clear_screen(); // clear the screen
}

void cmd_help() {
    printf("Available commands:\n");
    printf("clear - Clear the screen\n");
    printf("exit - Exit the program\n");
    printf("help - Show this help message\n");
}

void cmd_test() {
    printf("Test command!\n");
}