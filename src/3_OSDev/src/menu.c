#include "menu.h"
#include "libc/stdio.h"
#include "libc/stdint.h"
#include "vga.h"
#include "memory/memory.h"
#include "games/snakes/snakes.h"
#include "music_player/song.h"
#include "interrupts.h"

#define option_1_location 
#define option_2_location
#define option_3_location

bool menu_active = true;
bool snakes_active = false;
int selected_option = 1;

void play_music() {
    Song songs[] = {
        {rick_roll, sizeof(rick_roll) / sizeof(Note)},
        {star_wars_theme, sizeof(star_wars_theme) / sizeof(Note)}
    };
    uint32_t n_songs = sizeof(songs) / sizeof(Song);

    SongPlayer* player = create_song_player();

    for(uint32_t i = 0; i < n_songs; i++) {
        printf(0x0F, "Playing song...\n");
        player->play_song(&songs[i]);
        printf(0x0F, "Finished playing song.\n");
    }
    display_menu();
}

void display_menu_text(void) {
    Reset();
    printf(0x0B, "   ____   _____ _____             ____  \n");
    sleep_busy(100);
    printf(0x0B, "  / __ \\ / ____|  __ \\           |___ \\ \n");
    sleep_busy(100);
    printf(0x0B, " | |  | | (___ | |  | | _____   ____) | \n");
    sleep_busy(100);
    printf(0x0B, " | |  | |\\___ \\| |  | |/ _ \\ \\ / /__ < \n");
    sleep_busy(100);
    printf(0x0B, " | |__| |____) | |__| |  __/\\ V /___) | \n");
    sleep_busy(100);
    printf(0x0B, "  \\____/|_____/|_____/ \\___| \\_/|____/ \n");
    sleep_busy(100);
    printf(0x0B, "                                       \n");
    sleep_busy(100);
    printf(0x0F, "      Operating System Development     \n");
    sleep_busy(100);
    printf(0x07, "     UiA IKT218 Course Project Team 3  \n");
    sleep_busy(100);
    printf(0x07, "=======================================\n");
    sleep_busy(100);
    printf(0x0F, " 1. Music\n");
    sleep_busy(100);
    printf(0x0F, " 2. Game\n");
    sleep_busy(100);
    printf(0x0F, " 3. Print Memory Layout\n");
    sleep_busy(100);
    printf(0x0F, " 4. Terminal\n");
    sleep_busy(100);
    printf(0x0F, "=======================================\n");
    sleep_busy(100);
    printf(0x0F, " UP: W | DOWN: S | SELECT: Enter | BACK TO MENU: Esc\n");
    printf(0x0F, "\n");
}

void display_menu(void) {
    Reset();
    enable_interrupts();
    display_menu_text();
    selected_option = 1;
    menu_active = true;
    snakes_active = false;
    highlight_selected_option(selected_option);
}

void select_menu_option(uint8_t option) {
    switch (option) {
        case 1:
            // Music
            Reset();
            enable_interrupts();
            printf(0x0E, "Playing music...\n");
            play_music();
            break;
        case 2:
            // Game
            menu_active = 0;
            Reset();
            start_snake_game();
            break;
        case 3:
            // Print Memory Layout
            Reset();
            print_memory_layout();
            break; 
        case 4:
            Reset();
            printf(0x0E, "Warning: ");
            printf(0x0F, "No commands do anything as it has not been created.\n");
            printf(0x0F, "At this point in time. This is just a fancy note pad!\n");
            break;
    }
}
void highlight_selected_option(uint8_t option) {
    switch (option) {
        case 1:
            set_cursor_position(0, 10);
            printf(0x0E, " 1. Music\n");
            printf(0x0F, " 2. Game\n");
            printf(0x0F, " 3. Print Memory Layout\n");
            printf(0x0F, " 4. Terminal\n");
            set_cursor_position(-1, -1);
            break;
        case 2:
            set_cursor_position(0, 10);
            printf(0x0F, " 1. Music\n");
            printf(0x0E, " 2. Game\n");
            printf(0x0F, " 3. Print Memory Layout\n");
            printf(0x0F, " 4. Terminal\n");
            set_cursor_position(-1, -1);
            break;
        case 3:
            set_cursor_position(0, 10);
            printf(0x0F, " 1. Music\n");
            printf(0x0F, " 2. Game\n");
            printf(0x0E, " 3. Print Memory Layout\n");
            printf(0x0F, " 4. Terminal\n");
            set_cursor_position(-1, -1);
            break;
        case 4:
            set_cursor_position(0, 10);
            printf(0x0F, " 1. Music\n");
            printf(0x0F, " 2. Game\n");
            printf(0x0F, " 3. Print Memory Layout\n");
            printf(0x0E, " 4. Terminal\n");
            set_cursor_position(-1, -1);
            break;
    }
}

void handle_menu_input(char ascii_char) {
    if (ascii_char == 27) {
        // Escape key pressed
        display_menu();
        return;
    }

    if (ascii_char == 'w') {
        // Move up
        if (selected_option > 1) {
            selected_option--;
            highlight_selected_option(selected_option);
        }
    }
    else if (ascii_char == 's') {
        // Move down
        if (selected_option < 4) {
            selected_option++;
            highlight_selected_option(selected_option);
        }
    }
    else if (ascii_char == '\n') {
        // Select option
        select_menu_option(selected_option);
    }
}