#include "apps/game/snake.h"
#include "apps/song/song.h"
#include "apps/game/snake.h"
#include "apps/song/song.h"

// Single global player created at init
static SongPlayer *player;

void game_sound_init(void)
{
    player = create_song_player();
}

// Short beep when food is collected 
void game_sound_food(void)
{
    extern Song song_food;
    disable_speaker();
    player->play_song(&song_food);
}

// Longer intro melody at game start 
void game_sound_opening(void)
{
    extern Song song_open;
    disable_speaker();
    player->play_song(&song_open);
}

// Confirmation sound, e.g., menu selection
void game_sound_confirm(void)
{
    static Note confirm_beep[] = {
        {C6, 100},
    };
    Song s = { confirm_beep, sizeof(confirm_beep) / sizeof(Note) };
    disable_speaker();
    player->play_song(&s);
}

// Failure sound, e.g., wall hit or invalid action
void game_sound_fail(void)
{
    static Note fail_beep[] = {
        {C3, 150},
    };
    disable_speaker();
    Song s = { fail_beep, sizeof(fail_beep) / sizeof(Note) };
    player->play_song(&s);
}

// Pause toggle beep
void game_sound_toggle(void)
{
    static Note pause_beep[] = {
        {C5, 75},
    };
    disable_speaker();
    Song s = { pause_beep, sizeof(pause_beep) / sizeof(Note) };
    player->play_song(&s);
}
