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
    clear_screen();
    set_color(0x0E); // Yellow text

    // “The byte of 33” logo
    puts("                                                                          \n");
    puts("  _______ _            ____        _                __   ____    ____    \n");
    puts(" |__   __| |          |  _ \\      | |              / _| |___ \\  |___ \\   \n");
    puts("    | |  | |__   ___  | |_) |_   _| |_ ___    ___ | |_    __) |   __) |  \n");
    puts("    | |  | '_ \\ / _ \\ |  _ <| | | | __/ _ \\  / _ \\|  _|  |__ <|  |__ <|   \n");
    puts("    | |  | | | |  __/ | |_) | |_| | ||  __/ | (_) | |    ___) |  ___) |  \n");
    puts("    |_|  |_| |_|\\___| |____/ \\__, |\\__\\___|  \\___/|_|   |____/  |____/   \n");
    puts("                              __/ |                                      \n");
    puts("                             |___/                                       \n");
    puts("\n");  // blank line

    // Menu
    puts("  Select mode:\n");
    puts("  [i] Matrix mode\n");
    puts("  [m] Music player\n");
    puts("  [p] Piano mode\n");
    puts("  [t] Test mode\n");
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
                        clear_screen();
                        set_color(0x0B);
                        puts("Switched to Matrix mode");
                        matrix_mode();
                        mode = MODE_NONE;
                        keyboard_clear_last_char();
                        last_key = 0;
                        print_main_menu();
                        break;
                    case 'm':
                        clear_screen();
                        mode = MODE_MUSIC_MENU;
                        set_color(0x0B); // Light cyan text
                        puts("Switched to Music Player mode.\n");
                        set_color(0x0C); // Red text for menu
                            puts("\nSong Selection Menu:\n");   
                            for (uint32_t i = 0; i < n_songs; i++) {
                                printf("[%d] Song %d\n", i, i + 1);
                            }
                            puts("Press 0-5 to select a song, or q to return\n");
                        break;
                    case 'p':
                        clear_screen();
                        set_color(0x0B);
                        puts("Switched to Piano mode\n");
                        piano_mode();
                        // Flush the last 'p' so you can re-enter piano mode with a single press
                        keyboard_clear_last_char();
                        last_key = 0;
                        print_main_menu();
                        break;
                    case 't':
                        clear_screen();
                        mode = MODE_TEST;
                        set_color(0x0B);
                        puts("Entered test mode: press any key to show\n");
                        break;
                }
                keyboard_clear_last_char();
            } else if (mode == MODE_MUSIC_MENU) {
                if ((uint32_t)(last_key - '0') < n_songs) {
                    current_song = last_key - '0';
                    set_color(0x0B);
                    clear_screen();
                    printf("Selected song %d, now playing...\n", current_song + 1);
                    puts("[n] Next song\n");
                    puts("[b] Previous song\n");
                    puts("[s] Select song\n");
                    puts("[q] Quit to main menu\n");
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
                sleep_interrupt(3000); printf("[%d]: Done.\n", counter++);

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
                        clear_screen();
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
