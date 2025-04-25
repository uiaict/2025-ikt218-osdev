#include "menu.h"
#include "display.h"
#include "interruptHandler.h"
#include "musicPlayer.h"
#include "pcSpeaker.h"
#include "programmableIntervalTimer.h"
#include "testFuncs.h"
#include "miscFuncs.h"
#include "storage.h"  // For harddisk testing functions

// QEMU/Bochs shutdown port
#define QEMU_EXIT_PORT 0x604
#define QEMU_EXIT_CODE 0x2000

// Function to shutdown the system
static inline void shutdown_system(void) {
    display_write_color("\nShutting down system...\n", COLOR_YELLOW);
    
    // Gradual visual shutdown effect
    for (size_t i = 0; i < 25; i++) {
        display_write_char('\n');
        sleep_interrupt(50);
    }
    
    display_clear();
    display_write_color("\n\n\n         SYSTEM HALTED\n\n\n", COLOR_RED);
    
    // Send shutdown command to QEMU/Bochs
    outw(QEMU_EXIT_PORT, QEMU_EXIT_CODE);
    
    // If we get here, the shutdown didn't work, halt the CPU
    disable_interrupts();
    halt();
}

// Shows the main menu
void show_menu(void) {
    // Clear screen before showing menu
    display_clear();
    
    // Simple ASCII header
    display_write_color("\n", COLOR_WHITE);
    display_write_color("                  SWEATER OS\n", COLOR_CYAN);
    display_write_color("                  ==========\n\n", COLOR_CYAN);
    
    // Menu items - simple and clean
    display_write_color("  ", COLOR_WHITE);
    display_write_color("1", COLOR_LIGHT_GREEN);
    display_write_color(". Run System Tests\n", COLOR_WHITE);
    
    display_write_color("  ", COLOR_WHITE);
    display_write_color("2", COLOR_LIGHT_GREEN);
    display_write_color(". Music Player\n", COLOR_WHITE);
    
    display_write_color("  ", COLOR_WHITE);
    display_write_color("3", COLOR_LIGHT_GREEN);
    display_write_color(". Piano Keyboard\n", COLOR_WHITE);
    
    display_write_color("  ", COLOR_WHITE);
    display_write_color("4", COLOR_LIGHT_GREEN);
    display_write_color(". Snake Game\n", COLOR_WHITE);
    
    display_write_color("  ", COLOR_WHITE);
    display_write_color("5", COLOR_LIGHT_GREEN);
    display_write_color(". Shut Down\n", COLOR_WHITE);
    
    display_write_color("\nSelect an option (1-5): ", COLOR_LIGHT_CYAN);
}

// Shows the music menu
void show_music_menu(void) {
    // Clear screen before showing menu
    display_clear();
    
    // Simple ASCII header
    display_write_color("\n", COLOR_WHITE);
    display_write_color("                  MUSIC PLAYER\n", COLOR_CYAN);
    display_write_color("                  ============\n\n", COLOR_CYAN);
    
    // Menu items - simple and clean
    display_write_color("  ", COLOR_WHITE);
    display_write_color("1", COLOR_LIGHT_GREEN);
    display_write_color(". Play \"Hot Cross Buns\"\n", COLOR_WHITE);
    
    display_write_color("  ", COLOR_WHITE);
    display_write_color("2", COLOR_LIGHT_GREEN);
    display_write_color(". Play \"Mary Had a Little Lamb\"\n", COLOR_WHITE);
    
    display_write_color("  ", COLOR_WHITE);
    display_write_color("3", COLOR_LIGHT_GREEN);
    display_write_color(". Play \"Happy Birthday\"\n", COLOR_WHITE);
    
    display_write_color("  ", COLOR_WHITE);
    display_write_color("4", COLOR_LIGHT_GREEN);
    display_write_color(". Play \"Twinkle Twinkle Little Star\"\n", COLOR_WHITE);
    
    display_write_color("  ", COLOR_WHITE);
    display_write_color("5", COLOR_LIGHT_GREEN);
    display_write_color(". Return to Main Menu\n", COLOR_WHITE);
    
    display_write_color("\nSelect an option (1-5): ", COLOR_LIGHT_CYAN);
}

