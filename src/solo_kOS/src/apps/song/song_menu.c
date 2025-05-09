#include "apps/song/song.h"
#include "libc/system.h"
#include "libc/stdio.h"
#include "common/monitor.h"
#include "common/input.h"

extern volatile int last_key;

// Function to run the song menu
void run_song_menu() {
    // Define the songs
    Song* songs[] = {
        &song_music_1,
        &song_starwars,
        &song_music_2,
        &song_music_3,
        &song_music_4,
        &song_music_5,
        &song_music_6
    };
    // Define the song names
    const char* song_names[] = {
        "Music 1",
        "Star Wars Theme",
        "Music 2",
        "Music 3",
        "Music 4",
        "Music 5",
        "Music 6",
        "Back to Main Menu" // Extra menu entry
    };
    // Create a song player
    const int song_count = sizeof(songs) / sizeof(Song*);
    const int menu_count = song_count + 1; // +1 for the "Back" option
    SongPlayer* player = create_song_player();
    int selected = 0;

    // Start loop for the menu
    // This loop will run until the user selects "Back to Main Menu"
    while (1) {
        monitor_clear();
        printf("=== Music Player ===\n");
        printf("Use arrow keys to select a song. Press ENTER to play.\n\n");

        // Display the menu
        for (int i = 0; i < menu_count; i++) {
            if (i == selected) {
                printf("  > [%d] %s <\n", i + 1, song_names[i]);
            } else {
                printf("    [%d] %s\n", i + 1, song_names[i]);
            }
        }

        last_key = 0;
        while (last_key == 0) {
            sleep_busy(50);
        }
        // Handle key presses
        if (last_key == 1 && selected > 0) {
            selected--; // UP
        } else if (last_key == 2 && selected < menu_count - 1) {
            selected++; // DOWN
        } else if (last_key == 6) { // ENTER
            if (selected == menu_count - 1) {
                // User chose "Back to Main Menu"
                return;
            }

            monitor_clear();
            
            printf("Now playing: %s\n\n", song_names[selected]);
            player->play_song(songs[selected]);
            printf("Song finished.\n");
        }

        last_key = 0;
    }
}
