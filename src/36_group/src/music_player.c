#include "music_player.h"
#include "song.h"
#include "keyboard.h"
#include "io.h"
#include "libc/stdio.h"

void clear_screen(void);

static Song songs[] = {
    {music_1, sizeof(music_1) / sizeof(Note)},
    {music_2, sizeof(music_2) / sizeof(Note)},
    {music_3, sizeof(music_3) / sizeof(Note)},
    {music_4, sizeof(music_4) / sizeof(Note)},
    {music_5, sizeof(music_5) / sizeof(Note)},
    {music_6, sizeof(music_6) / sizeof(Note)},
};

static uint32_t n_songs = sizeof(songs) / sizeof(Song);
static SongPlayer *player = NULL;
static uint32_t current_song = 0;
static bool is_playing = false;

static void draw_outline(void)
{
    puts("+----------------------------------------+\n");
}

static const char *get_song_name(uint32_t index, bool is_playing)
{
    static char name[16];
    if (is_playing)
    {
        name[0] = 'S';
        name[1] = 'o';
        name[2] = 'n';
        name[3] = 'g';
        name[4] = ' ';
        name[5] = '1' + index;
        name[6] = '\0';
    }
    else
    {
        name[0] = '-';
        name[1] = '\0';
    }
    return name;
}

static void print_music_box_header(void)
{
    draw_outline();
    puts("|             Music Player              |\n");
    draw_outline();
    printf(" Now playing: %s\n\n", get_song_name(current_song, is_playing));
    draw_outline();
}

void music_player_show_menu(void)
{
    clear_screen();
    set_color(0x0B);
    print_music_box_header();
    puts("| Libary:\n");
    for (uint32_t i = 0; i < n_songs; i++)
    {
        printf("|  [%d] Song %d                           |\n", i + 1, i + 1);
    }
    draw_outline();
    puts("| Press 1-6, or Backspace to return     |\n");
    draw_outline();
}

KernelMode music_player_handle_input(char key)
{
    if ((uint32_t)(key - '1') < n_songs)
    {
        current_song = key - '1';
        clear_screen();
        set_color(0x0B);
        is_playing = true;
        print_music_box_header();
        puts("| [A] Previous song                     |\n");
        puts("| [S] Select song                       |\n");
        puts("| [D] Next song                         |\n");
        puts("| [Backspace] Main menu                 |\n");
        draw_outline();
        return MODE_MUSIC_PLAYER;
    }
    else if (key == '\b')
    {
        return MODE_NONE;
    }
    return MODE_MUSIC_MENU;
}

KernelMode music_player_update(void)
{
    if (!player)
        player = create_song_player();

    if (player && !player->is_playing)
    {
        SongResult res = player->play_song(player, &songs[current_song]);

        switch (res)
        {
        case SONG_COMPLETED:
        case SONG_INTERRUPTED_NEXT:
            current_song = (current_song + 1) % n_songs;
            break;

        case SONG_INTERRUPTED_PREV:
            current_song = (current_song + n_songs - 1) % n_songs;
            break;

        case SONG_INTERRUPTED_BACK:
            puts("Exiting Music Player mode.\n");
            free_song_player(player);
            player = NULL;
            return MODE_NONE;

        case SONG_INTERRUPTED_SELECT:
            clear_screen();
            set_color(0x0B);
            is_playing = false;
            print_music_box_header();
            puts(" Libary:\n");
            for (uint32_t i = 0; i < n_songs; i++)
                printf("|  [%d] Song %d                           |\n", i + 1, i + 1);
            puts("| Press 1-6, or Backspace to return     |\n");
            draw_outline();
            return MODE_MUSIC_MENU;
        }

        if (res != SONG_INTERRUPTED_SELECT && res != SONG_INTERRUPTED_BACK)
        {
            clear_screen();
            set_color(0x0B);
            is_playing = true;
            print_music_box_header();
            puts("| [A] Previous song                     |\n");
            puts("| [S] Select song                       |\n");
            puts("| [D] Next song                         |\n");
            puts("| [Backspace] Main menu                 |\n");
            draw_outline();
        }
    }

    return MODE_MUSIC_PLAYER;
}

void music_player_cleanup(void)
{
    if (player)
    {
        free_song_player(player);
        player = NULL;
    }
}