// Shows the piano menu
void show_piano_menu(void) {
    // Clear screen before showing menu
    display_clear();
    
    // Simple ASCII header
    display_write_color("\n", COLOR_WHITE);
    display_write_color("                  PIANO KEYBOARD\n", COLOR_CYAN);
    display_write_color("                  ==============\n\n", COLOR_CYAN);
    
    display_write_color("Use the following keys to play notes:\n\n", COLOR_WHITE);
    
    display_write_color("  ", COLOR_WHITE);
    display_write_color("1", COLOR_LIGHT_GREEN);
    display_write_color(" - C4    ", COLOR_WHITE);
    display_write_color("2", COLOR_LIGHT_GREEN);
    display_write_color(" - D4    ", COLOR_WHITE);
    display_write_color("3", COLOR_LIGHT_GREEN);
    display_write_color(" - E4\n", COLOR_WHITE);
    
    display_write_color("  ", COLOR_WHITE);
    display_write_color("4", COLOR_LIGHT_GREEN);
    display_write_color(" - F4    ", COLOR_WHITE);
    display_write_color("5", COLOR_LIGHT_GREEN);
    display_write_color(" - G4    ", COLOR_WHITE);
    display_write_color("6", COLOR_LIGHT_GREEN);
    display_write_color(" - A4\n", COLOR_WHITE);
    
    display_write_color("  ", COLOR_WHITE);
    display_write_color("7", COLOR_LIGHT_GREEN);
    display_write_color(" - B4    ", COLOR_WHITE);
    display_write_color("8", COLOR_LIGHT_GREEN);
    display_write_color(" - C5    ", COLOR_WHITE);
    display_write_color("9", COLOR_LIGHT_GREEN);
    display_write_color(" - D5\n", COLOR_WHITE);
    
    display_write_color("\nPress ", COLOR_WHITE);
    display_write_color("ESC", COLOR_LIGHT_RED);
    display_write_color(" to return to the main menu\n", COLOR_WHITE);
}

// Play a selected melody
void play_melody(Note* melody, uint32_t length) {
    Song song = {
        .notes = melody,
        .length = length
    };
    
    SongPlayer* player = create_song_player();
    if (player) {
        display_write("\nPlaying melody...\n");
        player->play_song(&song);
        free_song_player(player);
    } else {
        display_write("\nError: Could not create song player\n");
    }
}

// Handles music player menu
void handle_music_menu(void) {
    bool in_music_menu = true;

    while (in_music_menu) {
        show_music_menu();

        // Wait for valid input
        char key = 0;
        while (!keyboard_data_available()) {
            __asm__ volatile("hlt");
        }

        key = keyboard_getchar();
        display_write_char(key);
        display_write_char('\n');

        switch (key) {
            case '1':
                play_melody(mario_melody, sizeof(mario_melody) / sizeof(mario_melody[0]));
                break;
            case '2':
                play_melody(twinkle_melody, sizeof(twinkle_melody) / sizeof(twinkle_melody[0]));
                break;
            case '3':
                play_melody(jingle_bells, sizeof(jingle_bells) / sizeof(jingle_bells[0]));
                break;
            case '4':
                play_melody(imperial_march, sizeof(imperial_march) / sizeof(imperial_march[0]));
                break;
            case '5':
                in_music_menu = false;
                break;
            default:
                display_write("\nInvalid choice. Please try again.\n");
                sleep_interrupt(500);  // Use sleep_interrupt instead of delay
        }

        if (key >= '1' && key <= '4') {
            display_write("\nPress any key to return to music menu...\n");
            while (!keyboard_data_available()) {
                __asm__ volatile("hlt");
            }
            keyboard_getchar(); // Clear the keypress
        }
    }
}

