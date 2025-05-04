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
#include "kernel_main.h"
#include "matrix_mode.h"
#include "piano_mode.h"

int kernel_main() {
    set_color(0x0B); // Light cyan text
    puts("=== Entered kernel_main ===\n");

    // Music player setup
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

    // Initial menu
    set_color(0x0E); // Yellow text
    puts("\nSelect mode:\n");
    puts("  [i] Matrix mode\n");
    puts("  [m] Music player\n");
    puts("  [p] Piano mode\n");
    puts("  [t] Test mode\n");
    puts("  [h] print heap layout\n");

    // Infinite loop
    while (true) {
        char current_key = keyboard_get_last_char();
        if (last_key != current_key && current_key != 0) {
            last_key = current_key;
            puts("Key pressed: ");
            putchar(current_key);
            puts("\n");

            if (mode == MODE_NONE) {
                if (last_key == 'm') {
                    mode = MODE_MUSIC_MENU;
                    set_color(0x0B); // Light cyan text
                    puts("Switched to Music Player mode.\n");
                    set_color(0x0C); // Red text for menu
                        puts("\nSong Selection Menu:\n");
                        for (uint32_t i = 0; i < n_songs; i++) {
                            printf("[%d] Song %d\n", i, i + 1);
                        }
                        puts("Press 0-5 to select a song, or q to return\n");
                } else if (last_key == 'h') {
                    puts("\n=== Current Heap Layout ===\n");
                    print_heap_blocks();
                    puts("Switched to Music Player mode.\n");
                    puts("[n] Next song\n");
                    puts("[b] Previous song\n");
                    puts("[s] Select song\n");
                    puts("[q] Quit to main menu\n");
                } else if (last_key == 'p') {
                    puts("Switched to Piano mode\n");
                    piano_mode();
                } else if (last_key == 'i') {
                    puts("Switched to Matrix mode\n");
                    matrix_mode();
                } else if (last_key == 't') {
                    mode = MODE_TEST;
                    puts("Entered test mode\n");
                }
                keyboard_clear_last_char();
            } else if (mode == MODE_MUSIC_MENU) {
                if (last_key >= '0' && last_key <= '5') {
                    uint32_t selected_song = last_key - '0';
                    if (selected_song < n_songs) {
                        current_song = selected_song;
                        mode = MODE_MUSIC_PLAYER;
                        set_color(0x0B); // Light cyan text
                        printf("Selected song %d, now playing...\n", current_song + 1);
                        puts("[n] Next song\n");
                        puts("[b] Previous song\n");
                        puts("[s] Select song\n");
                        puts("[q] Quit to main menu\n");
                    }
                } else if (last_key == 'q') {
                    mode = MODE_MUSIC_PLAYER;
                    set_color(0x0B); // Light cyan text
                    puts("Returned to Music Player mode.\n");
                    puts("[n] Next song\n");
                    puts("[b] Previous song\n");
                    puts("[s] Select song\n");
                    puts("[q] Quit to main menu\n");
                }
                keyboard_clear_last_char();
            } else if (mode == MODE_TEST) {
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

                puts("Triggering ISR tests....\n");
                run_isr_tests();

                printf("[%d]: Sleeping with busy-waiting (HIGH CPU).\n", counter);
                sleep_busy(1000);
                printf("[%d]: Slept using busy-waiting.\n", counter++);

                printf("[%d]: Sleeping with interrupts (LOW CPU).\n", counter);
                sleep_interrupt(1000);
                printf("[%d]: Slept using interrupts.\n", counter++);

                mode = MODE_NONE;
                keyboard_clear_last_char();
            }
        }

        // Mode-specific updates without keypress
        if (mode == MODE_MUSIC_PLAYER) {
            if (!player) {
                player = create_song_player();
            }
            if (player && !player->is_playing) {
                printf("Playing song %d...\n", current_song + 1);
                SongResult result = player->play_song(player, &songs[current_song]);
                switch (result) {
                    case SONG_COMPLETED:
                        printf("Finished playing song %d.\n", current_song + 1);
                        current_song = (current_song + 1) % n_songs;
                        break;
                    case SONG_INTERRUPTED_N:
                        printf("Song interrupted, skipping to song %d.\n", current_song + 2);
                        current_song = (current_song + 1) % n_songs;
                        break;
                    case SONG_INTERRUPTED_Q:
                        puts("Exiting Music Player mode.\n");
                        if (player) {
                            free_song_player(player);
                            player = NULL;
                        }
                        mode = MODE_NONE;
                        set_color(0x0E); // Yellow text
                        puts("\nSelect mode:\n");
                        puts("[i] Matrix mode\n");
                        puts("[m] Music player\n");
                        puts("[p] Piano mode\n");
                        puts("[t] Test mode\n");
                        puts("[h] print heap layout\n");
                        break;
                    case SONG_INTERRUPTED_S:
                        mode = MODE_MUSIC_MENU;
                        set_color(0x0C); // Red text for menu
                        puts("\nSong Selection Menu:\n");
                        for (uint32_t i = 0; i < n_songs; i++) {
                            printf("[%d] Song %d\n", i, i + 1);
                        }
                        puts("Press 0-5 to select a song, or q to return\n");
                        break;
                    case SONG_INTERRUPTED_B:
                        if (current_song == 0) {
                            current_song = n_songs - 1;
                        } else {
                            current_song = (current_song - 1) % n_songs;
                        }
                        printf("Going back to song %d.\n", current_song + 1);
                        break;
                }
            }
        }
    }
    // Clean up if loop exits (unreachable in practice)
    if (player) {
        free_song_player(player);
    }
    return 0;
}

// Kan skille ut men ikke så viktig
void run_isr_tests(void) {
    __asm__ volatile("int $0");  // Divide by zero
    __asm__ volatile("int $1");  // Debug
    __asm__ volatile("int $2");  // NMI
}