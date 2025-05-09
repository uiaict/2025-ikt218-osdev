#include <ui/music_player.h>
#include <libc/stdio.h>
#include <ui/ui_common.h>
#include <kernel/interrupt/pit.h>
#include <driver/include/keyboard.h>

////////////////////////////////////////
// External Declarations
////////////////////////////////////////

extern void terminal_write(const char* str);
extern void play_sound(uint32_t frequency);
extern void stop_sound(void);
extern uint32_t get_current_tick(void);

extern Note music_1[];
extern Note starwars_theme[];
extern Note battlefield_1942_theme[];
extern Note music_2[];
extern Note music_3[];
extern Note music_4[];
extern Note music_5[];
extern Note music_6[];

static MusicPlayer global_music_player;

////////////////////////////////////////
// Utility Functions
////////////////////////////////////////

// Forcefully stop and reset speaker with delay
void reset_pc_speaker(void) {
    stop_sound();
    uint32_t reset_delay = get_current_tick();
    while (get_current_tick() - reset_delay < 100) {
        asm volatile("hlt");
    }
    stop_sound();
}

// Clear keyboard buffer with repeated delay
void clear_all_keyboard_input(void) {
    for (int i = 0; i < 10; i++) {
        while (!keyboard_buffer_empty()) {
            keyboard_buffer_dequeue();
        }
        uint32_t delay = get_current_tick();
        while (get_current_tick() - delay < 10) {
            asm volatile("hlt");
        }
    }
}

////////////////////////////////////////
// Initialization
////////////////////////////////////////

void music_player_init(MusicPlayer* player) {
    player->song_count = 0;
    player->selected_index = 0;
    player->is_playing = false;
    player->running = false;

    music_player_add_song(player, "Super Mario Theme", music_1, 100);
    music_player_add_song(player, "Star Wars Theme", starwars_theme, 100);
    music_player_add_song(player, "Battlefield 1942", battlefield_1942_theme, 100);
    music_player_add_song(player, "Melody 2", music_2, 100);
    music_player_add_song(player, "Melody 3", music_3, 100);
    music_player_add_song(player, "Melody 4", music_4, 100);
    music_player_add_song(player, "Melody 5", music_5, 100);
    music_player_add_song(player, "Imperial March", music_6, 100);
}

void music_player_add_song(MusicPlayer* player, const char* title, Note* notes, uint32_t length) {
    if (player->song_count >= MAX_SONGS) return;

    SongEntry* entry = &player->songs[player->song_count];

    size_t i = 0;
    while (title[i] != '\0' && i < 29) {
        entry->title[i] = title[i];
        i++;
    }
    entry->title[i] = '\0';

    entry->notes = notes;
    entry->length = length;
    player->song_count++;
}

////////////////////////////////////////
// Rendering
////////////////////////////////////////

void music_player_render(MusicPlayer* player) {
    clear_screen();

    terminal_setcursor(30, 1);
    terminal_write("MUSIC PLAYER");

    terminal_setcursor(5, 3);
    if (player->is_playing) {
        printf("Status: Playing \"%s\"", player->songs[player->selected_index].title);
    } else {
        terminal_write("Status: Ready to play");
    }

    terminal_setcursor(5, 5);
    terminal_write("Available Songs:");
    terminal_setcursor(5, 6);
    terminal_write("--------------------------------------------");

    for (size_t i = 0; i < player->song_count; i++) {
        terminal_setcursor(5, 7 + i);

        if (i == player->selected_index) {
            terminal_write("> ");
            uint8_t old_color = terminal_getcolor();
            terminal_setcolor(0x0F);
            printf("%s (%d notes)", player->songs[i].title, player->songs[i].length);
            terminal_setcolor(old_color);
        } else {
            terminal_write("  ");
            printf("%s (%d notes)", player->songs[i].title, player->songs[i].length);
        }
    }

    terminal_setcursor(5, 18);
    terminal_write("Use UP/DOWN arrow keys to navigate");
    terminal_setcursor(5, 19);
    terminal_write("Press ENTER to play the selected song");
    terminal_setcursor(5, 20);
    terminal_write("Press SPACE to stop playback");
    terminal_setcursor(5, 21);
    terminal_write("Press ESC to return to main menu");
}

