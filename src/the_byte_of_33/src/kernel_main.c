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

static void isr0_handler(registers_t* r) {
    (void)r; // Ignore the dummy registers
    puts("Interrupt 0 (Divide by Zero) handled\n");
}

static void isr1_handler(registers_t* r) {
    (void)r;
    puts("Interrupt 1 (Debug) handled\n");
}

static void isr2_handler(registers_t* r) {
    (void)r;
    puts("Interrupt 2 (NMI) handled\n");
}

// Define modes for the kernel loop
typedef enum {
    MODE_NONE,
    MODE_MUSIC_PLAYER,
    MODE_MEMORY_TEST,
    MODE_PIANO,
    MODE_MATRIX,
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
    
    //__clear_screen();
    set_color(0x0E);
    puts("\nSelect mode:\n");
    puts("  [i] Matrix mode\n");
    puts("  [m] Music player\n");
    puts("  [p] Piano mode\n");
    puts("  [s] Test mode\n");
    puts("  [h] print heap layout"); //skal flyttes til test
    puts("  [t] Memory test\n");
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
            } else if (last_key == 'h') {
                puts("\n=== Current Heap Layout ===\n");
                print_heap_blocks();
            } else if (last_key == 'p') {
                //mode = MODE_PIANO;
                puts("Switched to Piano mode\n");
            } else if (last_key == 'i') {
                //mode = MODE_MATRIX;
                puts("Switched to Matrix mode\n");
            } else if (last_key == 't') {
                mode = MODE_MEMORY_TEST;
                puts("Entered test mode");
            }
            keyboard_clear_last_char();
        }

        
        if (mode == MODE_PIANO) {
            //piano_mode();
        } else if (mode == MODE_MATRIX) {
            //matrix_mode();
        }   else if (mode == MODE_MEMORY_TEST) {            
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

            register_interrupt_handler(0, isr0_handler);
            register_interrupt_handler(1, isr1_handler);
            register_interrupt_handler(2, isr2_handler);

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
