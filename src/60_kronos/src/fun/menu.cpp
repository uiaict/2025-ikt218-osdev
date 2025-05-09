#include "musicplayer/song.h"
#include "libcpp/stddef.h"

extern "C"  
{
    #include "libc/stdio.h"
    #include "fun/menu.h"
    #include "sys/io.h"
    #include "drivers/keyboard.h"
    #include "fun/donut.h"
    #include "kernel/pit.h"
}



void init_menu() {
    terminal_clear();

    printf("__     _  __                 ____   _____     __\n");
    printf("\\ \\   | |/ /                / __ \\ / ____|   / /\n");
    printf(" | |  | ' / _ __ ___  _ __ | |  | | (___    | | \n");
    printf(" | |  |  < | '__/ _ \\| '_ \\| |  | |\\___ \\   | | \n");
    printf(" | |  | . \\| | | (_) | | | | |__| |____) |  | | \n");
    printf(" | |  |_|\\_\\_|  \\___/|_| |_|\\____/|_____/   | | \n");
    printf("/_/                                          \\_\\\n");
    
    printf("________________________________________________\n\n");
    printf("[1]: Spin a donut\n");
    printf("[2]: Play music\n");
    printf("[3]: Toggle printing interupts\n");
    printf("[4]: Print memory layout\n");

    Song* time_song = new Song {song_of_time, sizeof(song_of_time) / sizeof(Note)};
    SongPlayer* player = create_song_player();

    bool printing_interupts = false;

    while (1) {
        if (is_key_pressed('1')) {
            animate_donut();
            break;
        } else if (is_key_pressed('2')) {
            printf("\n");
            player->play_song(time_song);
            keyboard_get_last_char();
            break;
        } else if (is_key_pressed('3')) {
            printf("\n"); 
            printing_interupts = !printing_interupts;
            if (printing_interupts) {
                printf("Printing interrupts enabled\n");
            } else {
                printf("Printing interrupts disabled\n");
            }
            if (printing_interupts) {
                for (int i = 0; i < 256; i++) {
                    if (i == 32 || i == 33) {
                        continue;
                    }
                    register_interrupt_handler(i, print_interrupts);
                }

                printf("Simulating interrupt 1, 3 and 4\n");
                asm volatile("int $1");
                asm volatile("int $3");
                asm volatile("int $4");
            } else {
                for (int i = 0; i < 256; i++) {
                    if (i == 32 || i == 33) {
                        continue;
                    }
                    register_interrupt_handler(i, nullptr);
                }
            }

            
            keyboard_get_last_char();
        } else if (is_key_pressed('4')) {
            printf("\n");
            print_memory_layout();
        } else {
            keyboard_get_last_char();
        }
    }

    sleep_busy(10);

    delete time_song;
    delete player;
}