// Handles piano keyboard input and plays notes
void handle_piano_keyboard(void) {
    show_piano_menu();

    // Flag to track if a key is currently pressed
    bool key_pressed = false;
    char current_key = 0;
    
    // Enable interrupts for keyboard input
    __asm__ volatile("sti");
    
    // Clear any pending keyboard input
    while (keyboard_data_available()) {
        keyboard_getchar();
    }

    // Disable speaker initially to make sure it's off
    disable_speaker();
    
    bool running = true;
    uint32_t last_check_time = get_current_tick();
    
    // For tracking key release timing
    uint32_t key_press_time = 0;
    const uint32_t KEY_HOLD_TIMEOUT = 500; // Consider key released after 500ms
    
    while (running) {
        // Get current time for periodic checks
        uint32_t current_time = get_current_tick();
        
        // Process keyboard input
        if (keyboard_data_available()) {
            char key = keyboard_getchar();
            
            if (key == 27) { // ESC key
                running = false;
                break;
            }
            
            // Map keys to frequencies
            uint32_t frequency = 0;
            
            // Check if this is a new key (not already pressed)
            if (key == current_key) {
                // This is the same key being held down, update press time
                key_press_time = current_time;
                continue;
            }
            
            // Main piano keys on the middle row
            switch (key) {
                case 'a': frequency = C4; break;
                case 's': frequency = D4; break;
                case 'd': frequency = E4; break;
                case 'f': frequency = F4; break;
                case 'g': frequency = G4; break;
                case 'h': frequency = A4; break;
                case 'j': frequency = B4; break;
                case 'k': frequency = C5; break;
                
                // Sharp/flat notes on the top row
                case 'w': frequency = Cs4; break;
                case 'e': frequency = Ds4; break;
                case 't': frequency = Fs4; break;
                case 'y': frequency = Gs4; break;
                case 'u': frequency = As4; break;
                
                // Octave change
                case 'z': frequency = C3; break;
                case 'x': frequency = D3; break;
                case 'c': frequency = E3; break;
                case 'v': frequency = F3; break;
                case 'b': frequency = G3; break;
                case 'n': frequency = A3; break;
                case 'm': frequency = B3; break;
                case ',': frequency = C4; break;
                
                default: frequency = 0; break;
            }
            
            // Stop any previous note if a new one is played
            if (key_pressed && frequency > 0) {
                disable_speaker();
            }
            
            // Play note with direct speaker access for lowest latency
            if (frequency > 0) {
                // Update state for key press
                key_pressed = true;
                current_key = key;
                key_press_time = current_time; // Record when this key was pressed
                
                // Calculate divisor directly
                uint16_t divisor = 1193180 / frequency;
                
                // Disable interrupts while configuring PIT
                __asm__ volatile("cli");
                
                // Configure PIT channel 2
                outb(0x43, 0xB6);
                outb(0x42, divisor & 0xFF);
                outb(0x42, (divisor >> 8) & 0xFF);
                
                // Enable speaker
                outb(0x61, inb(0x61) | 0x03);
                
                // Re-enable interrupts
                __asm__ volatile("sti");
                
                // Display a visual indicator that a note is playing
                display_set_cursor(40, 15);
                display_write_color("♫ Playing: ", COLOR_LIGHT_GREEN);
                
                // Display the note name
                char* note_name = "Unknown";
                if (frequency == C3 || frequency == C4 || frequency == C5) note_name = "C";
                else if (frequency == Cs3 || frequency == Cs4) note_name = "C#";
                else if (frequency == D3 || frequency == D4) note_name = "D";
                else if (frequency == Ds3 || frequency == Ds4) note_name = "D#";
                else if (frequency == E3 || frequency == E4) note_name = "E";
                else if (frequency == F3 || frequency == F4) note_name = "F";
                else if (frequency == Fs3 || frequency == Fs4) note_name = "F#";
                else if (frequency == G3 || frequency == G4) note_name = "G";
                else if (frequency == Gs3 || frequency == Gs4) note_name = "G#";
                else if (frequency == A3 || frequency == A4) note_name = "A";
                else if (frequency == As3 || frequency == As4) note_name = "A#";
                else if (frequency == B3 || frequency == B4) note_name = "B";
                
                display_write_color(note_name, COLOR_LIGHT_CYAN);
                
                // Show which octave
                if (frequency >= C3 && frequency <= B3) {
                    display_write_color("3", COLOR_LIGHT_CYAN);
                } else if (frequency >= C4 && frequency <= B4) {
                    display_write_color("4", COLOR_LIGHT_CYAN);
                } else if (frequency >= C5) {
                    display_write_color("5", COLOR_LIGHT_CYAN);
                }
                
                display_write_color(" ♫", COLOR_LIGHT_GREEN);
            }
        }
        
        // Check for key release based on timing
        if (key_pressed && (current_time - key_press_time >= KEY_HOLD_TIMEOUT)) {
            // No keypress received for a while, assume key was released
            disable_speaker();
            key_pressed = false;
            current_key = 0;
            
            // Clear the note display
            display_set_cursor(40, 15);
            display_write_color("                     ", COLOR_BLACK);
        }
        
        // Small sleep to reduce CPU usage but stay responsive
        sleep_interrupt(1);
    }
    
    // Ensure speaker is off when exiting
    disable_speaker();
    
    // Clear keyboard buffer
    while (keyboard_data_available()) {
        keyboard_getchar();
    }
    
    display_clear();
}

// Handles the user's menu choice
void handle_menu_choice(char choice) {
    switch(choice) {
        case '1':
            display_write("\nRunning system tests...\n");
            run_all_tests();
            display_write("\nPress any key to return to menu...\n");
            while (!keyboard_data_available()) {
                __asm__ volatile("hlt");
            }
            keyboard_getchar();
            break;
        case '2':
            handle_music_menu();
            break;
        case '3':
            handle_piano_keyboard();
            break;
        case '4':
            handle_snake_game();
            break;
        case '5':
            shutdown_system();
            break;
        default:
            display_write("\nInvalid choice. Please try again.\n");
            sleep_interrupt(500);  // Use sleep_interrupt instead of delay
    }
}

// Main menu loop
void run_menu_loop(void) {
    char key;
    bool redraw = true;

    while(1) {
        if (redraw) {
            show_menu();
            redraw = false;
            
            // Clear any pending keystrokes after redrawing menu
            while (keyboard_data_available()) {
                keyboard_getchar();
            }
        }

        // Wait for keypress using hlt instruction
        while (!keyboard_data_available()) {
            __asm__ volatile("hlt");
        }

        // Read keypress
        key = keyboard_getchar();
        
        // Consume any additional keystrokes to prevent buffering
        while (keyboard_data_available()) {
            keyboard_getchar();
        }
        
        if (key != 0) {  // Ignore null characters
            display_write_char(key);
            display_write_char('\n');
            handle_menu_choice(key);
            redraw = true;  // Redraw menu after choice
        }
    }
} 