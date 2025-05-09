#include "keyboard.h"
#include "terminal.h"
#include "shell.h"
#include "common.h"
#include "song_player.h"
#include "system.h"
#include "snake.h"
#include "memory.h"
#include "pit.h"
#define MAX_COMMAND_LENGTH 128

// Forward declarations of functions
void display_help();
void play_music();
void kernel_sleep_test();
void assignment_4_testing();

// Command structure
typedef struct {
    const char* command_name;
    void (*command_function)();
} Command;

// List of available commands
Command commands[] = {
    {"help", display_help},
    {"play music", play_music},
    {"clear", terminal_initialize},
    {"test_sleep", kernel_sleep_test},
    {"test_malloc", assignment_4_testing},
    {"snake", play_snake}
};

#define NUM_COMMANDS (sizeof(commands) / sizeof(commands[0]))

// Display the available commands
void display_help() {
    printf("Available commands:\n");
    for (int i = 0; i < NUM_COMMANDS; i++) {
        printf("- %s\n", commands[i].command_name);
    }
}

// Read a line of input from the user
void read_line(char* buffer, uint32_t max_length) {
    uint32_t index = 0;

    while (1) {
        char c = keyboard_getchar();
    
        // Handle Enter key (newline)
        if (c == '\n') {
            buffer[index] = '\0'; // Null-terminate the buffer
            printf("\n"); // Print a newline after Enter is pressed
            return;
        }
        // Handle Backspace
        else if (c == '\b') {
            if (index > 0) {
                index--; // Move the index back
                terminal_write("\b \b"); // Move the cursor back, then overwrite with space
            }
        }
        // Handle regular characters
        else {
            if (index < max_length - 1) {
                buffer[index++] = c; // Store character in the buffer

                // Print the character that was entered
                char str[2] = {c, '\0'}; // Create a string of length 1
                printf("%s", str); // Print the character
            }
        }
    }
}

// Execute the command based on user input
static void execute_command(const char* input) {
    for (int i = 0; i < NUM_COMMANDS; i++) {
        if (strcmp(input, commands[i].command_name) == 0) {
            commands[i].command_function(); // Call the associated function
            return;
        }
    }
    // If the command is not found, print an error message
    printf("Unknown command. Type 'help' for a list of commands.\n");
}

// Kernel sleep test function
void kernel_sleep_test() {
    int counter = 0;
    while (1) {
        printf("[%d]: Sleeping with busy-waiting (HIGH CPU).\n", counter);
        sleep_busy(1000); // High CPU usage (busy-wait)
        printf("[%d]: Slept using busy-waiting.\n", counter++);

        printf("[%d]: Sleeping with interrupts (LOW CPU).\n", counter);
        sleep_interrupt(1000); // Low CPU usage (interrupt-driven)
        printf("[%d]: Slept using interrupts.\n", counter++);
    };
}

// Assignment 4 test function: testing malloc and kernel sleep
void assignment_4_testing() {
    void* a = malloc(64);
    void* b = malloc(128);
    void* c = malloc(256);
    
    printf("Allocated a at address: 0x%x\n", (uint32_t)a);
    printf("Allocated b at address: 0x%x\n", (uint32_t)b);
    printf("Allocated c at address: 0x%x\n", (uint32_t)c);
    
    printf("Memory allocated!\n");
    
    kernel_sleep_test(); // Run the sleep test after memory allocation
}

// Main shell loop
void shell() {
    printf("Welcome to this operating system!\n");

    while (1) {
        printf("> ");

        char input[MAX_COMMAND_LENGTH] = {0};
        read_line(input, MAX_COMMAND_LENGTH);

        execute_command(input);
    }
}