////////////////////////////////////////
// Playback
////////////////////////////////////////

void music_player_play_selected(MusicPlayer* player) {
    if (player->is_playing || player->selected_index >= player->song_count) return;

    reset_pc_speaker();  // Ensure speaker starts from known state
    player->is_playing = true;

    SongEntry* entry = &player->songs[player->selected_index];
    music_player_render(player);  // Update screen with current status

    for (uint32_t i = 0; i < entry->length; i++) {
        // Optional terminator check (0 Hz + 0 ms = song end)
        if (entry->notes[i].frequency == 0 && entry->notes[i].duration == 0) break;

        terminal_setcursor(5, 22);  // Display info about the current note
        printf("Playing note %d/%d: %d Hz for %d ms             ",
               i + 1, entry->length, entry->notes[i].frequency, entry->notes[i].duration);

        // Play or rest
        if (entry->notes[i].frequency > 0) {
            play_sound(entry->notes[i].frequency);
        } else {
            stop_sound();
        }

        // Wait using PIT ticks and check for input interruption
        uint32_t start_tick = get_current_tick();
        while (get_current_tick() - start_tick < entry->notes[i].duration) {
            if (!keyboard_buffer_empty()) {
                uint8_t key = keyboard_buffer_dequeue();

                // If SPACE or ESC is pressed, stop playback immediately
                if (key == ' ' || key == KEY_ESC) {
                    player->is_playing = false;
                    break;
                }
            }

            asm volatile("hlt");  // Sleep until next interrupt
        }

        // If the user interrupted or the player stopped running, exit loop
        if (!player->is_playing || !player->running) break;

        stop_sound();  // Stop between notes for clarity
    }

    // End-of-song cleanup
    reset_pc_speaker();
    player->is_playing = false;
    clear_all_keyboard_input();  // Remove stray key presses

    music_player_render(player);  // Show updated state

    terminal_setcursor(5, 23);
    terminal_write("Playback complete. Press any key to continue...");
    keyboard_get_key();  // Pause until user confirms
    clear_all_keyboard_input();
}

////////////////////////////////////////
// Input Handling
////////////////////////////////////////

void music_player_handle_input(MusicPlayer* player, uint8_t key) {
    // If music is playing, only allow immediate stop keys
    if (player->is_playing) {
        if (key == ' ' || key == KEY_ESC) {
            player->is_playing = false;
            stop_sound();
            reset_pc_speaker();
        }
        return;
    }

    switch (key) {
        case KEY_UP:
            if (player->selected_index > 0) {
                player->selected_index--;
            }
            break;

        case KEY_DOWN:
            if (player->selected_index < player->song_count - 1) {
                player->selected_index++;
            }
            break;

        case KEY_ENTER:
            music_player_play_selected(player);
            break;

        case ' ':
            // Redundant in non-playing state, but keeps logic consistent
            player->is_playing = false;
            reset_pc_speaker();
            break;

        case KEY_ESC:
            music_player_exit(player);
            break;

        default:
            // Allow quick access via number keys (1–9)
            if (key >= '1' && key <= '9') {
                size_t index = key - '1';
                if (index < player->song_count) {
                    player->selected_index = index;
                }
            }
            break;
    }
}




////////////////////////////////////////
// Main Loop
////////////////////////////////////////

void music_player_run(MusicPlayer* player) {
    player->running = true;
    reset_pc_speaker();  // Ensure no noise from previous sessions

    while (player->running) {
        music_player_render(player);  // Refresh UI
        clear_all_keyboard_input();   // Avoid buffering input across loops

        char key = (char)keyboard_get_key();  // Blocking key read

        // Prevent accidental double presses by debouncing input
        uint32_t delay_start = get_current_tick();
        while (get_current_tick() - delay_start < 10) {
            asm volatile("hlt");
        }

        music_player_handle_input(player, key);
    }

    reset_pc_speaker();  // Always leave the speaker silent
    clear_screen();      // Return to clean terminal
}


void music_player_exit(MusicPlayer* player) {
    player->is_playing = false;
    player->running = false;
    reset_pc_speaker();
    clear_all_keyboard_input();
}

void launch_music_player(void) {
    music_player_init(&global_music_player);
    music_player_run(&global_music_player);
}
