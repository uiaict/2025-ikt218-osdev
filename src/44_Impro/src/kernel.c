#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/string.h"
#include "memory.h"
#include "gdt.h"
#include <multiboot2.h>
#include "idt.h"
#include "vga.h"
#include "paging.h"
#include "pit.h"
#include "song/song.h"
#include "libc/stdio.h"
#include "random.h"

extern uint32_t end;

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};






// Outside of main
SongPlayer* create_song_player() {
    SongPlayer* player = (SongPlayer*)malloc(sizeof(SongPlayer));
    player->play_song = play_song_impl;
    return player;
}

// In main or another function
void play_music() {
    // How to play music
    Song songs[] = {
        {music_1, sizeof(music_1) / sizeof(Note)},
        {starwars_theme, sizeof(starwars_theme) / sizeof(Note)},
        {battlefield_1942_theme, sizeof(battlefield_1942_theme) / sizeof(Note)}
    };
    uint32_t n_songs = sizeof(songs) / sizeof(Song);

    SongPlayer* player = create_song_player();
    printf("number of songs: %d\n", n_songs);
    for(uint32_t i = 0; i < n_songs; i++) {
        printf("Playing Song...\n");
        player->play_song(&songs[i]);
        printf("Finished playing the song.\n");
    }
    
    // Note: This code will never reach here due to infinite loop,
    // but good practice would be to free the player when done:
    free(player);
}

extern void printf_char();







int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    init_gdt();
    init_idt();
    init_kernel_memory(&end);
    init_paging();
    init_pit();
    printf("everything is ok\n\r");
    sleep_busy(1000);
    clear();
    malloc(1000);

    //test malloc of pointer
    /*
    print_memory_layout();
    char* test = (char*)malloc(100);
    print_memory_layout();
    free(test);
    print_memory_layout();
    */
  
    while (1);
    

}