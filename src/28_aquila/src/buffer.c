#include "buffer.h"
#include "song.h"
#include "filesystem.h"
#include "printf.h"
#include "kernel/pit.h"
#include "nano.h"
#include "libc/string.h"

// declare variables for input buffer
char buffer[INPUT_BUFFER_MAX] = { 0};
int input_len = 0;
int input_cursor = 0;
int in_nano = 0; // 0 = aquila, 1 = program mode

extern int input_start; // cursor position for aquila:

// string startswith function
int startsWith(const char *str, const char *prefix) {
    while (*prefix) {
        if (*str++ != *prefix++) {
            return 1; // not a match
        }
    }
    return 0; // match
}


void cmd_clear_screen() {
    printf("Clearing screen...\n");
    sleep_interrupt(1000);
    clear_screen();
    printf("Hello, Aquila!\n");
    // clear the screen
}

void cmd_exit() {
    printf("Exiting...\n");
    sleep_interrupt(1000);
    clear_screen();
    printf("Hello, Aquila!\n");
    // clear the screen
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

void cmd_ls() {
    fs_ls(); 
}

void cmd_save() {
    char filename[MAX_FILE_NAME_SIZE] = "elias.txt";
    char data[MAX_FILE_SIZE] = "dette er en test fil";
    fs_save(filename, data); 
}

void cmd_cat(char *filename) {
    fs_cat(filename);
}

void cmd_nano(char *filename) {
    // if filename is empty, return
    if (filename == NULL) {
        printf("Filename is empty\n");
        return;
    }
    // if filename is too long, return
    if (strlen(filename) > MAX_FILE_NAME_SIZE) {
        printf("Filename is too long\n");
        return;
    }

    in_nano = 1; // set in nano mode
    open_nano(filename); // open nano with filename 
}

void buffer_handler(int action, char ascii) {
    if (in_nano == 1) {
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
            if (input_len < INPUT_BUFFER_MAX - 1) { // leave space for null terminator
                buffer[input_len] = '\n'; // add character to buffer
                input_len++;
                input_cursor++;
            }                
            return; // do nothing if not in program mode
        }
    } else {
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
                } else if (strcmp(buffer, "ls") == 0) {
                    cmd_ls();
                } else if (startsWith(buffer, "cat") == 0) {
                    // cat function with parameter
                    char *filename = buffer + 4; // skip "cat "
                    cmd_cat(filename); // call cat function with filename
                } else if (startsWith(buffer, "nano") == 0) {
                    // cat function with parameter
                    char *raw_filename = buffer + 4;
                    while (*raw_filename == ' ') raw_filename++; // skip extra spaces if any
                    char filename[MAX_FILE_NAME_SIZE];
                    int i = 0;
                    while (raw_filename[i] != '\0' && i < MAX_FILE_NAME_SIZE - 1) {
                        filename[i] = raw_filename[i];
                        i++;
                    }
                    filename[i] = '\0'; // terminate it
                    
                    cmd_nano(filename);
                    return;
                }
                else {
                    printf("Command: %s is not recognized\n", buffer); // print the command
                }
                if (in_nano == 0) {
                    printf("\naquila: ");
                    input_start = cursor; // prevent deletion of "aquila: "
                }
                input_len = 0; // reset buffer length after processing
                input_cursor = 0; // reset cursor position
                for (int i = 0; i < INPUT_BUFFER_MAX; i++) {
                    buffer[i] = 0; // clear the buffer
                }
                break;
            case 5: // clear buffer
                for (int i = 0; i < INPUT_BUFFER_MAX; i++) {
                    buffer[i] = 0; // clear the buffer
                }            
                input_len = 0; // reset buffer length
                input_cursor = 0; // reset cursor position
                break;

        }
    }
}
