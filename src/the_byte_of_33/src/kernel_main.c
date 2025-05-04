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

// Print the main selection menu
static void print_main_menu(void) {
    set_color(0x0E); // Yellow text
    puts("\nSelect mode:\n");
    puts("  [i] Matrix mode");
    puts("  [m] Music player");
    puts("  [p] Piano mode");
    puts("  [t] Test mode");
    puts("  [h] Print heap layout");
}

// Run ISR tests
void run_isr_tests(void) {
    __asm__ volatile("int $0");
    __asm__ volatile("int $1");
    __asm__ volatile("int $2");
}

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
    KernelMode mode = MODE_NONE;
    uint32_t counter = 0;
    char last_key = 0;

    print_main_menu();

    // Main loop
    while (true) {
        char current_key = keyboard_get_last_char();
        if (last_key != current_key && current_key != 0) {
            last_key = current_key;
            puts("Key pressed: "); putchar(current_key); puts("\n");

            if (mode == MODE_NONE) {
                switch (last_key) {
                    case 'i':
                        set_color(0x0B);
                        puts("Switched to Matrix mode");
                        matrix_mode();
                        mode = MODE_NONE;
                        keyboard_clear_last_char();
                        last_key = 0;
                        print_main_menu();
                        break;
                    case 'm':
                        mode = MODE_MUSIC_PLAYER;
                        set_color(0x0B);
                        puts("Switched to Music Player mode.\n");
                        puts("[n] Next song");
                        puts("[b] Previous song");
                        puts("[s] Select song");
                        puts("[q] Quit to main menu");
                        break;
                    case 'p':
                        set_color(0x0B);
                        puts("Switched to Piano mode\n");
                        piano_mode();
                        print_main_menu();
                        break;
                    case 't':
                        mode = MODE_TEST;
                        set_color(0x0B);
                        puts("Entered test mode\n");
                        break;
                    case 'h':
                        puts("\n=== Current Heap Layout ===\n");
                        print_heap_blocks();
                        print_main_menu();
                        break;
                }
                keyboard_clear_last_char();
            } else if (mode == MODE_MUSIC_MENU) {
                if ((uint32_t)(last_key - '0') < n_songs) {
                    current_song = last_key - '0';
                    set_color(0x0B);
                    printf("Selected song %d, now playing...\n", current_song + 1);
                    mode = MODE_MUSIC_PLAYER;
                } else if (last_key == 'q') {
                    mode = MODE_NONE;
                    print_main_menu();
                }
                keyboard_clear_last_char();
            } else if (mode == MODE_TEST) {
                // Memory tests
                void* a = malloc(1024);
                void* b = malloc(2048);
                void* c = malloc(4096);
                puts("\nHeap after 3 mallocs:"); print_heap_blocks();
                free(b); puts("\nAfter freeing b:"); print_heap_blocks();
                void* d = malloc(1024); puts("\nAfter reallocating d:"); print_heap_blocks();
                free(a); free(c); free(d);

                // ISR tests
                puts("Triggering ISR tests...\n");
                run_isr_tests();

                // Sleep tests
                printf("[%d]: Busy-wait sleep...\n", counter);
                sleep_busy(1000); printf("[%d]: Done.\n", counter++);
                printf("[%d]: Interrupt sleep...\n", counter);
                sleep_interrupt(1000); printf("[%d]: Done.\n", counter++);

                mode = MODE_NONE;
                keyboard_clear_last_char();
                print_main_menu();
            }
        }

        if (mode == MODE_MUSIC_PLAYER) {
            if (!player) player = create_song_player();
            if (player && !player->is_playing) {
                printf("Playing song %d...\n", current_song + 1);
                SongResult res = player->play_song(player, &songs[current_song]);
                switch (res) {
                    case SONG_COMPLETED:
                        current_song = (current_song + 1) % n_songs;
                        break;
                    case SONG_INTERRUPTED_N:
                        current_song = (current_song + 1) % n_songs;
                        break;
                    case SONG_INTERRUPTED_B:
                        current_song = (current_song + n_songs - 1) % n_songs;
                        break;
                    case SONG_INTERRUPTED_Q:
                        puts("Exiting Music Player mode.\n");
                        free_song_player(player);
                        player = NULL;
                        mode = MODE_NONE;
                        print_main_menu();
                        last_key = 0;
                        break;
                    case SONG_INTERRUPTED_S:
                        mode = MODE_MUSIC_MENU;
                        set_color(0x0C);
                        puts("\nSong Selection Menu:");
                        for (uint32_t i = 0; i < n_songs; i++)
                            printf("  [%d] Song %d\n", i, i+1);
                        puts("Press 0-5 or q");
                        break;
                }
            }
        }
    }

    if (player) free_song_player(player);
    return 0;
}
