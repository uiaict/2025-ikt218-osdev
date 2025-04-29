#include "menu.h"
#include "display.h"
#include "interruptHandler.h"
#include "musicPlayer.h"
#include "pcSpeaker.h"
#include "programmableIntervalTimer.h"
#include "testFuncs.h"
#include "miscFuncs.h"
#include "storage.h"  // For harddisk testing functions
#include "snake.h"
#include "piano.h"

// QEMU/Bochs exit port
#define QEMU_EXIT_PORT 0x604
#define QEMU_EXIT_CODE 0x2000

// Funksjon for å slå av systemet
static inline void shutdown_system(void) {
    display_write_color("\nShutting down system...\n", COLOR_YELLOW);
    
    // Avslutter systemet
    for (size_t i = 0; i < 25; i++) {
        display_write_char('\n');
        sleep_interrupt(50);
    }
    
    display_clear();
    display_write_color("\n\n\n         SYSTEM STOPPED\n\n\n", COLOR_RED);
    
    // Send avslutningskommando til QEMU/Bochs
    outw(QEMU_EXIT_PORT, QEMU_EXIT_CODE);
    
    // Hvis vi kommer hit fungerte ikke avslutningen, stopp CPU-en
    disable_interrupts();
    halt();
}

// Viser hovedmenyen
void show_menu(void) {
    // Tøm skjermen før menyen vises
    display_clear();
    
    // Enkel ASCII header
    display_write_color("\n", COLOR_WHITE);
    display_write_color("                  SWEATER OS\n", COLOR_CYAN);
    display_write_color("                  ==========\n\n", COLOR_CYAN);
    
    // Menyelementer - enkle og rene
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
    display_write_color(". Shutdown\n", COLOR_WHITE);
    
    display_write_color("\nSelect an option (1-5): ", COLOR_LIGHT_CYAN);
}

// Viser musikkmenyen
void show_music_menu(void) {
    display_clear();
    
    display_write_color("\n", COLOR_WHITE);
    display_write_color("                  MUSIC PLAYER\n", COLOR_CYAN);
    display_write_color("                  ============\n\n", COLOR_CYAN);
    
    display_write_color("  ", COLOR_WHITE);
    display_write_color("1", COLOR_LIGHT_GREEN);
    display_write_color(". Super Mario Theme\n", COLOR_WHITE);
    
    display_write_color("  ", COLOR_WHITE);
    display_write_color("2", COLOR_LIGHT_GREEN);
    display_write_color(". Ode to Joy\n", COLOR_WHITE);
    
    display_write_color("  ", COLOR_WHITE);
    display_write_color("3", COLOR_LIGHT_GREEN);
    display_write_color(". Frère Jacques\n", COLOR_WHITE);
    
    display_write_color("  ", COLOR_WHITE);
    display_write_color("4", COLOR_LIGHT_GREEN);
    display_write_color(". Back to Main Menu\n", COLOR_WHITE);
    
    display_write_color("\nSelect an option (1-4): ", COLOR_LIGHT_CYAN);
}

// Spiller en valgt melodi
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

// Håndterer musikkmeny
void handle_music_menu(void) {
    bool in_music_menu = true;

    while (in_music_menu) {
        show_music_menu();

        char key = 0;
        while (!keyboard_data_available()) {
            __asm__ volatile("hlt");
        }

        key = keyboard_getchar();
        display_write_char(key);
        display_write_char('\n');

        switch (key) {
            case '1':
                play_melody(music_1, sizeof(music_1) / sizeof(music_1[0]));
                break;
            case '2':
                play_melody(music_3, sizeof(music_3) / sizeof(music_3[0]));
                break;
            case '3':
                play_melody(music_4, sizeof(music_4) / sizeof(music_4[0]));
                break;
            case '4':
                in_music_menu = false;
                break;
            default:
                display_write("\nInvalid choice\n");
                sleep_interrupt(500);
        }

        if (key >= '1' && key <= '3') {
            display_write("\nPress any key to continue...\n");
            while (!keyboard_data_available()) {
                __asm__ volatile("hlt");
            }
            keyboard_getchar();
        }
    }
}

// Håndterer menyvalg
void handle_menu_choice(char choice) {
    switch (choice) {
        case '1':
            display_clear();
            display_write_color("\nRunning system tests...\n\n", COLOR_YELLOW);
            run_all_tests();
            display_write_color("\nPress any key to continue...\n", COLOR_LIGHT_CYAN);
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
            display_clear();
            snake_game();
            break;
            
        case '5':
            shutdown_system();
            break;
            
        default:
            display_write_color("\nInvalid choice. Try again.\n", COLOR_LIGHT_RED);
            sleep_interrupt(500);
    }
}

// Kjører hovedmenyløkka
void run_menu_loop(void) {
    while (1) {
        show_menu();
        
        char key = 0;
        while (!keyboard_data_available()) {
            __asm__ volatile("hlt");
        }
        
        key = keyboard_getchar();
        display_write_char(key);
        display_write_char('\n');
        
        handle_menu_choice(key);
    }
} 