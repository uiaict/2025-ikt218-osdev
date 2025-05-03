#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/stdio.h"

#include "kernel_memory.h"
#include "pit.h"
#include "paging.h"
#include "interrupt.h"
#include "io.h"
#include "song/song.h"
#include "keyboard.h"

// Define modes for the kernel loop
typedef enum {
    MODE_NONE,
    MODE_SLEEP_TEST,
    MODE_MUSIC_PLAYER
} KernelMode;

int kernel_main() {
    set_color(0x0B); // light cyan text
    puts("=== Entered kernel_main ===\n");

    //Music player setup
    Song songs[] = {
        {music_1, sizeof(music_1) / sizeof(Note)},
        {music_2, sizeof(music_2) / sizeof(Note)},
        {music_3, sizeof(music_3) / sizeof(Note)},
        {music_4, sizeof(music_4) / sizeof(Note)},
        {music_5, sizeof(music_5) / sizeof(Note)},
        {music_6, sizeof(music_6) / sizeof(Note)}
    };
    uint32_t n_songs = sizeof(songs) / sizeof(Song);
    SongPlayer* player = create_song_player();
    uint32_t current_song = 0;

    // Mode control
    KernelMode mode = MODE_NONE; // Start with no mode
    uint32_t counter = 0;
    char last_key = 0;
    
    puts("Press 'm' to toggle music player, 's' to toggle sleep tests, 'n' to skip song in music mode, 'h' to print memory tets.\n");
    // Infinite loop
    while (true) {
        // Check for keyboard input to toggle mode
        char current_key = keyboard_get_last_char();
        if(last_key != current_key && current_key != 0) {
            last_key = current_key;
            puts("Key pressed: ");
            putchar(current_key);
            puts("\n");

            if(last_key == 'm') {
                mode = MODE_MUSIC_PLAYER;
                puts("Switched to Music Player mode.\n");
            } else if(last_key == 's') {
                mode = MODE_SLEEP_TEST;
                puts("Switched to Sleep Test mode.\n");
            } else if (last_key == 'h') {
                mode = MODE_MEMORY_TEST;
                puts("Switched to Memory Test mode.\n");
            }
            keyboard_clear_last_char();
        }


        if (mode == MODE_MEMORY_TEST) {
            
        void* a = malloc(1024);
        void* b = malloc(2048);
        void* c = malloc(4096);

        puts("\nHeap after 3 mallocs:");
        print_heap_blocks();

        free(b);

        puts("\nHeap after freeing the second block (b):");
        print_heap_blocks();

        void* d = malloc(1024);

        puts("\nHeap after reallocating a smaller block (should reuse free block):");
        print_heap_blocks();

        free(a);
        free(c);
        free(d);
        }

        if (mode == MODE_SLEEP_TEST) {
            // Original sleep test functionality
            printf("[%d]: Sleeping with busy-waiting (HIGH CPU).\n", counter);
            sleep_busy(1000); // Sleep 1000 milliseconds (1 second)
            printf("[%d]: Slept using busy-waiting.\n", counter++);
            
            printf("[%d]: Sleeping with interrupts (LOW CPU).\n", counter);
            sleep_interrupt(1000); // Sleep 1000 milliseconds (1 second)
            printf("[%d]: Slept using interrupts.\n", counter++);
        } else if (mode == MODE_MUSIC_PLAYER) {
            // Play the current song
            printf("Playing song %d...\n", current_song + 1);
            bool completed = player->play_song(&songs[current_song]);
            if (completed) {
                printf("Finished playing song %d.\n", current_song + 1);
                // Move to the next song if it completed normally
                current_song = (current_song + 1) % n_songs;
            } else {
                // Song was interrupted (e.g., by pressing 'n')
                printf("Skipping to song %d.\n", current_song + 2); // +2 because we're about to increment
                current_song = (current_song + 1) % n_songs;
            }
        }
    }

    // free(player) unnecessary for now since its used indefinitely, but after future additions add it idk
    return 0;
}
