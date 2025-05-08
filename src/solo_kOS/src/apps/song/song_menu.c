#include "apps/song/song.h"
#include "libc/system.h"
#include "libc/stdio.h"
#include "common/monitor.h"
#include "common/input.h"

extern volatile int last_key;

void run_song_menu() {
    Song songs[] = {
        {music_1, sizeof(music_1) / sizeof(Note)},
        {starwars_theme, sizeof(starwars_theme) / sizeof(Note)},
        {music_2, sizeof(music_2) / sizeof(Note)},
        {music_3, sizeof(music_3) / sizeof(Note)},
        {music_4, sizeof(music_4) / sizeof(Note)},
        {music_5, sizeof(music_5) / sizeof(Note)},
        {music_6, sizeof(music_6) / sizeof(Note)}
    };

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

    const int song_count = sizeof(songs) / sizeof(Song);
    const int menu_count = song_count + 1; // +1 for the "Back" option
    SongPlayer* player = create_song_player();
    int selected = 0;

    while (1) {
        monitor_clear();
        printf("=== Music Player ===\n");
        printf("Use arrow keys to select a song. Press ENTER to play.\n\n");

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
            player->play_song(&songs[selected]);
            printf("Song finished.\n");
        }

        last_key = 0;
    }
}
