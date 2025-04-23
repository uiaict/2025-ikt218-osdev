#include "command.h"
#include "terminal.h"
#include "snake_game.h"
#include "song.h"
#include "song_player.h"
#include <libc/string.h>

// Command buffer
char command_buffer[MAX_COMMAND_LENGTH];
int command_length = 0;

// Initialize the command buffer
void init_command_buffer() {
    clear_command_buffer();
}

// Clear the command buffer
void clear_command_buffer() {
    for (int i = 0; i < MAX_COMMAND_LENGTH; i++) {
        command_buffer[i] = 0;
    }
    command_length = 0;
}

// Append a character to the command buffer
void append_to_command(char c) {
    if (command_length < MAX_COMMAND_LENGTH - 1) {
        command_buffer[command_length++] = c;
        command_buffer[command_length] = 0; // Null terminate
    }
}

// Execute the current command
void execute_current_command() {
    if (command_length > 0) {
        process_command(command_buffer);
        clear_command_buffer();
    }
    
    // Print a new prompt
    terminal_putchar('\n');
    writeline("daemon-duo> ");
}

// Process a command
void process_command(const char* cmd) {
    // Simple string comparison for commands
    if (strcmp(cmd, "help") == 0) {
        writeline("\nAvailable commands:\n");
        writeline("  help   - Display this help message\n");
        writeline("  clear  - Clear the screen\n");
        writeline("  snake  - Play Snake game\n");
        writeline("  music  - Play Mario theme song\n");
    }
    else if (strcmp(cmd, "clear") == 0) {
        terminal_clear();
    }
    else if (strcmp(cmd, "snake") == 0) {
        // Ensure interrupts are enabled
        __asm__ __volatile__("sti");
        
        // Reset the PIT
        reset_pit_timer();
        
        // Make sure timer and keyboard IRQs are enabled
        enable_irq(0);
        enable_irq(1);
        
        // Launch the snake game
        start_snake_game();
    }
    else if (strcmp(cmd, "music") == 0) {
        writeline("\nPlaying Mario theme song...\n");
        
        // Make sure we can handle keyboard interrupts during music playback
        enable_irq(1);
        
        // Play the song
        play_song(mario_theme);
        
        // Explicitly reset keyboard functionality
        enable_irq(1);
        
        writeline("\nMusic playback complete.\n");
    }
    else {
        writeline("\nUnknown command: ");
        writeline(cmd);
        writeline("\nType 'help' for a list of commands\n");
    }
}
