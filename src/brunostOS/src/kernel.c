#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/string.h"
#include "libc/stdio.h"
#include <multiboot2.h>
#include "descriptor_tables.h"
#include "keyboard.h"
#include "timer.h"
#include "speaker.h"
#include "song/song.h"
#include "io.h"
#include "libc/stdlib.h"
#include "memory/memory.h"
#include "memory/paging.h"
#include "wave.h"

extern uint32_t end;

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};


int main(uint32_t magic, struct multiboot_info *mb_info_addr) {
    set_vga_color(VGA_GREEN, VGA_BLACK);
    init_gdt();
    init_idt();
    init_keyboard();
    init_pit(500);
    init_kernel_memory(&end);
    init_paging();
    enable_speaker();
    stop_sound();
    set_freewrite(true);
    // freewrite();

    char painting1[STORAGE_SPACE];
    char painting2[STORAGE_SPACE];
    paint(painting1, painting2);


    struct song songs[] = {
        {SMB_1_1, sizeof(SMB_1_1) / sizeof(SMB_1_1[0])},
        {imperial_march, sizeof(imperial_march) / sizeof(imperial_march[0])},
        {battlefield_1942_theme, sizeof(battlefield_1942_theme) / sizeof(battlefield_1942_theme[0])},
        {music_2, sizeof(music_2) / sizeof(music_2[0])},
        {ode_to_joy, sizeof(ode_to_joy) / sizeof(ode_to_joy[0])},
        {brother_john, sizeof(brother_john) / sizeof(brother_john[0])},
        {music_5, sizeof(music_5) / sizeof(music_5[0])},
        {music_6, sizeof(music_6) / sizeof(music_6[0])},
        {megalovania, sizeof(megalovania) / sizeof(megalovania[0])}
    };


    struct song_player *player = create_song_player();
    // uint32_t n_songs = sizeof(songs) / sizeof(songs[0]);
    // player->play_song(&songs[1]);
    free(player);

    // print("       _.---._    /\\\n\r"
    //    "    ./'       \"--`\\//\n\r"
    //    "  ./              o \\          .-----.\n\r"
    //    " /./\\  )______   \\__ \\        ( help! )\n\r"
    //    "./  / /\\ \\   | \\ \\  \\ \\       /`-----'\n\r"
    //    "   / /  \\ \\  | |\\ \\  \\7--- ooo ooo ooo ooo ooo ooo\n\r");


    while (true){
        /* code */
    }
    
    
//TODO: 
//io buffer overflow
//memory
//malloc musicplayer
//improv
    return 0;
}