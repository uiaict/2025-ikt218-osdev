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
int in_nano = 0; // 0 = aquila, 1 = program mode (user is in nano)

extern int input_start; // cursor position for aquila:


void cmd_clear_screen() {
    printf("Clearing screen...\n");
    sleep_interrupt(1000);
    clear_screen();
    printf("Hello, Aquila!\n");
}

void cmd_help() {
    printf("Available commands:\n");
    printf("clear - Clear the screen\n");
    printf("help - Show this help message\n");
    printf("test - Test command\n");
    printf("ls - List files\n");
    printf("cat <filename> - Print file content\n");
    printf("rm <filename> - Remove file\n");
    printf("nano <filename> - Open nano editor\n");
}

void cmd_test() {
    printf("Test command!\n");
}

void cmd_ls() {
    fs_ls(); 
}

void cmd_cat(char *filename) {
    fs_cat(filename);
}

void cmd_remove(char *filename) {
    fs_remove(filename);
}

void cmd_nano(char *filename) {
    // if filename empty, return
    if (strlen(filename) <= 0) {
        printf("Filename is empty\n");
        buffer_handler(5, 0); // clear buffer
        printf("aquila: ");
        input_len = 0; 
        input_cursor = 0; 
        input_start = cursor; 
        return;
    }
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
            case 0: // add char to buffer
                if (input_len < INPUT_BUFFER_MAX - 1) { 
                    buffer[input_len] = ascii; // add char to buffer
                    buffer[input_cursor] = ascii;
                    input_len++;
                    input_cursor++;
                }
                break;
            case 1: // remove char from cursor position    
                if (input_cursor <= input_len) {
                    
                    for (int i = input_cursor; i <= input_len; i++) {
                        buffer[i-1] = buffer[i]; // shift char left
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
            if (input_len < INPUT_BUFFER_MAX - 1) { 
                buffer[input_len] = '\n'; // add char to buffer
                input_len++;
                input_cursor++;
            }                
            return; 
        }
    } else {
        switch (action) {
            case 0: // add char to buffer
                if (input_len < INPUT_BUFFER_MAX - 1) { 
                    buffer[input_len] = ascii; // add char to buffer
                    buffer[input_cursor] = ascii; 
                    input_len++;
                    input_cursor++;
                }
                break;
            case 1: // remove char from cursor position

                if (input_cursor <= input_len) {
                    for (int i = input_cursor; i <= input_len; i++) {
                        buffer[i-1] = buffer[i]; // shift char left
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
                    cmd_cat(filename); 
                } else if (startsWith(buffer, "rm") == 0) {
                    // rm function with parameter
                    char *filename = buffer + 3; // skip "rm "
                    cmd_remove(filename); 
                    
                } else if (startsWith(buffer, "nano") == 0) {
                    // nano function with parameter
                    char *raw_filename = buffer + 4;
                    while (*raw_filename == ' ') raw_filename++;
                    char filename[MAX_FILE_NAME_SIZE];
                    int i = 0;
                    while (raw_filename[i] != '\0' && i < MAX_FILE_NAME_SIZE - 1) {
                        filename[i] = raw_filename[i];
                        i++;
                    }
                    filename[i] = '\0';
                    
                    cmd_nano(filename);
                    return;
                }
                else {
                    printf("Command: %s is not recognized\n", buffer); // print the command
                }
                if (in_nano == 0) {
                    printf("\naquila: ");
                    input_start = cursor; // prevent delete of "aquila: "
                }
                input_len = 0; // reset buffer
                input_cursor = 0;
                for (int i = 0; i < INPUT_BUFFER_MAX; i++) {
                    buffer[i] = 0;
                }
                break;
            case 5: // reset buffer
                for (int i = 0; i < INPUT_BUFFER_MAX; i++) {
                    buffer[i] = 0; 
                }            
                input_len = 0;
                input_cursor = 0; 
                break;

        }
    }
}
