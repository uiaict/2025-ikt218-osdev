#include "musicplayer/song.h"
#include "libcpp/stddef.h"

extern "C"  
{
    #include "libc/stdio.h"
    #include "fun/menu.h"
    #include "sys/io.h"
    #include "drivers/keyboard.h"
    #include "fun/donut.h"
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

    Song* time_song = new Song {song_of_time, sizeof(song_of_time) / sizeof(Note)};
    SongPlayer* player = create_song_player();

    while (1) {
        if (is_key_pressed('1')) {
            animate_donut();
            break;
        } else if (is_key_pressed('2')) {
            player->play_song(time_song);
            printf("sadasd\n\n");
            break;
        }
    }

    terminal_clear();
    delete time_song;
    delete player;
}