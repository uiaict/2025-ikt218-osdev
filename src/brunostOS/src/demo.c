#include "demo.h"
#include "io.h"
#include "libc/stdio.h"
#include "song/song.h"
#include "memory/memory.h"
#include "memory/memutils.h"
#include "libc/limits.h"
#include "timer.h"

void print_main_menu(){
    printf("1:set terminal color\n\r");
    printf("2:freewrite\n\r");
    printf("3:print memory layout\n\r");
    printf("4:music player\n\r");
    printf("5:paint\n\r");
    printf("6:crash system (memory panic)\n\r");
    printf("\n\rinput:");
}
void print_color_menu(){
    set_vga_color(VGA_WHITE, VGA_RED);
    printf("R");
    set_vga_color(VGA_WHITE, VGA_GREEN);
    printf("G");
    set_vga_color(VGA_WHITE, VGA_BLUE);
    printf("B");
    set_vga_color(VGA_WHITE, VGA_CYAN);
    printf("C");
    set_vga_color(VGA_WHITE, VGA_MAGENTA);
    printf("M");
    set_vga_color(VGA_WHITE, VGA_GREY);
    printf("Y");
    set_vga_color(VGA_WHITE, VGA_BLACK);
    printf("K\n\r");
    set_vga_color(VGA_WHITE, VGA_LIGHT_RED);
    printf("r");
    set_vga_color(VGA_WHITE, VGA_LIGHT_GREEN);
    printf("g");
    set_vga_color(VGA_WHITE, VGA_LIGHT_BLUE);
    printf("b");
    set_vga_color(VGA_WHITE, VGA_LIGHT_CYAN);
    printf("c");
    set_vga_color(VGA_WHITE, VGA_LIGHT_MAGENTA);
    printf("m");
    set_vga_color(VGA_WHITE, VGA_LIGHT_GREY);
    printf("y");
    set_vga_color(VGA_BLACK, VGA_WHITE);
    printf("W");

    set_vga_color(VGA_WHITE, VGA_BLACK);
}

enum vga_color color_selection(enum vga_color def, int c){
    switch (c){
        case 'R':
            return VGA_RED;
        case 'G':
            return VGA_GREEN;
        case 'B':
            return VGA_BLUE;
        case 'C':
            return VGA_CYAN;
        case 'M':
            return VGA_MAGENTA;
        case 'Y':
            return VGA_GREY;
        case 'K':
            return VGA_BLACK;
        case 'k':
            return VGA_BLACK;
        case 'r':
            return VGA_LIGHT_RED;
        case 'g':
            return VGA_LIGHT_GREEN;
        case 'b':
            return VGA_LIGHT_BLUE;
        case 'c':
            return VGA_LIGHT_CYAN;
        case 'm':
            return VGA_LIGHT_MAGENTA;
        case 'y':
            return VGA_LIGHT_GREY;
        case 'W':
            return VGA_WHITE;
        case 'w':
            return VGA_WHITE;
        default:
            return def;
    }
}

void change_terminal_color(){
    enum vga_color txt_clr = get_vga_txt_clr();
    enum vga_color bg_clr = get_vga_bg_clr();
    print_color_menu();
    printf("\n\r\n\rtext color:");
    int c = getchar();
    txt_clr = color_selection(txt_clr, c);
    printf("\n\r\n\rbackground color:");
    c = getchar();
    bg_clr = color_selection(bg_clr, c);
    set_vga_color(txt_clr, bg_clr);
}

void print_music_menu(){
    printf("1:SMB 1-1\n\r");
    printf("2:imperial march\n\r");
    printf("3:battlefield 1942 theme\n\r");
    printf("4:song 2\n\r");
    printf("5:ode to joy\n\r");
    printf("6:brother john\n\r");
    printf("7:song 5\n\r");
    printf("8:song 6\n\r");
    printf("9:megalovania\n\r");
    printf("\n\rinput:");
}

void music_player(){
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
    int c = 0;
    while (c != 1){
        reset_cursor_pos();
        clear_terminal();
        print_music_menu();
        c = getchar();
        switch (c){

            case '1':
                printf("\n\rplaying:SMB 1-1");
                player->play_song(&songs[0]);
                break;
            case '2':
                printf("\n\rplaying:imperial march");
                player->play_song(&songs[1]);
                break;
            case '3':
                printf("\n\rplaying:battlefield 1942 theme");
                player->play_song(&songs[2]);
                break;
            case '4':
                printf("\n\rplaying:song 2");
                player->play_song(&songs[3]);
                break;
            case '5':
                printf("\n\rplaying:ode to joy");
                player->play_song(&songs[4]);
                break;
            case '6':
                printf("\n\rplaying:brother john");
                player->play_song(&songs[5]);
                break;
            case '7':
                printf("\n\rplaying:song 5");
                player->play_song(&songs[6]);
                break;
            case '8':
                printf("\n\rplaying:song 6");
                player->play_song(&songs[7]);
                break;
            case '9':
                printf("\n\rplaying:megalovania");
                player->play_song(&songs[8]);
                break;
            
            default:
                break;
        }
    }
    free(player);
}


void suicide(){
    for (size_t i = 1; true; i++){
        while (true){
            void *a = malloc(INT_MAX/i);
            if (!a){
                memset(a, 0, INT_MAX/i);
            } 
        }   
    } 
}