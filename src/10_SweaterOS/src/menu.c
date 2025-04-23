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
    display_write_color(". Shut Down\n", COLOR_WHITE);
    
    display_write_color("\nSelect an option (1-4): ", COLOR_LIGHT_CYAN);
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

// Håndterer piano keyboard input og spiller noter
void handle_piano_keyboard(void) {
    show_piano_menu();
    
    display_write_color("\nPlaying in silent mode - no note display for minimum latency\n", COLOR_YELLOW);
    
    // Tøm keyboard buffer før vi starter
    while (keyboard_data_available()) {
        keyboard_getchar();
    }
    
    // Disable interrupts for direct keyboard access
    __asm__ volatile("cli");
    
    bool running = true;
    // Track which key is currently playing
    int current_key = -1;
    // Track the last scancode processed to avoid repeat processing
    uint8_t last_scancode = 0;
    bool last_was_keyup = true;
    
    while (running) {
        // Les direkte fra keyboard porten for bedre responsivitet
        if ((inb(KEYBOARD_STATUS_PORT) & 0x01)) {
            uint8_t scancode = inb(KEYBOARD_DATA_PORT);
            bool key_released = (scancode & 0x80);
            uint8_t clean_scancode = scancode & ~0x80;  // Fjern release bit
            
            // If this is the same scancode as last time (key held down)
            // and it's not a key release event, skip it to avoid repeats
            if (clean_scancode == last_scancode && !key_released && !last_was_keyup) {
                continue;
            }
            
            // Update tracking variables
            last_scancode = clean_scancode;
            last_was_keyup = key_released;
            
            // Håndter ESC-tasten spesielt
            if (clean_scancode == 0x01) {  // ESC scancode
                if (!key_released) {
                    running = false;
                    stop_sound();
                    break;
                }
                continue;
            }
            
            // Konverter scancode til piano-tast-indeks (1-9 for number keys)
            int key_index = -1;
            if (clean_scancode >= 0x02 && clean_scancode <= 0x0A) {  // Taster 1-9
                key_index = clean_scancode - 0x02;
            }
            
            if (key_index != -1) {
                if (key_released) {
                    // Key was released
                    if (key_index == current_key) {
                        stop_sound();
                        current_key = -1;
                    }
                } else {
                    // Tasten ble nettopp trykket ned
                    current_key = key_index;
                    
                    // Stop any previous sound immediately
                    stop_sound();
                    
                    // Bestem hvilken note som skal spilles
                    uint32_t frequency = 0;
                    
                    switch (key_index) {
                        case 0: frequency = C4; break;
                        case 1: frequency = D4; break;
                        case 2: frequency = E4; break;
                        case 3: frequency = F4; break;
                        case 4: frequency = G4; break;
                        case 5: frequency = A4; break;
                        case 6: frequency = B4; break;
                        case 7: frequency = C5; break;
                        case 8: frequency = D5; break;
                    }
                    
                    // IMPORTANT: Play the note FIRST, before any other operations
                    // This dramatically reduces latency
                    enable_speaker();
                    play_sound(frequency);
                    
                    // No note display to minimize latency
                }
            }
        }
        
        // Reduce delay to improve responsiveness 
        for (volatile int i = 0; i < 100; i++) {
            __asm__ volatile("nop");
        }
    }
    
    // Stop any playing sounds and disable the speaker
    stop_sound();
    disable_speaker();
    
    // Re-enable interrupts before returning
    __asm__ volatile("sti");
    
    // Tøm keyboard buffer før vi går ut
    while (keyboard_data_available()) {
        keyboard_getchar();
    }
    
    display_write_color("\nExiting piano mode...\n", COLOR_YELLOW);
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