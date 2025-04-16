#include "libcpp/stddef.h"
#include "musicplayer/song.h"

extern "C"  
{
    #include "libc/stdio.h"
    
    int kernel_main(void);
}

int kernel_main() {
    printf("Print from kernel_main\n");


    Song* songs[] = {
        new Song({starwars_theme, sizeof(starwars_theme) / sizeof(Note)}),
        new Song({battlefield_1942_theme, sizeof(battlefield_1942_theme) / sizeof(Note)}),
        new Song({music_1, sizeof(music_1) / sizeof(Note)}),
        new Song({music_2, sizeof(music_2) / sizeof(Note)}),
        new Song({music_3, sizeof(music_3) / sizeof(Note)}),
        new Song({music_4, sizeof(music_4) / sizeof(Note)}),
        new Song({music_5, sizeof(music_5) / sizeof(Note)}),
        new Song({music_6, sizeof(music_6) / sizeof(Note)})    
    };

    uint8_t num_songs = sizeof(songs) / sizeof(Song*);
    SongPlayer* player = create_song_player();
    while (true) {
        for (uint8_t i = 0; i < num_songs; i++) {
            printf("Playing song %d\n", i);
            player->play_song(songs[i]);
        }
    }
    

    for(;;) {
        // Halt CPU until next interrupt
        asm volatile("hlt");
    }
    return 0;
}